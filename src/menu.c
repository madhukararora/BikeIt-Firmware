/*
 * menu.c
 *
 *  Created on: Apr 18, 2021
 *      Author: Dhruva
 */
#include "menu.h"

#define PAGE1 0
#define PAGE2 1

extern GNSS_data_t GNRMC_data;
extern BME_data_t BME_data;

void displayMenu(uint8_t menupage){
	displayPrintf(DISPLAY_ROW_TEMPVALUE,"LOC PB0|PB1 COND");
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
	displayPrintf(DISPLAY_ROW_CONNECTION,"Lat:%0.2f", data->flat);
	displayPrintf(DISPLAY_ROW_PASSKEY,"Lon:%0.2f", data->flon);
	displayPrintf(DISPLAY_ROW_ACTION,"Spd:%d", data->gspd);
}

void displayTempPres(BME_data_t *data){
	displayPrintf(DISPLAY_ROW_BTADDR,"Temp/Baro Menu 2");
	displayPrintf(DISPLAY_ROW_BTADDR2,"");
	displayPrintf(DISPLAY_ROW_CONNECTION,"Temp:%0.3f C", data->temperature);
	displayPrintf(DISPLAY_ROW_PASSKEY,"Pressure:%d Pa", data->pressure);
	displayPrintf(DISPLAY_ROW_ACTION,"");
}

