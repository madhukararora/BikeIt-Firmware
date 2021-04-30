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

//			timerWaitUs(1000000);
			nextState = START_DELAY;
		}
		break;
	case START_DELAY:
//		if((evt->data.evt_system_external_signal.extsignals) == DELAY_GENERATED){
			sleep_block_on(sleepEM2);
//			CMU_ClockEnable(cmuClock_I2C0,true);
//			timerWaitUs(2000000);
			sleep_block_off(sleepEM2);
			nextState = SENSOR_IO;
//		}
		break;
	case SENSOR_IO:
		UARTDRV_Receive(gnssHandle0, leuartbuffer, 66, LEUART_rx_callback);	// start non blocking (LDMA) Rx
//		if((evt->data.evt_system_external_signal.extsignals) == DELAY_GENERATED){
			sleep_block_on(sleepEM2);
			measure_navigation(&GNRMC_data);	// send GNRMC_data
			BME_data.temperature = getTemperature();
			measure_temperature(&BME_data);
			BME_data.pressure = getPressure();
			measure_pressure(&BME_data);
			sleep_block_off(sleepEM2);
			nextState = POWER_OFF;
//		}
		break;
	case POWER_OFF:
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



