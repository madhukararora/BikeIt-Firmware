/*
 * bme280.c
 *
 *  Created on: Apr 13, 2021
 *      Author: madhu
 */


#include "i2c.h"
#include <stdint.h>
#include "bme280.h"
#include "display.h"
/* Retarget serial headers */
#include "retargetserial.h"
#include <stdio.h>

#ifdef ENABLE_LOGGING
#define log(...) printf(__VA_ARGS__)
#else
#define log(...)
#endif




//initialize the BME280
void bmeInit(void)
{
	int8_t rslt;

	/* chip id read try count */
	uint8_t try_count = 5;
	uint8_t chip_id = 0;

	while(try_count)
	{
		LOG_INFO("trying to read data\n");
		displayPrintf(DISPLAY_ROW_NAME,"finding bme280");
		I2C0_ReadRegister(BME280_CHIP_ID_ADDR,&chip_id);
		//i2cWriteRead(BME280_CHIP_ID_ADDR,BME280_CHIP_ID_ADDR,&chip_id);
		log("chip ID read from BME280 : %d",chip_id);
		if(chip_id == BME280_CHIP_ID)
		{
			displayPrintf(DISPLAY_ROW_NAME,"found bme280");
			log("chip_id read successfully\n");
			break;
		}
		try_count--;
	}
}
