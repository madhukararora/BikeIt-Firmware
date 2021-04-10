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

void gpioInit();
uint8_t get_leds(void);
void set_leds(uint8_t control_byte);
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioLed1SetOn();
void gpioLed1SetOff();
void si7021_enable();
void si7021_disable();
void sda_disable();
void scl_disable();
#endif /* SRC_GPIO_H_ */
