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
#include "display.h"
#include "leuart.h"

int appMain(gecko_configuration_t *config)
{
	struct gecko_cmd_packet* evt;

	/* Sleep Functionality */
	SLEEP_Init_t sleepConfig = {0};
	SLEEP_InitEx(&sleepConfig);

	/* Initialize stack */
	gecko_init(config);

	/*Enable scheduler, GPIO and Oscillator */
	scheduler_Init();
	gpioInit();
	oscillatorInit();

	gpioLedDbgSetOn();
	gpioLedDbgSetOff();

	/*Initialize Display*/
	displayInit();
	displayPrintf(DISPLAY_ROW_NAME,"BikeIt On");
	letimer0_Init();

	// must initialize IMU first as it shares I2C0 on different pins
	// initialize and config IMU, enable interrupts, reset and disable I2C
//	I2C0_Init_BNO();
//	BNO055_Init();
//	BNO055EnableAnyMotion(100, 1);
//	BNO055EnableIntOnXYZ(1, 1, 1);
//	bnoEnableInterrupts();
//	I2C_Reset(I2C0);
//	I2C_Enable(I2C0,false);
//	CMU_ClockEnable(cmuClock_I2C0,false);

	I2C0_Init();
	BME280_Init();

	initLEUART();
	gpioGpsToggleSetOn();

	while(1){
		if(!gecko_event_pending()){
			logFlush();
		}
		evt = gecko_wait_event();
		ble_EventHandler(evt);
		process_event(evt);
	}
}
