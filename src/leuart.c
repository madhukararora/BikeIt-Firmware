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
GNSS_data_t GNRMC_data = {
		"$GNRMC",
		"000000.00",
		"3935.24000",
		3935.24000,
		"10514.2400",
		10514.2400,
		"2.00",
		1
};

void LEUART_rx_callback(UARTDRV_Handle_t handle, Ecode_t transferStatus, uint8_t *data, UARTDRV_Count_t transferCount){
	if(transferStatus == ECODE_EMDRV_UARTDRV_OK){
		char *gnssstr;
		// check if gnss packet starts with the GNRMC (Recommended Minimum Specific GNSS Data) and UTC time stamp is populated
		gpioGpsToggleSetOff();
		gnssstr = strstr((char *)data, "$GNRMC");
		if((gnssstr != 0) && (strlen(gnssstr) > 56) && !(strncmp(gnssstr, "$GNRMC,,", 8) == 0)){
			// split gnssstr into data we want
			strncpy(GNRMC_data.header, gnssstr, 6);
			strncpy(GNRMC_data.utctime, &gnssstr[7], 9);
			strncpy(GNRMC_data.latitude, &gnssstr[19], 10);
			if(strchr(GNRMC_data.latitude, ',') != NULL) return;	// check if gps lock is complete (in case of empty values during warmup)
			strncpy(GNRMC_data.longitude, &gnssstr[32], 11);
			strncpy(GNRMC_data.gspeed, &gnssstr[46], 4);

			// convert strings to floats for display
			GNRMC_data.flon = strtof(GNRMC_data.longitude, NULL) / 100;
			GNRMC_data.flat = strtof(GNRMC_data.latitude, NULL) / 100;
			float fgspeed = strtof(GNRMC_data.gspeed, NULL);
			GNRMC_data.gspd = (uint16_t)(FLT_TO_UINT32(fgspeed, 0) >> 1);	// convert speed in knots to m/s. units is 1/100 of a m/s
		}
	}
	gpioGpsToggleSetOn();
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




