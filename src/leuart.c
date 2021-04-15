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
	  if(strstr(array, "$GNRMC")){
		  gpioLed0SetOn();
//		  displayPrintf(DISPLAY_ROW_ACTION,":%s", array);
	  }
//	  displayPrintf(DISPLAY_ROW_ACTION,":%s", array);
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




