/*
 * menu.c
 *
 *  Created on: Apr 18, 2021
 *      Author: Dhruva
 */
#include "menu.h"

#define PAGE1 1
#define PAGE2 2

extern GNSS_data_t GNRMC_data;
extern BME_data_t BME_data;

void displayMenu(uint8_t menupage){
	displayPrintf(DISPLAY_ROW_TEMPVALUE,"<- PB0 | PB1 ->");
	switch(menupage){
	case PAGE1:
		displayGNSS(&GNRMC_data);
		break;
	case PAGE2:
		displayTempPres(&BME_data);
		break;
	default:
		break;
	}
}

void displayGNSS(GNSS_data_t *data){
	displayPrintf(DISPLAY_ROW_BTADDR,"GNSS Menu 1");
	displayPrintf(DISPLAY_ROW_BTADDR2,"UTC:%s", data->utctime);
	displayPrintf(DISPLAY_ROW_CONNECTION,"Lat:%s", data->latitude);
	displayPrintf(DISPLAY_ROW_PASSKEY,"Lon:%s", data->longitude);
	displayPrintf(DISPLAY_ROW_ACTION,"Spd:%s", data->gspeed);
}

void displayTempPres(BME_data_t *data){
	displayPrintf(DISPLAY_ROW_BTADDR,"Temp/Baro Menu 2");
	displayPrintf(DISPLAY_ROW_BTADDR2,"");
	displayPrintf(DISPLAY_ROW_CONNECTION,"Temp:%0.3f C", data->temperature);
	displayPrintf(DISPLAY_ROW_PASSKEY,"Pressure:%0.1f Pa", data->pressure);
	displayPrintf(DISPLAY_ROW_ACTION,"");
}

