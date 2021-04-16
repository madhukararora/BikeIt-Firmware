/*
 * bme280.h
 *
 *  Created on: Apr 13, 2021
 *      Author: madhu
 */

#ifndef SRC_BME280_H_
#define SRC_BME280_H_

/**\name I2C addresses */
#define BME280_I2C_ADDR_PRIM                      (0x76) //address.
#define BME280_I2C_ADDR_SEC                       (0x77)

/**\name BME280 chip identifier */
#define BME280_CHIP_ID                            (0x60)

/**\name Register Address */
#define BME280_CHIP_ID_ADDR                       (0xD0)
#define BME280_RESET_ADDR                         (0xE0)
#define BME280_TEMP_PRESS_CALIB_DATA_ADDR         (0x88)
#define BME280_HUMIDITY_CALIB_DATA_ADDR           (0xE1)
#define BME280_PWR_CTRL_ADDR                      (0xF4)
#define BME280_CTRL_HUM_ADDR                      (0xF2)
#define BME280_CTRL_MEAS_ADDR                     (0xF4)
#define BME280_CONFIG_ADDR                        (0xF5)
#define BME280_DATA_ADDR                          (0xF7)




/**\name API success code */
#define BME280_OK                                 (0)

/**\name API error codes */
#define BME280_E_NULL_PTR                         (-1)
#define BME280_E_DEV_NOT_FOUND                    (-2)
#define BME280_E_INVALID_LEN                      (-3)
#define BME280_E_COMM_FAIL                        (-4)
#define BME280_E_SLEEP_MODE_FAIL                  (-5)
#define BME280_E_NVM_COPY_FAILED                  (-6)

/**\name API warning codes */
#define BME280_W_INVALID_OSR_MACRO                (1)

/**\name Macros related to size */
#define BME280_TEMP_PRESS_CALIB_DATA_LEN          (26)
#define BME280_HUMIDITY_CALIB_DATA_LEN            (7)
#define BME280_P_T_H_DATA_LEN                     (8)

/**\name Sensor power modes */
#define BME280_SLEEP_MODE                         (0x00)
#define BME280_FORCED_MODE                        (0x01)
#define BME280_NORMAL_MODE                        (0x03)

/**\name Macro to combine two 8 bit data's to form a 16 bit data */
#define BME280_CONCAT_BYTES(msb, lsb)             (((uint16_t)msb << 8) | (uint16_t)lsb)

#define BME280_SET_BITS(reg_data, bitname, data) \
    ((reg_data & ~(bitname##_MSK)) | \
     ((data << bitname##_POS) & bitname##_MSK))
#define BME280_SET_BITS_POS_0(reg_data, bitname, data) \
    ((reg_data & ~(bitname##_MSK)) | \
     (data & bitname##_MSK))

#define BME280_GET_BITS(reg_data, bitname)        ((reg_data & (bitname##_MSK)) >> \
                                                   (bitname##_POS))
#define BME280_GET_BITS_POS_0(reg_data, bitname)  (reg_data & (bitname##_MSK))

/**\name Macros for bit masking */
#define BME280_SENSOR_MODE_MSK                    UINT8_C(0x03)
#define BME280_SENSOR_MODE_POS                    UINT8_C(0x00)

#define BME280_CTRL_HUM_MSK                       UINT8_C(0x07)
#define BME280_CTRL_HUM_POS                       UINT8_C(0x00)

#define BME280_CTRL_PRESS_MSK                     UINT8_C(0x1C)
#define BME280_CTRL_PRESS_POS                     UINT8_C(0x02)

#define BME280_CTRL_TEMP_MSK                      UINT8_C(0xE0)
#define BME280_CTRL_TEMP_POS                      UINT8_C(0x05)

#define BME280_FILTER_MSK                         UINT8_C(0x1C)
#define BME280_FILTER_POS                         UINT8_C(0x02)

#define BME280_STANDBY_MSK                        UINT8_C(0xE0)
#define BME280_STANDBY_POS                        UINT8_C(0x05)

/**\name Sensor component selection macros
 * These values are internal for API implementation. Don't relate this to
 * data sheet.
 */
#define BME280_PRESS                              UINT8_C(1)
#define BME280_TEMP                               UINT8_C(1 << 1)
#define BME280_HUM                                UINT8_C(1 << 2)
#define BME280_ALL                                UINT8_C(0x07)

/**\name Settings selection macros */
#define BME280_OSR_PRESS_SEL                      UINT8_C(1)
#define BME280_OSR_TEMP_SEL                       UINT8_C(1 << 1)
#define BME280_OSR_HUM_SEL                        UINT8_C(1 << 2)
#define BME280_FILTER_SEL                         UINT8_C(1 << 3)
#define BME280_STANDBY_SEL                        UINT8_C(1 << 4)
#define BME280_ALL_SETTINGS_SEL                   UINT8_C(0x1F)

/**\name Oversampling macros */
#define BME280_NO_OVERSAMPLING                    UINT8_C(0x00)
#define BME280_OVERSAMPLING_1X                    UINT8_C(0x01)
#define BME280_OVERSAMPLING_2X                    UINT8_C(0x02)
#define BME280_OVERSAMPLING_4X                    UINT8_C(0x03)
#define BME280_OVERSAMPLING_8X                    UINT8_C(0x04)
#define BME280_OVERSAMPLING_16X                   UINT8_C(0x05)

/**\name Measurement delay calculation macros  */
#define BME280_MEAS_OFFSET                        UINT16_C(1250)
#define BME280_MEAS_DUR                           UINT16_C(2300)
#define BME280_PRES_HUM_MEAS_OFFSET               UINT16_C(575)
#define BME280_MEAS_SCALING_FACTOR                UINT16_C(1000)

/**\name Standby duration selection macros */
#define BME280_STANDBY_TIME_0_5_MS                (0x00)
#define BME280_STANDBY_TIME_62_5_MS               (0x01)
#define BME280_STANDBY_TIME_125_MS                (0x02)
#define BME280_STANDBY_TIME_250_MS                (0x03)
#define BME280_STANDBY_TIME_500_MS                (0x04)
#define BME280_STANDBY_TIME_1000_MS               (0x05)
#define BME280_STANDBY_TIME_10_MS                 (0x06)
#define BME280_STANDBY_TIME_20_MS                 (0x07)

/**\name Filter coefficient selection macros */
#define BME280_FILTER_COEFF_OFF                   (0x00)
#define BME280_FILTER_COEFF_2                     (0x01)
#define BME280_FILTER_COEFF_4                     (0x02)
#define BME280_FILTER_COEFF_8                     (0x03)
#define BME280_FILTER_COEFF_16                    (0x04)

#define BME280_STATUS_REG_ADDR                    (0xF3)
#define BME280_SOFT_RESET_COMMAND                 (0xB6)
#define BME280_STATUS_IM_UPDATE                   (0x01)

//initialize the BME280
void bmeInit(void);
#endif /* SRC_BME280_H_ */
