/*
 * @File   : scheduler.c
 * @brief  : source file containing function definition for scheduler
 * @Author : Madhukar Arora
 * @References : Instructors Lecture Slides from Lecture 6 to design the scheduler.
 *
 */

#include "scheduler.h"

// set BME sensor data wherever
BME_data_t BME_data = {
		1.234,
		1.2
};

extern UARTDRV_Handle_t  gnssHandle0;

static void sleep_block_on(SLEEP_EnergyMode_t sleep_mode){
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_CRITICAL();
	SLEEP_SleepBlockBegin(sleep_mode);
	CORE_EXIT_CRITICAL();

}

static void sleep_block_off(SLEEP_EnergyMode_t sleep_mode){
	CORE_DECLARE_IRQ_STATE;
	CORE_ENTER_CRITICAL();
	SLEEP_SleepBlockEnd(sleep_mode);
	CORE_EXIT_CRITICAL();
}
/**
 * @brief :  function to check if a particular event is set in the external signal
 * @param :  gecko command structure
 *           event to be checked if set
 * @return : true if present, else false.
 */
//static bool scheduler_ExternalSignalPresent(struct gecko_cmd_packet* evt,scheduler_events_t event){
//	if((evt->data.evt_system_external_signal.extsignals) == event){
//		return true;
//	}
//	else{
//		return false;
//	}
//}

/***************************************************************************//**
 *  Handling of external signal events.
 *
 *  @param[in] signal  External signal handle that is serviced by this function.
 ******************************************************************************/
void handle_external_signal_event(uint8_t signal){

}

/*
 * function sets the default value of the eventFlag
 * and the default event state
 */
void scheduler_Init(void)
{
	eventFlag = 0;

	temp_Si7021.data_8 = 0;
	temp_Si7021.data_16 = 0;
	temp_Si7021.temp_code = 0;
	temp_Si7021.tempC = 0;
}

/*
 * function to return event to the main
 */
bool scheduler_GetEvent(uint32_t event){
	CORE_DECLARE_IRQ_STATE;

	if((eventFlag & event) == event){
		//clear the event
		CORE_ENTER_CRITICAL();
		eventFlag &= ~ event;
		CORE_EXIT_CRITICAL();
		return true;

	}
	else{
		return false;
	}
}

/*
 * function will set a flag corresponding to an event
 */
void scheduler_SetEvent(uint32_t event){
	//set received event
	eventFlag |= event;
	gecko_external_signal(event);
}

/*
 * Reference : Took Instructors help
 */
bool scheduler_EventsPresent(void){

	if(eventFlag){
		return true;
	}
	else
		return false;
}


void process_event(struct gecko_cmd_packet* evt){

	scheduler_states_t currentState;
	static scheduler_states_t nextState = POWER_ON;
	static menu_states_t menustate;

	currentState = nextState;
	//LOG_INFO("INSIDE process_event %ld",evt->data.evt_system_external_signal.extsignals);
	switch(currentState){

	case POWER_ON:
		if((evt->data.evt_system_external_signal.extsignals) == TIMER_UF){
			CMU_ClockEnable(cmuClock_I2C0,true);
			i2cInit();
			initLEUART();
#if DEVKIT
//			si7021_enable();
#endif
			nextState = WRITE_BEGIN;
			timerWaitUs(80000);
		}
		break;
	case WRITE_BEGIN:
		if((evt->data.evt_system_external_signal.extsignals) == DELAY_GENERATED){
			sleep_block_on(sleepEM2);
//			temp_Si7021.data_8 = 0X04;
//			i2cWrite(0x10,&temp_Si7021.data_8,sizeof(uint8_t));
			nextState = WRITE_DONE;
		}
		break;
	case WRITE_DONE:
//		if((evt->data.evt_system_external_signal.extsignals) == I2C_TRANSFER_DONE){
//			NVIC_DisableIRQ(I2C0_IRQn);
			sleep_block_off(sleepEM2);
			timerWaitUs(10000);
			nextState = READ_BEGIN;
		//}
		break;
	case READ_BEGIN:
		if((evt->data.evt_system_external_signal.extsignals) == DELAY_GENERATED){
			sleep_block_on(sleepEM2);
//			temp_Si7021.data_8 = 0XE3;
//			i2cReadDataBlock(0x40,temp_Si7021.data_8,&temp_Si7021.data_16,sizeof(uint16_t));
			nextState = POWER_OFF;
		}
		break;
	case POWER_OFF:
//		LOG_INFO("Getting measurements");
		UARTDRV_Receive(gnssHandle0, leuartbuffer, 66, UART_rx_callback);	// start non blocking (LDMA) Rx
		measure_pressure(&BME_data);
		measure_temperature(&BME_data);
		switch(evt->data.evt_system_external_signal.extsignals){
		case PB_PAGE1:
			menustate = PAGE1;
			break;
		case PB_PAGE2:	// BUG: cannot switch menus when health thermometer service is being indicated
			menustate = PAGE2;
			break;
		default:
			break;
		}
		displayMenu(menustate);
		if((evt->data.evt_system_external_signal.extsignals) == I2C_TRANSFER_DONE){
			sleep_block_off(sleepEM2);
			NVIC_DisableIRQ(I2C0_IRQn);

#if DEVKIT
//			si7021_disable();
#endif
			I2C_Reset(I2C0);
			I2C_Enable(I2C0,false);
			CMU_ClockEnable(cmuClock_I2C0,false);
#if DEVKIT
			scl_disable();
			sda_disable();
#endif
			nextState = POWER_ON;
		}
		break;
	default:
		break;
	}
	if(currentState != nextState){
		LOG_DEBUG("TEMPERATURE SENSOR transitioned from state %d to state %d",currentState,nextState);
		currentState = nextState;
	}
}



