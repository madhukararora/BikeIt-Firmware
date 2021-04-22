/*
 * leuart.c
 *
 *  Created on: Apr 14, 2021
 *      Author: Dhruva
 */

#include "leuart.h"

#define packetLength 100

char gnssarray[100];
UARTDRV_HandleData_t leuartHandle0; /* UART driver handle */
UARTDRV_Handle_t  gnssHandle0 = &leuartHandle0;
GNSS_data_t GNRMC_data = {
		"$GNRMC",
		"00000.000",
		"0000.00000",
		"00000.0000",
		"0.00"
};

void LEUART_rx_callback(UARTDRV_Handle_t handle, Ecode_t transferStatus, uint8_t *data, UARTDRV_Count_t transferCount){
	static  uint8_t rxCnt = 0;
	if(transferStatus == ECODE_EMDRV_UARTDRV_OK){
		// copy leuart rx data into gnss buffer array
		memcpy(gnssarray, data, packetLength);
		// check if gnss packet starts with the GNRMC (Recommended Minimum Specific GNSS Data) and UTC time stamp is populated
		if((strncmp(gnssarray, "$GNRMC", 6) == 0) && !(strncmp(gnssarray, "$GNRMC,,", 8) == 0)){
			// split gnssarray into data we want
			strncpy(GNRMC_data.header, gnssarray, 6);
			strncpy(GNRMC_data.utctime, &gnssarray[7], 9);
			strncpy(GNRMC_data.latitude, &gnssarray[19], 10);
			if(strchr(GNRMC_data.latitude, ',') != NULL) return;	// check if gps lock is full (in case of empty values during warmup)
			strncpy(GNRMC_data.longitude, &gnssarray[32], 11);
			strncpy(GNRMC_data.gspeed, &gnssarray[46], 4);
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




