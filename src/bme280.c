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
#include "timers.h"
/* Retarget serial headers */
#include "retargetserial.h"
#include <stdio.h>
#include "math.h"

#ifdef ENABLE_LOGGING
#define log(...) printf(__VA_ARGS__)
#else
#define log(...)
#endif

static void delayApproxOneSecond(void)
{
	/**
	 * Wait loops are a bad idea in general!  Don't copy this code in future assignments!
	 * We'll discuss how to do this a better way in the next assignment.
	 */
	volatile int i;
	for (i = 0; i < 3500000; ) {
		i=i+1;
	}
}
uint8_t BME280Read8(uint8_t reg)
{
	uint8_t data;
	I2C0_Write(BME280_ADDRESS,&reg,sizeof(uint8_t));
	while(BME_TRANSFER_DONE == false);
	BME_TRANSFER_DONE = false;
	delayApproxOneSecond();

	I2C0_Read(BME280_ADDRESS,&data,sizeof(uint8_t));
	while(BME_TRANSFER_DONE == false);
	BME_TRANSFER_DONE = false;
	delayApproxOneSecond();
	return data;
}

uint16_t BME280Read16(uint8_t reg)
{
	uint8_t data[2];//first byte is MSB, second byte is LSB
	I2C0_Write(BME280_ADDRESS,&reg,sizeof(uint8_t));
	while(BME_TRANSFER_DONE == false);
	BME_TRANSFER_DONE = false;
	delayApproxOneSecond();

	I2C0_Read(BME280_ADDRESS,data,sizeof(uint16_t));
	while(BME_TRANSFER_DONE == false);
	BME_TRANSFER_DONE = false;
	delayApproxOneSecond();
	return (uint16_t)data[0] << 8 | data[1];

}

uint32_t BME280Read24(uint8_t reg)
{
	uint32_t data = 0;
	uint8_t dat0, dat1,dat2;
	I2C0_Write(BME280_ADDRESS,&reg,sizeof(uint8_t));
	while(BME_TRANSFER_DONE == false);
	BME_TRANSFER_DONE = false;
	delayApproxOneSecond();

	I2C0_Read(BME280_ADDRESS,&dat0,sizeof(uint8_t));
	while(BME_TRANSFER_DONE == false);
	BME_TRANSFER_DONE = false;
	delayApproxOneSecond();
	data = dat0;
	data = data << 8;

	I2C0_Read(BME280_ADDRESS,&dat1,sizeof(uint8_t));
	while(BME_TRANSFER_DONE == false);
	BME_TRANSFER_DONE = false;
	delayApproxOneSecond();
	data |= dat1;
	data = data << 8;

	I2C0_Read(BME280_ADDRESS,&dat2,sizeof(uint8_t));
	while(BME_TRANSFER_DONE == false);
	BME_TRANSFER_DONE = false;
	delayApproxOneSecond();

	data |= dat2;

	return data;

}


uint16_t BME280Read16LE(uint8_t reg)
{
	uint16_t data = BME280Read16(reg);
	return (data >> 8)| (data <<8);
}


int16_t BME280ReadS16(uint8_t reg) {
	return (int16_t)BME280Read16(reg);
}

int16_t BME280ReadS16LE(uint8_t reg) {
	return (int16_t)BME280Read16LE(reg);
}


void BME280_WriteRegister(uint8_t reg,uint8_t val)
{
	uint8_t data[2] = {reg,val};
	I2C0_Write(BME280_ADDRESS,data,sizeof(uint16_t));
	while(BME_TRANSFER_DONE == false);
	BME_TRANSFER_DONE = false;
	delayApproxOneSecond();

}
//initialize the BME280
bool BME280_Init(void)
{
	/* chip id read try count */
	uint8_t try_count = 5;
	uint8_t chip_id = 0;

	while ((try_count--) && (chip_id != 0x60)) {
		chip_id = BME280Read8(BME280_REG_CHIPID);
		//add display prints to debug
	}
	if(chip_id != 0x60)
	{
		//debug to display
		displayPrintf(DISPLAY_ROW_NAME+1,"CHIP ID %x",chip_id);
		return false;
	}
	displayPrintf(DISPLAY_ROW_NAME+1,"CHIP ID %x",chip_id);
	dig_T1 = BME280Read16LE(BME280_REG_DIG_T1);
	dig_T2 = BME280Read16LE(BME280_REG_DIG_T2);
	dig_T3 = BME280Read16LE(BME280_REG_DIG_T3);

	dig_P1 = BME280Read16LE(BME280_REG_DIG_P1);
	dig_P2 = BME280ReadS16LE(BME280_REG_DIG_P2);
	dig_P3 = BME280ReadS16LE(BME280_REG_DIG_P3);
	dig_P4 = BME280ReadS16LE(BME280_REG_DIG_P4);
	dig_P5 = BME280ReadS16LE(BME280_REG_DIG_P5);
	dig_P6 = BME280ReadS16LE(BME280_REG_DIG_P6);
	dig_P7 = BME280ReadS16LE(BME280_REG_DIG_P7);
	dig_P8 = BME280ReadS16LE(BME280_REG_DIG_P8);
	dig_P9 = BME280ReadS16LE(BME280_REG_DIG_P9);

	dig_H1 = BME280Read8(BME280_REG_DIG_H1);
	dig_H2 = BME280Read16LE(BME280_REG_DIG_H2);
	dig_H3 = BME280Read8(BME280_REG_DIG_H3);
	dig_H4 = (BME280Read8(BME280_REG_DIG_H4) << 4) | (0x0F & BME280Read8(BME280_REG_DIG_H4 + 1));
	dig_H5 = (BME280Read8(BME280_REG_DIG_H5 + 1) << 4) | (0x0F & BME280Read8(BME280_REG_DIG_H5) >> 4);
	dig_H6 = (int8_t)BME280Read8(BME280_REG_DIG_H6);

	BME280_WriteRegister(BME280_REG_CONTROLHUMID, 0x05);  //Choose 16X oversampling
	BME280_WriteRegister(BME280_REG_CONTROL, 0xB7);  //Choose 16X oversampling
	return true;

}


float getTemperature(void)
{
	int32_t var1, var2;
	int32_t adc_T = BME280Read24(BME280_REG_TEMPDATA);
	adc_T >>= 4;
	var1 = (((adc_T >> 3) - ((int32_t)(dig_T1 << 1))) *
			((int32_t)dig_T2)) >> 11;

	var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) *
			((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) *
			((int32_t)dig_T3)) >> 14;

	t_fine = var1 + var2;
	float T = (t_fine * 5 + 128) >> 8;
	return T / 100;

}

uint32_t getPressure(void)
{
	int64_t var1, var2, p;

	// Call getTemperature to get t_fine
	getTemperature();
	int32_t adc_P = BME280Read24(BME280_REG_PRESSUREDATA);
	adc_P >>= 4;

	var1 = ((int64_t)t_fine) - 128000;
	var2 = var1 * var1 * (int64_t)dig_P6;
	var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
	var2 = var2 + (((int64_t)dig_P4) << 35);
	var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
	var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
	if (var1 == 0) {
		return 0; // avoid exception caused by division by zero
	}
	p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((int64_t)dig_P8) * p) >> 19;
	p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
	return (uint32_t)p / 256;

}

float calcAltitude(float pressure) {

	float A = pressure / 101325;
	float B = 1 / 5.25588;
	float C = pow(A, B);
	C = 1.0 - C;
	C = C / 0.0000225577;
	return C;
}
