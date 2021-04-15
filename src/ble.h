/*
 * @File   : ble.h
 * @brief  : source file containing function prototypes for BLE
 * @Author : Madhukar Arora
 */

#ifndef SRC_BLE_H_
#define SRC_BLE_H_

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#include "infrastructure.h"

/* EM library (EMlib) */
#include "em_system.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_core.h"
#include "em_cmu.h"
#include "timers.h"
#include "gpio.h"
#include "log.h"

/*
 * Temporary GNSS packet data struct
 */
typedef struct gnss_data
{
	int32_t lon;
	int32_t lat;
	float fLon;
	float fLat;
	int32_t gSpeed;
} gnss_data;

uint8_t rxUARTBuffer[100];

/*  Macro Definitions for RSSI*/
#define RSSI_NEG35DB		(-35)
#define RSSI_NEG45DB		(-45)
#define RSSI_NEG55DB		(-55)
#define RSSI_NEG65DB		(-65)
#define RSSI_NEG75DB		(-75)
#define RSSI_NEG85DB		(-85)

/*Macro Definition for TX Power*/
#define TX_PWR_MIN      	(-260)
#define TX_PWR_NEG20DB		(-200)
#define TX_PWR_NEG15DB		(-150)
#define TX_PWR_NEG5DB		 (-50)
#define TX_PWR_0DB      	   (0)
#define TX_PWR_5DB     		  (50)
#define TX_PWR_MAX            (80)


/*Function prototypes*/

/**
 * @brief : Function for taking a single temperature measurement with EFR32 internal temperature sensor.
 * @param : tempC is the temperature measured using I2C
 * @return : void
 * Credits : Used Silicon Labs Example Provided in Class Lecture as reference.
 */
void measure_temperature(float tempC);
/**
 * @brief : Function for taking a single pressure measurement with BME280 sensor.
 * @param : pressPa is the pressure measured using I2C
 * @return : void
 */
void measure_pressure(float pressPa);

/**
 * @brief : Function for getting GNSS location and speed measurements with uBlox MAX-M8-Q module.
 * @param : gnss_data* dat is the temporary struct containing GNSS packet data (TBD)
 * @return : void
 */
void measure_navigation(struct gnss_data* dat);

/**
 * @brief  : ble_EventHandler function to handle the BLE Events
 * @param  : pointer to gecko command structure
 * @return : void
 */
void ble_EventHandler(struct gecko_cmd_packet* evt);

/***************************************************************************//**
 *  Handling of external signal events.
 *
 *  @param[in] signal  External signal handle that is serviced by this function.
 ******************************************************************************/
void handle_external_signal_event(uint8_t signal);
#endif /* SRC_BLE_H_ */
