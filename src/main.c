/*
 * Edited By : Madhukar Arora
 * Organization : University of Colorado Boulder
 *  main.c
 *
 *  Reference : Utilized Silicon Labs' EMLIB peripheral libraries to implement functionality.
 *
 * NOTE : To change the following conditional directives
 *        LOW_ENERGY_MODE      : value can be 0,1,2,3
 *        ENABLE_SLEEPING      : value can be 0 or 1.
 *        LETIMER_TOTAL_PERIOD : default value is 3000ms (supports up to 7s)
 *        please modify them in main.h source file.
 */





#include "gecko_configuration.h"
#include "gpio.h"
#include "native_gecko.h"
#include "oscillators.h"
#include "timers.h"
#include "main.h"
#include "log.h"
#include "irq.h"
#include "sleep.h"
#include "i2c.h"
#include "scheduler.h"
#include "ble.h"


int appMain(gecko_configuration_t *config)
{
	struct gecko_cmd_packet* evt;
	//Sleep Functionality
	SLEEP_Init_t sleepConfig = {0};
	SLEEP_InitEx(&sleepConfig);


	// Initialize stack
	gecko_init(config);
	logInit(); //logging

	scheduler_Init();

	//Modification : Adding the function call gpioInit()
	gpioInit();
	oscillatorInit();
	//letimer0_Init();

	SLEEP_SleepBlockBegin(sleepEM3);
	while(1){
		if(!gecko_event_pending()){
			logFlush();
		}
		evt = gecko_wait_event();
		ble_EventHandler(evt);
		process_event(evt);
	}
}
