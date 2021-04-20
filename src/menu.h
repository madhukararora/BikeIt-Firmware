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

typedef enum{
	PAGE1,
	PAGE2,
	PAGE_MAX,
}menu_states_t;

void displayMenu(uint8_t menupage);

void displayGNSS(GNSS_data_t *data);

void displayTempPres(BME_data_t *data);


#endif /* SRC_MENU_H_ */
