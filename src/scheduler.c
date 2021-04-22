/*
 * @File   : scheduler.c
 * @brief  : source file containing function definition for scheduler
 * @Author : Madhukar Arora
 * @References : Instructors Lecture Slides from Lecture 6 to design the scheduler.
 *
 */

#include "scheduler.h"
extern GNSS_data_t GNRMC_data;
// set BME sensor data wherever it should be set
BME_data_t BME_data = {
		15.23,
		83730	// DIA
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


	currentState = nextState;
	//LOG_INFO("INSIDE process_event %ld",evt->data.evt_system_external_signal.extsignals);
	switch(currentState){

	case POWER_ON:
		if((evt->data.evt_system_external_signal.extsignals) == TIMER_UF){
			CMU_ClockEnable(cmuClock_I2C0,true);

			gpioGpsToggleSetOn();
//			timerWaitUs(1000000);
			nextState = WRITE_BEGIN;
		}
		break;
	case WRITE_BEGIN:
//		if((evt->data.evt_system_external_signal.extsignals) == DELAY_GENERATED){
			sleep_block_on(sleepEM2);
			initLEUART();
			UARTDRV_Receive(gnssHandle0, leuartbuffer, 66, LEUART_rx_callback);	// start non blocking (LDMA) Rx
			timerWaitUs(1000000);
			sleep_block_off(sleepEM2);
			nextState = WRITE_DONE;
//		}
		break;
	case WRITE_DONE:
		if((evt->data.evt_system_external_signal.extsignals) == DELAY_GENERATED){
			sleep_block_on(sleepEM2);
			measure_navigation(&GNRMC_data);	// send gnsarray for parsing and send off
			BME_data.pressure += 1;//= getPressure();
			measure_pressure(&BME_data);
			BME_data.temperature += 0.1;//= getTemperature();
			measure_temperature(&BME_data);
			sleep_block_off(sleepEM2);
			nextState = WRITE_BEGIN;
		}
		break;
	case POWER_OFF:
		if((evt->data.evt_system_external_signal.extsignals) == I2C_TRANSFER_DONE){
			sleep_block_off(sleepEM2);
			NVIC_DisableIRQ(I2C0_IRQn);
			I2C_Reset(I2C0);
			I2C_Enable(I2C0,false);
			CMU_ClockEnable(cmuClock_I2C0,false);
			// disable leuart
			UARTDRV_DeInit(gnssHandle0);
//#if DEVKIT
//			scl_disable();
//			sda_disable();
//			bnoSDADisable();
//			bnoSCLDisable();
//#endif
		}
		nextState = POWER_ON;
		break;
	default:
		break;
	}
	if(currentState != nextState){
		LOG_DEBUG("TEMPERATURE SENSOR transitioned from state %d to state %d",currentState,nextState);
		currentState = nextState;
	}
}



