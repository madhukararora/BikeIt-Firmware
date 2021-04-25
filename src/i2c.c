/*
 * @file   : i2c.c
 * @Author : Madhukar Arora
 * @brief  : contains function definitions for using the i2c peripheral
 * Leveraged Professor's instruction on performing repeated reads and writes.
 *
 */

#include "i2c.h"

/* Retarget serial headers */
#include "retargetserial.h"
#include <stdio.h>

#ifdef ENABLE_LOGGING
#define log(...) printf(__VA_ARGS__)
#else
#define log(...)
#endif


/*
 *  Initialize the I2C0 for BME
 *  SCL : PC10#14
 *  SDA : PC11#16
 */
void I2C0_Init(void){

	I2CSPM_Init_TypeDef i2c_Config = {
			.port = I2C0,
			.sclPort = gpioPortC,
			.sclPin = 10,
			.sdaPort = gpioPortC,
			.sdaPin = 11,
			.portLocationScl = 14,
			.portLocationSda = 16,
			.i2cRefFreq = 0,
			.i2cMaxFreq = I2C_FREQ_STANDARD_MAX,
			.i2cClhr = i2cClockHLRStandard
	};
	I2CSPM_Init(&i2c_Config);
}

void I2C0_Write(uint8_t slaveAddr,uint8_t *data, uint16_t len)
{


	uint8_t dataBuf[len];
	//copy data into a local buffer
	for(int i = 0; i < len; i++)
	{
		dataBuf[i] = data[i];
	}
	transferSequence.addr = slaveAddr << 1;
	transferSequence.flags = I2C_FLAG_WRITE;
	transferSequence.buf[0].data = (uint8_t*)&dataBuf;
	transferSequence.buf[0].len = len;

	NVIC_EnableIRQ(I2C0_IRQn);
	I2C_TransferInit(I2C0,&transferSequence);
}

void I2C0_Read(uint8_t slaveAddr,uint8_t *data,uint16_t len)
{
	transferSequence.addr = slaveAddr << 1;
	transferSequence.flags = I2C_FLAG_READ;
	transferSequence.buf[0].data = data;
	transferSequence.buf[0].len = len;

	NVIC_EnableIRQ(I2C0_IRQn);
	I2C_TransferInit(I2C0,&transferSequence);

}

/*
 *  Initialize the I2C0 for BNO
 *  SCL : PD11#18
 *  SDA : PD10#18
 */
void I2C0_Init_BNO(void){

	I2CSPM_Init_TypeDef i2c_Config = {
			.port = I2C0,
			.sclPort = gpioPortD,
			.sclPin = 11,
			.sdaPort = gpioPortD,
			.sdaPin = 10,
			.portLocationScl = 18,
			.portLocationSda = 18,
			.i2cRefFreq = 0,
			.i2cMaxFreq = I2C_FREQ_STANDARD_MAX,
			.i2cClhr = i2cClockHLRStandard
	};
	I2CSPM_Init(&i2c_Config);
}







