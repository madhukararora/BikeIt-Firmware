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
#include "em_cmu.h"
#include "timers.h"
#include "log.h"



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

void measure_pressure(float pressPa);

/**
 * @brief  : ble_EventHandler function to handle the BLE Events
 * @param  : pointer to gecko command structure
 * @return : void
 */
void ble_EventHandler(struct gecko_cmd_packet* evt);

#endif /* SRC_BLE_H_ */
