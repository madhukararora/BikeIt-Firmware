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
 *  Initialize the I2C0
 *  SCL : PC10#14
 *  SDA : PC11#16
 */
void I2C0_init(void){

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

void I2C0_WriteRegister(uint8_t regAddr, uint8_t data){

	I2C_TransferSeq_TypeDef transaction_config;
	transaction_config.addr = BME280_I2C_ADDR_PRIM;
	transaction_config.flags = I2C_FLAG_WRITE_WRITE;
	transaction_config.buf[0].data = regAddr;
	transaction_config.buf[0].len = 1;
	transaction_config.buf[1].data = data;
	transaction_config.buf[1].len = 1;

	NVIC_EnableIRQ(I2C0_IRQn);

	I2C_TransferInit(I2C0,&transaction_config);

}

void I2C0_ReadRegister(uint8_t regAddr, uint8_t *data){

	I2C_TransferSeq_TypeDef transaction_config;
	transaction_config.addr = BME280_I2C_ADDR_PRIM;
	transaction_config.flags = I2C_FLAG_WRITE_READ;
	transaction_config.buf[0].data = (uint8_t*)&regAddr;
	transaction_config.buf[0].len = 1;
	transaction_config.buf[1].data = data;
	transaction_config.buf[1].len = 1;

	NVIC_EnableIRQ(I2C0_IRQn);

	I2C_TransferInit(I2C0,&transaction_config);

}





















/*
 * write a byte of data to a register on the device */

I2C_TransferReturn_TypeDef  i2cWrite(uint8_t slaveAddr,uint8_t reg, uint8_t data){

	I2C_TransferSeq_TypeDef    transferSequence;
	uint8_t cmd_data[2];
	cmd_data[0] = reg;
	cmd_data[1] = data;

	transferSequence.addr = slaveAddr << 1;
	transferSequence.flags = I2C_FLAG_WRITE;
	transferSequence.buf[0].data = (uint8_t*)&cmd_data;
	transferSequence.buf[0].len = 2;
	transferSequence.buf[1].data = 0;
	transferSequence.buf[1].len = 0;


	return I2CSPM_Transfer(I2C0, &transferSequence);
}



/*write and read a byte of data*/
void  i2cWriteRead(uint8_t slaveAddr, uint8_t writeData, uint8_t* readData){

	I2C_TransferSeq_TypeDef        transferSequence;
	transferSequence.addr = slaveAddr;
	transferSequence.flags = I2C_FLAG_WRITE_READ; //perform read followed by a write
	transferSequence.buf[0].data = (uint8_t*)&writeData;
	transferSequence.buf[0].len = 1;
	transferSequence.buf[1].data = readData;
	transferSequence.buf[1].len  = 1;

	I2C_TransferInit(I2C0,&transferSequence);
}

void  i2cReadDataBlock(uint8_t slaveAddr, uint8_t reg,uint8_t *val, unsigned int len){

	I2C_TransferSeq_TypeDef    transferSequence;
	transferSequence.addr = slaveAddr;
	transferSequence.flags = I2C_FLAG_WRITE_READ;
	transferSequence.buf[0].data = (uint8_t*)&reg;
	transferSequence.buf[0].len = 1;
	transferSequence.buf[1].data = val;
	transferSequence.buf[1].len  = len;
	//return I2CSPM_Transfer(I2C0,&transferSequence);


}
