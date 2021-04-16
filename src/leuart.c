/*
 * leuart.c
 *
 *  Created on: Apr 14, 2021
 *      Author: Dhruva
 */

#include "leuart.h"

#define packetLength 66

char gnssarray[100];
UARTDRV_HandleData_t leuartHandle0; /* UART driver handle */
UARTDRV_Handle_t  gnssHandle0 = &leuartHandle0;

void UART_rx_callback(UARTDRV_Handle_t handle, Ecode_t transferStatus, uint8_t *data, UARTDRV_Count_t transferCount)
{
  static  uint8_t rxCnt = 0;
  if(transferStatus == ECODE_EMDRV_UARTDRV_OK)
  {
	  gpioLed1SetOff();
	  // copy leuart rx data into gnss buffer array
	  memcpy(gnssarray, data, packetLength);
	  // check if gnss packet starts with the GNRMC (Recommended Minimum Specific GNSS Data) and UTC time stamp is populated
	  if((strncmp(gnssarray, "$GNRMC", 6) == 0) && !(strncmp(gnssarray, "$GNRMC,,", 8) == 0)){
		  gpioLed1SetOn();
		  // split gnssarray into data we want
		  char header[7], utctime[10], latitude[11], longitude[12], gspeed[5] = {'\0'};
		  strncpy(header, gnssarray, 6);
		  strncpy(utctime, &gnssarray[7], 9);
		  strncpy(latitude, &gnssarray[19], 10);
		  if(strchr(latitude, ',') != NULL) return;	// check if gps lock is full (in case of empty values during warmup)
		  strncpy(longitude, &gnssarray[32], 11);
		  strncpy(gspeed, &gnssarray[46], 4);
		  displayPrintf(DISPLAY_ROW_BTADDR,"Header:%s", header);
		  displayPrintf(DISPLAY_ROW_BTADDR2,"UTC:%s", utctime);
		  displayPrintf(DISPLAY_ROW_CONNECTION,"Lat:%s", latitude);
		  displayPrintf(DISPLAY_ROW_PASSKEY,"Lon:%s", longitude);
		  displayPrintf(DISPLAY_ROW_ACTION,"Spd:%s", gspeed);
		  measure_navigation(gnssarray);	// send gnsarray for parsing and sendoff
	  }
	  memset(gnssarray, '\0', 100);	// reset gnss array
	  rxCnt++;
  }
}

void initLEUART(void){
	UARTDRV_InitLeuart_t initData;

	DEFINE_BUF_QUEUE(EMDRV_UARTDRV_MAX_CONCURRENT_RX_BUFS, rxBufferQueueI0);
	DEFINE_BUF_QUEUE(EMDRV_UARTDRV_MAX_CONCURRENT_TX_BUFS, txBufferQueueI0);

	initData.port                 = LEUART0;
	initData.baudRate             = 9600;
	initData.portLocationTx       = LEUART_ROUTELOC0_TXLOC_LOC27;	// GPS RX <-> PF3 (P10) TX LOC27
	initData.portLocationRx       = LEUART_ROUTELOC0_RXLOC_LOC27;	// GPS TX <-> PF4 (P30) RX LOC27
	initData.stopBits             = (LEUART_Stopbits_TypeDef) LEUART_CTRL_STOPBITS_ONE;
	initData.parity               = (LEUART_Parity_TypeDef) LEUART_CTRL_PARITY_NONE;
	initData.fcType               = uartdrvFlowControlNone;
	initData.rxQueue              = (UARTDRV_Buffer_FifoQueue_t *)&rxBufferQueueI0;
	initData.txQueue              = (UARTDRV_Buffer_FifoQueue_t *)&txBufferQueueI0;

	UARTDRV_InitLeuart(gnssHandle0, &initData);
}




