/*
 * leuart.c
 *
 *  Created on: Apr 14, 2021
 *      Author: Dhruva
 */

#include "leuart.h"

char array[100];
#define packetLength 66
UARTDRV_HandleData_t uartHandle0; /* UART driver handle */
UARTDRV_Handle_t  testHandle0 = &uartHandle0;

void UART_rx_callback(UARTDRV_Handle_t handle, Ecode_t transferStatus, uint8_t *data,
                             UARTDRV_Count_t transferCount)
{
  static  uint8_t rxCnt = 0;

  if(transferStatus == ECODE_EMDRV_UARTDRV_OK)
  {
	  gpioLed0SetOff();
	  memcpy(array, data, 66);
	  if(strncmp(array, "$GNRMC", 6) == 0){
		  gpioLed0SetOn();
		  char header[7], utctime[10], latitude[11], longitude[12], gspeed[5] = {'\0'};
		  strncpy(header, array, 6);
		  strncpy(utctime, &array[7], 9);
		  strncpy(latitude, &array[19], 10);
		  strncpy(longitude, &array[32], 11);
		  strncpy(gspeed, &array[46], 4);
		  displayPrintf(DISPLAY_ROW_BTADDR,"Header:%s", header);
		  displayPrintf(DISPLAY_ROW_BTADDR2,"UTC:%s", utctime);
		  displayPrintf(DISPLAY_ROW_CONNECTION,"Lat:%s", latitude);
		  displayPrintf(DISPLAY_ROW_PASSKEY,"Lon:%s", longitude);
		  displayPrintf(DISPLAY_ROW_ACTION,"Spd:%s", gspeed);
	  }
	  memset(array, '\0', 100);
	  rxCnt++;
  }
}

void initLEUART(void){
	GPIO_PinModeSet(BSP_VCOM_ENABLE_PORT, BSP_VCOM_ENABLE_PIN, gpioModePushPull, 1);
	GPIO_PinOutSet(BSP_VCOM_ENABLE_PORT, BSP_VCOM_ENABLE_PIN);

	UARTDRV_InitLeuart_t initData;

	DEFINE_BUF_QUEUE(EMDRV_UARTDRV_MAX_CONCURRENT_RX_BUFS, rxBufferQueueI0);
	DEFINE_BUF_QUEUE(EMDRV_UARTDRV_MAX_CONCURRENT_TX_BUFS, txBufferQueueI0);

	initData.port                 = LEUART0;
	initData.baudRate             = 9600;
	initData.portLocationTx       = LEUART_ROUTELOC0_TXLOC_LOC2;
	initData.portLocationRx       = LEUART_ROUTELOC0_RXLOC_LOC2;
	initData.stopBits             = (LEUART_Stopbits_TypeDef) LEUART_CTRL_STOPBITS_ONE;
	initData.parity               = (LEUART_Parity_TypeDef) LEUART_CTRL_PARITY_NONE;
	initData.fcType               = uartdrvFlowControlNone;
	initData.rxQueue              = (UARTDRV_Buffer_FifoQueue_t *)&rxBufferQueueI0;
	initData.txQueue              = (UARTDRV_Buffer_FifoQueue_t *)&txBufferQueueI0;

	UARTDRV_InitLeuart(testHandle0, &initData);
}




