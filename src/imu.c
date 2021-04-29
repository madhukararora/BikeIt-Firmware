/*
 * imu.c
 *
 *  Created on: Apr 19, 2021
 *      Author: Dhruva
 */

#ifndef SRC_IMU_C_
#define SRC_IMU_C_

#include "imu.h"

bno055_t bno;
bno055_opmode_t _mode;
bno055_page_t _page;

bool BNO055_Init(void){
	BNO_TRANSFER_DONE = false;
	/* chip id read try count */
	uint8_t try_count = 5;
	bno.dev_addr = BNO055_I2C_ADDR1;

	while ((try_count--) && (bno.chip_id != 0xA0)){
		bno.chip_id = BNO055Read8(BNO055_CHIP_ID_ADDR);
		//add display prints to debug
	}
	if(bno.chip_id != 0xA0){
		//debug to display
		return false;
	}

	BNO055SetMode(OPERATION_MODE_CONFIG);
	//delay 50
	BNO055_WriteRegister(BNO055_PWR_MODE_ADDR, POWER_MODE_LOWPOWER);
	//delay 10

	BNO055SetPage(PAGE_0);

	// output units
	uint8_t unitsel = (0 << 7) | // Orientation = Android
					(0 << 4) | // Temperature = Celsius
					(0 << 2) | // Euler = Degrees
					(1 << 1) | // Gyro = Rads
					(0 << 0);  // Accelerometer = m/s^2
	BNO055_WriteRegister(BNO055_UNIT_SEL_ADDR, unitsel);

	// axis mapping
	BNO055_WriteRegister(BNO055_AXIS_MAP_CONFIG_ADDR, BNO055_REMAP_CONFIG_P0); // P0-P7, Default is P1
//	delay(10);
	BNO055_WriteRegister(BNO055_AXIS_MAP_SIGN_ADDR, BNO055_REMAP_SIGN_P0); // P0-P7, Default is P1
//	delay(10);

	BNO055_WriteRegister(BNO055_SYS_TRIGGER_ADDR, 0x0);
	//delay 10
	BNO055SetMode(OPERATION_MODE_ACCONLY);

	return true;
}

void BNO055ResetInt(void){
    // Read to get the current settings
    int8_t sysTrigger = (int8_t)(BNO055Read8(BNO055_SYS_TRIGGER_ADDR));

    // Set the flags as requested
    sysTrigger = BNO055SliceValueIntoRegister(0x01, sysTrigger, BNO055_SYS_TRIGGER_ADDR_RST_INT_MSK, BNO055_SYS_TRIGGER_ADDR_RST_INT_POS);

    // Write back the entire register
    BNO055_WriteRegister(BNO055_SYS_TRIGGER_ADDR, sysTrigger);
//    delay(30);
}

void BNO055EnableIntOnXYZ(uint8_t x, uint8_t y, uint8_t z){
    // Need to be on page 0 to get into config mode
    bno055_page_t lastPage = _page;
    if (lastPage != PAGE_0) BNO055SetPage(PAGE_0);

    // Must be in config mode, so force it
    bno055_opmode_t lastMode = _mode;
    BNO055SetMode(OPERATION_MODE_CONFIG);

    // Change to page 1 for interrupt settings
    BNO055SetPage(PAGE_1);

    // Read to get the current settings
    int8_t intSettings = (int8_t)(BNO055Read8(BNO055_ACCEL_INTR_SETTINGS_ADDR));

    // Set the flags as requested--binary choice, so set or unset
    intSettings = BNO055SliceValueIntoRegister(x ? 0x01 : 0x00, intSettings, BNO055_ACC_INT_Settings_ACC_X_MSK, BNO055_ACC_INT_Settings_ACC_X_POS);
    intSettings = BNO055SliceValueIntoRegister(y ? 0x01 : 0x00, intSettings, BNO055_ACC_INT_Settings_ACC_Y_MSK, BNO055_ACC_INT_Settings_ACC_Y_POS);
    intSettings = BNO055SliceValueIntoRegister(z ? 0x01 : 0x00, intSettings, BNO055_ACC_INT_Settings_ACC_Z_MSK, BNO055_ACC_INT_Settings_ACC_Z_POS);

    // Write back the entire register
    BNO055_WriteRegister(BNO055_ACCEL_INTR_SETTINGS_ADDR, intSettings);
//    delay(30);

    // Return the mode to the last mode
    BNO055SetPage(PAGE_0);
    BNO055SetMode(lastMode);

    // Change the page back to whichever it was initially
    if (lastPage != PAGE_0) BNO055SetPage(lastPage);
}

void BNO055EnableAnyMotion(uint8_t threshold, uint8_t duration){
    // Need to be on page 0 to get into config mode
    bno055_page_t lastPage = _page;
    if (lastPage != PAGE_0) BNO055SetPage(PAGE_0);

    // Must be in config mode, so force it
    bno055_opmode_t lastMode = _mode;
    BNO055SetMode(OPERATION_MODE_CONFIG);

    // Change to page 1 for interrupt settings
    BNO055SetPage(PAGE_1);

    // Set duration (bits 1-6)
    int8_t intSettings = (int8_t)(BNO055Read8(BNO055_ACCEL_INTR_SETTINGS_ADDR));
    intSettings = BNO055SliceValueIntoRegister(duration, intSettings, BNO055_ACC_INT_Settings_AM_DUR_MSK, BNO055_ACC_INT_Settings_AM_DUR_POS);
    BNO055_WriteRegister(BNO055_ACCEL_INTR_SETTINGS_ADDR, intSettings);

    // Set the threshold (full byte)
    int8_t threshSettings = (int8_t)(BNO055Read8(BNO055_ACCEL_ANY_MOTION_THRES_ADDR));
    threshSettings = BNO055SliceValueIntoRegister(threshold, threshSettings, BNO055_ACC_AM_THRES_MSK, BNO055_ACC_AM_THRES_POS);
    BNO055_WriteRegister(BNO055_ACCEL_ANY_MOTION_THRES_ADDR, threshSettings);

    // Enable the interrupt
    BNO055SetInterruptEnableAccelAM(1);

    // Fire on the pin
    BNO055SetInterruptMaskAccelAM(1);

//    delay(30);

    // Return the mode to the last mode
    BNO055SetPage(PAGE_0);
    BNO055SetMode(lastMode);

    // Change the page back to whichever it was initially
    if (lastPage != PAGE_0) BNO055SetPage(lastPage);
}

void BNO055DisableAnyMotion(void){
    // Need to be on page 0 to get into config mode
    bno055_page_t lastPage = _page;
    if (lastPage != PAGE_0) BNO055SetPage(PAGE_0);

    // Must be in config mode, so force it
    bno055_opmode_t lastMode = _mode;
    BNO055SetMode(OPERATION_MODE_CONFIG);

    // Change to page 1 for interrupt settings
    BNO055SetPage(PAGE_1);

    // Disable the interrupt
    BNO055SetInterruptEnableAccelAM(0);

    // Stop firing on the pin
    BNO055SetInterruptMaskAccelAM(0);

//    delay(30);

    // Return the mode to the last mode
    BNO055SetPage(PAGE_0);
    BNO055SetMode(lastMode);

    // Change the page back to whichever it was initially
    if (lastPage != PAGE_0) BNO055SetPage(lastPage);
}

/**************************************************************************/
/*!
 @brief  Enable or disable the any motion interrupt entirely
 */
/**************************************************************************/
// WARNING: ONLY CALL INSIDE PAGE/CONFIG WRAPPER
void BNO055SetInterruptEnableAccelAM(uint8_t enable){
    int8_t intEnable = (int8_t)(BNO055Read8(BNO055_INT_ADDR));
    intEnable = BNO055SliceValueIntoRegister(enable, intEnable, BNO055_INT_EN_ACC_AM_MSK, BNO055_INT_EN_ACC_AM_POS);
    BNO055_WriteRegister(BNO055_INT_ADDR, intEnable);
}

/**************************************************************************/
/*!
 @brief  Tell any motion interrupt to set pin and output or just set the output
 */
/**************************************************************************/
// WARNING: ONLY CALL INSIDE PAGE/CONFIG WRAPPER
void BNO055SetInterruptMaskAccelAM(uint8_t enable){
    int8_t intMask = (int8_t)(BNO055Read8(BNO055_INT_ADDR));
    intMask = BNO055SliceValueIntoRegister(enable, intMask, BNO055_INT_MSK_ACC_AM_MSK, BNO055_INT_MSK_ACC_AM_POS);
    BNO055_WriteRegister(BNO055_INT_ADDR, intMask);
}

void BNO055SetMode(bno055_powermode_t mode){
	_mode = mode;
	BNO055_WriteRegister(BNO055_OPR_MODE_ADDR, _mode);
	//delay 30
}

void BNO055SetPage(bno055_page_t page){
	_page = page;
	BNO055_WriteRegister(BNO055_PAGE_ID_ADDR, _page);
	//delay 30
}

uint8_t BNO055Read8(uint8_t reg){
	uint8_t data;
	I2C0_Write(BNO055_I2C_ADDR1, &reg,sizeof(uint8_t));
	while(BNO_TRANSFER_DONE == false);
	BNO_TRANSFER_DONE = false;

	I2C0_Read(BNO055_I2C_ADDR1, &data, sizeof(uint8_t));
	while(BNO_TRANSFER_DONE == false);
	BNO_TRANSFER_DONE = false;

	return data;
}

uint16_t BNO055Read16(uint8_t reg){
	uint8_t data[2];//first byte is MSB, second byte is LSB

	I2C0_Write(BNO055_I2C_ADDR1, &reg, sizeof(uint8_t));
	while(BNO_TRANSFER_DONE == false);
	BNO_TRANSFER_DONE = false;

	I2C0_Read(BNO055_I2C_ADDR1, data, sizeof(uint16_t));
	while(BNO_TRANSFER_DONE == false);
	BNO_TRANSFER_DONE = false;

	return (uint16_t)data[0] << 8 | data[1];
}

uint32_t BNO055Read24(uint8_t reg){
	uint32_t data;
	uint8_t dat0, dat1, dat2;

	I2C0_Write(BNO055_I2C_ADDR1, &reg, sizeof(uint8_t));
	while(BNO_TRANSFER_DONE == false);
	BNO_TRANSFER_DONE = false;

	I2C0_Read(BNO055_I2C_ADDR1, &dat0, sizeof(uint8_t));
	while(BNO_TRANSFER_DONE == false);
	BNO_TRANSFER_DONE = false;

	data = dat0;
	data = data << 8;

	I2C0_Read(BNO055_I2C_ADDR1, &dat1, sizeof(uint8_t));
	while(BNO_TRANSFER_DONE == false);
	BNO_TRANSFER_DONE = false;

	data |= dat1;
	data = data << 8;

	I2C0_Read(BNO055_I2C_ADDR1, &dat2, sizeof(uint8_t));
	while(BNO_TRANSFER_DONE == false);
	BNO_TRANSFER_DONE = false;

	data |= dat2;

	return data;
}

uint16_t BNO055Read16LE(uint8_t reg){
	uint16_t data = BNO055Read16(reg);
	return (data >> 8)| (data <<8);
}

int16_t BNO055ReadS16(uint8_t reg){
	return (int16_t)BNO055Read16(reg);
}

int16_t BNO055ReadS16LE(uint8_t reg){
	return (int16_t)BNO055Read16LE(reg);
}

void BNO055_WriteRegister(uint8_t reg, uint8_t val){
	uint8_t data[2] = {reg, val};
	I2C0_Write(BNO055_I2C_ADDR1, data, sizeof(uint16_t));
	while(BNO_TRANSFER_DONE == false);
	BNO_TRANSFER_DONE = false;
}

/**************************************************************************/
/*!
 @brief  Sets the value of the partial (or full) register
 */
/**************************************************************************/
uint8_t BNO055SliceValueIntoRegister(uint8_t value, uint8_t reg, uint8_t mask, uint8_t position){
    return (reg & ~mask) | ((value << position) & mask);
}

#endif /* SRC_IMU_C_ */
