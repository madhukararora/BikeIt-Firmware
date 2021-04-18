/*
 * menu.h
 *
 *  Created on: Apr 18, 2021
 *      Author: Dhruva
 */

#ifndef SRC_MENU_H_
#define SRC_MENU_H_

#include "scheduler.h"
#include "display.h"
#include "gpio.h"
#include "ble.h"

void displayMenu(uint8_t menupage);

void displayGNSS(GNSS_data_t *data);

void displayTempPres(BME_data_t *data);


#endif /* SRC_MENU_H_ */
