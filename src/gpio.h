/*
 * gpio.h
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include <stdbool.h>


/*Display enabled Macros*/
#define GPIO_SET_DISPLAY_EXT_COMIN_IMPLEMENTED 	1
#define GPIO_DISPLAY_SUPPORT_IMPLEMENTED		1

void gpioInit(void);
void gpioLed0SetOn(void);
void gpioLed0SetOff(void);
void gpioLed1SetOn(void);
void gpioLed1SetOff(void);
void si7021_enable(void);
void si7021_disable(void);
void sda_disable(void);
void scl_disable(void);

void gpioEnableDisplay(void);
void gpioSetDisplayExtcomin(bool high);

#endif /* SRC_GPIO_H_ */
