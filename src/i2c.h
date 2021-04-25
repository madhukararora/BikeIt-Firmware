/*
 * @file   : i2c.h
 * @Author : Madhukar Arora
 * @brief  : contains function prototypes for using the i2c peripheral
 *
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

/* Includes */
#include "em_i2c.h"
#include "em_cmu.h"
#include "i2cspm.h"
#include "gpio.h"
#include "em_core.h"
#include "bme280.h"

I2C_TransferSeq_TypeDef transferSequence;
/* Function Prototypes */

void I2C0_Init(void);
void I2C0_Write(uint8_t slaveAddr,uint8_t *data, uint16_t len);
void I2C0_Read(uint8_t slaveAddr,uint8_t *data,uint16_t len);





















/*
 * @function : i2cWrite
 * @brief    : write a byte of data on the I2C peripheral Data line
 * @param    :
 *           uint8_t slaveAddr - address of slave to write data to
 *           uint8_t  reg - register on device where data is to be written
 *           uint8_t  data  - data to be written to the register
 * @return   : void
 */
I2C_TransferReturn_TypeDef  i2cWrite(uint8_t slaveAddr,uint8_t reg, uint8_t data);

/*
 * @function : i2cRead
 * @brief    : read a byte of data on the I2C peripheral Data line
 * @param    :
 *           uint8_t slaveAddr - address of slave to read data from
 *           uint8_t*  writeData - register from which data to be read
 *           uint8_t*  readData  - read data to be stored
 * @return   : void
 */
void  i2cWriteRead(uint8_t slaveAddr, uint8_t writeData, uint8_t* readData);

void  i2cReadDataBlock(uint8_t slaveAddr, uint8_t reg,uint8_t *val, unsigned int len);

#endif /* SRC_I2C_H_ */
