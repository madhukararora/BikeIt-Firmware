/*
 * gpio.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include <stdbool.h>
#include <stdint.h>
#include "scheduler.h"

/*Display enabled Macros*/
#define GPIO_SET_DISPLAY_EXT_COMIN_IMPLEMENTED 	1
#define GPIO_DISPLAY_SUPPORT_IMPLEMENTED		1

#define BOARD 0
#define DEVKIT 1

void gpioInit(void);
void enable_button_interrupts(void);
void button_interrupt(uint8_t pin);
uint8_t get_leds(void);
void set_leds(uint8_t control_byte);
#if DEVKIT
void gpioLed0SetOn(void);
void gpioLed0SetOff(void);
void gpioLed1SetOn(void);
void gpioLed1SetOff(void);

void si7021_enable(void);
void si7021_disable(void);
void sda_disable(void);
void scl_disable(void);
void bnoSDADisable(void);
void bnoSCLDisable(void);
void bnoInterrupt(uint8_t pin);
void bnoEnableInterrupts(void);
void gpioGpsToggleSetOn(void);
void gpioGpsToggleSetOff(void);
#endif
#if BOARD
void gpioLedDbgSetOn(void);
void gpioLedDbgSetOff(void);
void gpioPmuxD1SetOn(void);
void gpioPmuxD1SetOff(void);
void gpioGpsResetSetOn(void);
void gpioGpsResetSetOff(void);
void gpioGpsExtIntSetOn(void);
void gpioGpsExtIntSetOff(void);
void gpioImuResetSetOn(void);
void gpioImuResetSetOff(void);
void gpioGpsExtIntRead(void);
void gpioSetDisplayExtmode(bool high);
void bmeSDADisable(void);
void bmeSCLDisable(void);
#endif

void gpioEnableDisplay(void);
void gpioSetDisplayExtcomin(bool high);

#endif /* SRC_GPIO_H_ */
