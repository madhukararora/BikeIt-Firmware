/*
 * imu.h
 *
 *  Created on: Apr 19, 2021
 *      Author: Dhruva
 */

#ifndef SRC_IMU_H_
#define SRC_IMU_H_

#include "i2c.h"
#include <stdint.h>
#include "display.h"
#include <stdio.h>
#include "math.h"

/********************************************************/
/**\name    I2C ADDRESS DEFINITION FOR BNO055           */
/********************************************************/
/* bno055 I2C Address */
#define BNO055_I2C_ADDR1 (0x28)
//#define BNO055_I2C_ADDR2 (0x29)
volatile bool BNO_TRANSFER_DONE;

/***************************************************/
/**\name    REGISTER ADDRESS DEFINITION  */
/***************************************************/
/* Page id register definition*/
#define BNO055_PAGE_ID_ADDR                 (0X07)

/* PAGE0 REGISTER DEFINITION START*/
#define BNO055_CHIP_ID_ADDR                 (0x00)
#define BNO055_ACCEL_REV_ID_ADDR            (0x01)
#define BNO055_MAG_REV_ID_ADDR              (0x02)
#define BNO055_GYRO_REV_ID_ADDR             (0x03)
#define BNO055_SW_REV_ID_LSB_ADDR           (0x04)
#define BNO055_SW_REV_ID_MSB_ADDR           (0x05)
#define BNO055_BL_REV_ID_ADDR               (0X06)

/* Accel data register*/
#define BNO055_ACCEL_DATA_X_LSB_ADDR        (0X08)
#define BNO055_ACCEL_DATA_X_MSB_ADDR        (0X09)
#define BNO055_ACCEL_DATA_Y_LSB_ADDR        (0X0A)
#define BNO055_ACCEL_DATA_Y_MSB_ADDR        (0X0B)
#define BNO055_ACCEL_DATA_Z_LSB_ADDR        (0X0C)
#define BNO055_ACCEL_DATA_Z_MSB_ADDR        (0X0D)

/*Mag data register*/
#define BNO055_MAG_DATA_X_LSB_ADDR          (0X0E)
#define BNO055_MAG_DATA_X_MSB_ADDR          (0X0F)
#define BNO055_MAG_DATA_Y_LSB_ADDR          (0X10)
#define BNO055_MAG_DATA_Y_MSB_ADDR          (0X11)
#define BNO055_MAG_DATA_Z_LSB_ADDR          (0X12)
#define BNO055_MAG_DATA_Z_MSB_ADDR          (0X13)

/*Gyro data registers*/
#define BNO055_GYRO_DATA_X_LSB_ADDR         (0X14)
#define BNO055_GYRO_DATA_X_MSB_ADDR         (0X15)
#define BNO055_GYRO_DATA_Y_LSB_ADDR         (0X16)
#define BNO055_GYRO_DATA_Y_MSB_ADDR         (0X17)
#define BNO055_GYRO_DATA_Z_LSB_ADDR         (0X18)
#define BNO055_GYRO_DATA_Z_MSB_ADDR         (0X19)

/*Euler data registers*/
#define BNO055_EULER_H_LSB_ADDR             (0X1A)
#define BNO055_EULER_H_MSB_ADDR             (0X1B)

#define BNO055_EULER_R_LSB_ADDR             (0X1C)
#define BNO055_EULER_R_MSB_ADDR             (0X1D)

#define BNO055_EULER_P_LSB_ADDR             (0X1E)
#define BNO055_EULER_P_MSB_ADDR             (0X1F)

/*Quaternion data registers*/
#define BNO055_QUATERNION_DATA_W_LSB_ADDR   (0X20)
#define BNO055_QUATERNION_DATA_W_MSB_ADDR   (0X21)
#define BNO055_QUATERNION_DATA_X_LSB_ADDR   (0X22)
#define BNO055_QUATERNION_DATA_X_MSB_ADDR   (0X23)
#define BNO055_QUATERNION_DATA_Y_LSB_ADDR   (0X24)
#define BNO055_QUATERNION_DATA_Y_MSB_ADDR   (0X25)
#define BNO055_QUATERNION_DATA_Z_LSB_ADDR   (0X26)
#define BNO055_QUATERNION_DATA_Z_MSB_ADDR   (0X27)

/* Linear acceleration data registers*/
#define BNO055_LINEAR_ACCEL_DATA_X_LSB_ADDR (0X28)
#define BNO055_LINEAR_ACCEL_DATA_X_MSB_ADDR (0X29)
#define BNO055_LINEAR_ACCEL_DATA_Y_LSB_ADDR (0X2A)
#define BNO055_LINEAR_ACCEL_DATA_Y_MSB_ADDR (0X2B)
#define BNO055_LINEAR_ACCEL_DATA_Z_LSB_ADDR (0X2C)
#define BNO055_LINEAR_ACCEL_DATA_Z_MSB_ADDR (0X2D)

/*Gravity data registers*/
#define BNO055_GRAVITY_DATA_X_LSB_ADDR      (0X2E)
#define BNO055_GRAVITY_DATA_X_MSB_ADDR      (0X2F)
#define BNO055_GRAVITY_DATA_Y_LSB_ADDR      (0X30)
#define BNO055_GRAVITY_DATA_Y_MSB_ADDR      (0X31)
#define BNO055_GRAVITY_DATA_Z_LSB_ADDR      (0X32)
#define BNO055_GRAVITY_DATA_Z_MSB_ADDR      (0X33)

/* Temperature data register*/
#define BNO055_TEMP_ADDR                    (0X34)

/* Status registers*/
#define BNO055_CALIB_STAT_ADDR              (0X35)
#define BNO055_SELFTEST_RESULT_ADDR         (0X36)
#define BNO055_INTR_STAT_ADDR               (0X37)
#define BNO055_SYS_CLK_STAT_ADDR            (0X38)
#define BNO055_SYS_STAT_ADDR                (0X39)
#define BNO055_SYS_ERR_ADDR                 (0X3A)

/* Unit selection register*/
#define BNO055_UNIT_SEL_ADDR                (0X3B)
#define BNO055_DATA_SELECT_ADDR             (0X3C)

/* Mode registers*/
#define BNO055_OPR_MODE_ADDR                (0X3D)
#define BNO055_PWR_MODE_ADDR                (0X3E)

#define BNO055_SYS_TRIGGER_ADDR             (0X3F)
#define BNO055_SYS_TRIGGER_ADDR_RST_INT_MSK	(0X40)
#define BNO055_SYS_TRIGGER_ADDR_RST_INT_POS (6)
#define BNO055_TEMP_SOURCE_ADDR             (0X40)

/* Axis remap registers*/
#define BNO055_AXIS_MAP_CONFIG_ADDR         (0X41)
#define BNO055_AXIS_MAP_SIGN_ADDR           (0X42)

/* SIC registers*/
#define BNO055_SIC_MATRIX_0_LSB_ADDR        (0X43)
#define BNO055_SIC_MATRIX_0_MSB_ADDR        (0X44)
#define BNO055_SIC_MATRIX_1_LSB_ADDR        (0X45)
#define BNO055_SIC_MATRIX_1_MSB_ADDR        (0X46)
#define BNO055_SIC_MATRIX_2_LSB_ADDR        (0X47)
#define BNO055_SIC_MATRIX_2_MSB_ADDR        (0X48)
#define BNO055_SIC_MATRIX_3_LSB_ADDR        (0X49)
#define BNO055_SIC_MATRIX_3_MSB_ADDR        (0X4A)
#define BNO055_SIC_MATRIX_4_LSB_ADDR        (0X4B)
#define BNO055_SIC_MATRIX_4_MSB_ADDR        (0X4C)
#define BNO055_SIC_MATRIX_5_LSB_ADDR        (0X4D)
#define BNO055_SIC_MATRIX_5_MSB_ADDR        (0X4E)
#define BNO055_SIC_MATRIX_6_LSB_ADDR        (0X4F)
#define BNO055_SIC_MATRIX_6_MSB_ADDR        (0X50)
#define BNO055_SIC_MATRIX_7_LSB_ADDR        (0X51)
#define BNO055_SIC_MATRIX_7_MSB_ADDR        (0X52)
#define BNO055_SIC_MATRIX_8_LSB_ADDR        (0X53)
#define BNO055_SIC_MATRIX_8_MSB_ADDR        (0X54)

/* Accelerometer Offset registers*/
#define BNO055_ACCEL_OFFSET_X_LSB_ADDR      (0X55)
#define BNO055_ACCEL_OFFSET_X_MSB_ADDR      (0X56)
#define BNO055_ACCEL_OFFSET_Y_LSB_ADDR      (0X57)
#define BNO055_ACCEL_OFFSET_Y_MSB_ADDR      (0X58)
#define BNO055_ACCEL_OFFSET_Z_LSB_ADDR      (0X59)
#define BNO055_ACCEL_OFFSET_Z_MSB_ADDR      (0X5A)

/* Magnetometer Offset registers*/
#define BNO055_MAG_OFFSET_X_LSB_ADDR        (0X5B)
#define BNO055_MAG_OFFSET_X_MSB_ADDR        (0X5C)
#define BNO055_MAG_OFFSET_Y_LSB_ADDR        (0X5D)
#define BNO055_MAG_OFFSET_Y_MSB_ADDR        (0X5E)
#define BNO055_MAG_OFFSET_Z_LSB_ADDR        (0X5F)
#define BNO055_MAG_OFFSET_Z_MSB_ADDR        (0X60)

/* Gyroscope Offset registers*/
#define BNO055_GYRO_OFFSET_X_LSB_ADDR       (0X61)
#define BNO055_GYRO_OFFSET_X_MSB_ADDR       (0X62)
#define BNO055_GYRO_OFFSET_Y_LSB_ADDR       (0X63)
#define BNO055_GYRO_OFFSET_Y_MSB_ADDR       (0X64)
#define BNO055_GYRO_OFFSET_Z_LSB_ADDR       (0X65)
#define BNO055_GYRO_OFFSET_Z_MSB_ADDR       (0X66)

/* Radius registers*/
#define BNO055_ACCEL_RADIUS_LSB_ADDR        (0X67)
#define BNO055_ACCEL_RADIUS_MSB_ADDR        (0X68)
#define BNO055_MAG_RADIUS_LSB_ADDR          (0X69)
#define BNO055_MAG_RADIUS_MSB_ADDR          (0X6A)

/* PAGE0 REGISTERS DEFINITION END*/
/* PAGE1 REGISTERS DEFINITION START*/
/* Configuration registers*/
#define BNO055_ACCEL_CONFIG_ADDR            (0X08)
#define BNO055_MAG_CONFIG_ADDR              (0X09)
#define BNO055_GYRO_CONFIG_ADDR             (0X0A)
#define BNO055_GYRO_MODE_CONFIG_ADDR        (0X0B)
#define BNO055_ACCEL_SLEEP_CONFIG_ADDR      (0X0C)
#define BNO055_GYRO_SLEEP_CONFIG_ADDR       (0X0D)
#define BNO055_MAG_SLEEP_CONFIG_ADDR        (0x0E)

/* Interrupt registers*/
#define BNO055_INT_MASK_ADDR                (0X0F)
#define BNO055_INT_ADDR                     (0X10)
#define BNO055_ACCEL_ANY_MOTION_THRES_ADDR  (0X11)
#define BNO055_ACCEL_INTR_SETTINGS_ADDR     (0X12)
#define BNO055_ACCEL_HIGH_G_DURN_ADDR       (0X13)
#define BNO055_ACCEL_HIGH_G_THRES_ADDR      (0X14)
#define BNO055_ACCEL_NO_MOTION_THRES_ADDR   (0X15)
#define BNO055_ACCEL_NO_MOTION_SET_ADDR     (0X16)
#define BNO055_GYRO_INTR_SETING_ADDR        (0X17)
#define BNO055_GYRO_HIGHRATE_X_SET_ADDR     (0X18)
#define BNO055_GYRO_DURN_X_ADDR             (0X19)
#define BNO055_GYRO_HIGHRATE_Y_SET_ADDR     (0X1A)
#define BNO055_GYRO_DURN_Y_ADDR             (0X1B)
#define BNO055_GYRO_HIGHRATE_Z_SET_ADDR     (0X1C)
#define BNO055_GYRO_DURN_Z_ADDR             (0X1D)
#define BNO055_GYRO_ANY_MOTION_THRES_ADDR   (0X1E)
#define BNO055_GYRO_ANY_MOTION_SET_ADDR     (0X1F)
/* Interrupt mask register */
#define BNO055_INT_MSK_ACC_NM_MSK			(0X80)
#define BNO055_INT_MSK_ACC_NM_POS			(7)
#define BNO055_INT_MSK_ACC_AM_MSK			(0X40)
#define BNO055_INT_MSK_ACC_AM_POS			(6)

/* Interrupt enabled register */
#define BNO055_INT_EN_ACC_NM_MSK			(0X80)
#define BNO055_INT_EN_ACC_NM_POS			(7)
#define BNO055_INT_EN_ACC_AM_MSK			(0X40)
#define BNO055_INT_EN_ACC_AM_POS			(6)

/* Any Motion interrupt threshold register */
#define BNO055_ACC_AM_THRES_MSK				(0XFF)
#define BNO055_ACC_AM_THRES_POS				(0)

/* Acceleration interrupt settings register */
#define BNO055_ACC_INT_Settings_ACC_X_MSK	(0X04)
#define BNO055_ACC_INT_Settings_ACC_X_POS	(2)
#define BNO055_ACC_INT_Settings_ACC_Y_MSK	(0X08)
#define BNO055_ACC_INT_Settings_ACC_Y_POS	(3)
#define BNO055_ACC_INT_Settings_ACC_Z_MSK	(0X10)
#define BNO055_ACC_INT_Settings_ACC_Z_POS	(4)
#define BNO055_ACC_INT_Settings_AM_DUR_MSK	(0x03)
#define BNO055_ACC_INT_Settings_AM_DUR_POS	(0)

#define BNO055_REMAP_CONFIG_P0				(0x21)
#define BNO055_REMAP_CONFIG_P1				(0x24) // default
#define BNO055_REMAP_CONFIG_P2				(0x24)
#define BNO055_REMAP_CONFIG_P3				(0x21)
#define BNO055_REMAP_CONFIG_P4				(0x24)
#define BNO055_REMAP_CONFIG_P5				(0x21)
#define BNO055_REMAP_CONFIG_P6				(0x21)
#define BNO055_REMAP_CONFIG_P7				(0x24)

#define BNO055_REMAP_SIGN_P0				(0x04)
#define BNO055_REMAP_SIGN_P1				(0x00) // default
#define BNO055_REMAP_SIGN_P2				(0x06)
#define BNO055_REMAP_SIGN_P3				(0x02)
#define BNO055_REMAP_SIGN_P4				(0x03)
#define BNO055_REMAP_SIGN_P5				(0x01)
#define BNO055_REMAP_SIGN_P6				(0x07)
#define BNO055_REMAP_SIGN_P7				(0x05)

typedef struct bno055_t
{
    uint8_t chip_id; /**< chip_id of bno055 */
    uint16_t sw_rev_id; /**< software revision id of bno055 */
    uint8_t page_id; /**< page_id of bno055 */
    uint8_t accel_rev_id; /**< accel revision id of bno055 */
    uint8_t mag_rev_id; /**< mag revision id of bno055 */
    uint8_t gyro_rev_id; /**< gyro revision id of bno055 */
    uint8_t bl_rev_id; /**< boot loader revision id of bno055 */
    uint8_t dev_addr; /**< i2c device address of bno055 */
} bno055_t;

typedef enum bno055_opmode_t
{
    /* Operation mode settings*/
    OPERATION_MODE_CONFIG                                   = 0X00,
    OPERATION_MODE_ACCONLY                                  = 0X01,
    OPERATION_MODE_MAGONLY                                  = 0X02,
    OPERATION_MODE_GYRONLY                                  = 0X03,
    OPERATION_MODE_ACCMAG                                   = 0X04,
    OPERATION_MODE_ACCGYRO                                  = 0X05,
    OPERATION_MODE_MAGGYRO                                  = 0X06,
    OPERATION_MODE_AMG                                      = 0X07,
    OPERATION_MODE_IMUPLUS                                  = 0X08,
    OPERATION_MODE_COMPASS                                  = 0X09,
    OPERATION_MODE_M4G                                      = 0X0A,
    OPERATION_MODE_NDOF_FMC_OFF                             = 0X0B,
    OPERATION_MODE_NDOF                                     = 0X0C
} bno055_opmode_t;

typedef enum bno055_page_t
{
    /* Operation mode settings*/
    PAGE_0                                                  = 0X00,
    PAGE_1                                                  = 0X01
} bno055_page_t;

typedef enum bno055_powermode_t
{
    POWER_MODE_NORMAL                                       = 0X00,
    POWER_MODE_LOWPOWER                                     = 0X01,
    POWER_MODE_SUSPEND                                      = 0X02
} bno055_powermode_t;

//initialize the BNO055
uint8_t BNO055Read8(uint8_t reg);
uint16_t BNO055Read16(uint8_t reg);
uint32_t BNO055Read24(uint8_t reg);
uint16_t BNO055Read16LE(uint8_t reg);
int16_t BNO055ReadS16(uint8_t reg);
int16_t BNO055ReadS16LE(uint8_t reg);
void BNO055_WriteRegister(uint8_t reg, uint8_t val);
bool BNO055_Init(void);
void BNO055SetMode(bno055_powermode_t mode);
void BNO055SetPage(bno055_page_t page);
void BNO055ResetInt(void);
void BNO055EnableIntOnXYZ(uint8_t x, uint8_t y, uint8_t z);
void BNO055EnableAnyMotion(uint8_t threshold, uint8_t duration);
void BNO055DisableAnyMotion(void);
void BNO055SetInterruptEnableAccelAM(uint8_t enable);
void BNO055SetInterruptMaskAccelAM(uint8_t enable);
uint8_t BNO055SliceValueIntoRegister(uint8_t value, uint8_t reg, uint8_t mask, uint8_t position);

#endif /* SRC_IMU_H_ */
