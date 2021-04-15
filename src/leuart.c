/*
 * leuart.c
 *
 *  Created on: Apr 14, 2021
 *      Author: Dhruva
 */

#include "leuart.h"


LEUART_Init_TypeDef leuart0Init =
{
		.baudrate = 9600,
		.databits = leuartDatabits8,
		.enable = leuartDisable,
		.parity = leuartNoParity,
		.refFreq = 0,
		.stopbits = leuartStopbits1,
};

static UARTDRV_HandleData_t uartHandle0; /* UART driver handle */
static UARTDRV_Handle_t  testHandle0 = &uartHandle0;
static uint8_t rxByte;
uint8_t array[20];
int16_t packetLength;

static void UART_rx_callback(UARTDRV_Handle_t handle, Ecode_t transferStatus, uint8_t *data,
                             UARTDRV_Count_t transferCount);


static void UART_rx_callback(UARTDRV_Handle_t handle, Ecode_t transferStatus, uint8_t *data,
                             UARTDRV_Count_t transferCount)
{
  static  uint8_t rxCnt = 0;

  if(transferStatus == ECODE_EMDRV_UARTDRV_OK)
  {
	  memcpy(array, data, 10);
	  packetLength = 10;
	  displayPrintf(DISPLAY_ROW_ACTION,"U: %s", array);
	  rxCnt++;
  }

  /* RX the next byte */
//  UARTDRV_Receive(testHandle0, uartbuffer, 1, UART_rx_callback);
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
	UARTDRV_Receive(testHandle0, uartbuffer, 10, UART_rx_callback);

}




