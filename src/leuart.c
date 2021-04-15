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


//static DMA_CfgDescr_TypeDef LEUARTDmaRxCfhDescr, LEUARTDmaTxCfgDescr;
//static uint8_t dma_buffer[20];                                                                                  /* Buffer to hold the incoming LEUART data (including the signal frame '#') */

void LEUART_Sync(LEUART_TypeDef *leuart, uint32_t mask)
{
	/* Avoid deadlock if modifying the same register twice when freeze mode is */
	/* activated. */
	if (leuart->FREEZE & LEUART_FREEZE_REGFREEZE) {
	return;
	}

	/* Wait for any pending previous write operation to have been completed */
	/* in low frequency domain */
	while (leuart->SYNCBUSY & mask);
}

void LEUART_TxDMAEnable(bool enable)
{
	/* LF register about to be modified require sync. Busy check */
	LEUART_Sync(LEUART0, LEUART_SYNCBUSY_CMD);

	/* Disable TX -> Errata item LEUART_E201 */
	LEUART0->CMD = LEUART_CMD_TXDIS;

	LEUART_TxDmaInEM2Enable(LEUART0, enable);

	/* LF register about to be modified require sync. busy check */
	LEUART_Sync(LEUART0, LEUART_SYNCBUSY_CMD);

	/* Enable TX -> Errata item LEUART_E201 */
	LEUART0->CMD = LEUART_CMD_TXEN;
}

void LEUART_RxBlock()
{
	/* LF register about to be modified require sync. Busy check */
	LEUART_Sync(LEUART0, LEUART_SYNCBUSY_CMD);

	/* Block RX until the start frame is received which will automatically unblock RX */
	LEUART0->CMD = LEUART_CMD_RXBLOCKEN;
}

void LEUART0_enter_DefaultMode_from_RESET(void) {
	LOG_INFO("LEUART Init start2");
	LEUART_Init_TypeDef initleuart = LEUART_INIT_DEFAULT;

	/* Enable clock for LDMA */
	CMU_ClockEnable(cmuClock_LDMA, true);

	/* Enable clock for LEUART0 */
	CMU_ClockEnable(cmuClock_LEUART0, true);
	CMU_ClockDivSet(cmuClock_LEUART0, cmuClkDiv_1); // Don't prescale LEUART clock

	LEUART_Init(LEUART0, &initleuart);

	/* Configuring LEUART I/O */
	LEUART0->ROUTELOC0 = LEUART_ROUTELOC0_RXLOC_LOC2 | LEUART_ROUTELOC0_TXLOC_LOC2;
	LEUART0->ROUTEPEN  = LEUART_ROUTEPEN_RXPEN | LEUART_ROUTEPEN_TXPEN;
	/* Set up RX pin */
//	LEUART0->ROUTELOC0 = (LEUART0->ROUTELOC0 & (~_LEUART_ROUTELOC0_RXLOC_MASK)) | LEUART_ROUTELOC0_RXLOC_LOC0;
//	LEUART0->ROUTEPEN = LEUART0->ROUTEPEN | LEUART_ROUTEPEN_RXPEN;
	/* Set up TX pin */
//	LEUART0->ROUTELOC0 = (LEUART0->ROUTELOC0 & (~_LEUART_ROUTELOC0_TXLOC_MASK)) | LEUART_ROUTELOC0_TXLOC_LOC0;
//	LEUART0->ROUTEPEN = LEUART0->ROUTEPEN | LEUART_ROUTEPEN_TXPEN;
	/* Configuring non-standard properties */
	LEUART_RxDmaInEM2Enable(LEUART0, 0);
	LEUART_RxDmaInEM2Enable(LEUART0, 1);
}

void initLEUART(void){
	/* Freeze registers to avoid stalling for LF synchronization */
	LEUART_FreezeEnable(LEUART0, true);

	/* LEUART_CTRL_SFUBRX - Clears RXBLOCK when the start-frame is found in the incoming data.
	* The start-frame is loaded into the receive buffer */
	LEUART0->CTRL = LEUART_CTRL_SFUBRX;
//	LEUART0->CMD |= LEUART_CMD_RXEN;

	/* Set the START FRAME which will wake-up the CPU with an interrupt */
	LEUART0->STARTFRAME = (uint32_t)'$';
	/* Set the SIGNAL FRAME which will wake-up the CPU with an interrupt */
	LEUART0->SIGFRAME = (uint32_t)'\r';

	/* Block RX until the start frame is received, which will automatically unblock RX. */
	LEUART_RxBlock();

	/* Unfreeze registers, pass new settings on to LEUART */
	LEUART_FreezeEnable(LEUART0, false);

	/* Enable START FRAME and SIGNAL FRAME interrupts as well as the LEUART interrupt vector in the NVIC */
	LEUART_IntEnable(LEUART0, LEUART_IEN_SIGF);
	LEUART_IntEnable(LEUART0, LEUART_IEN_STARTF);
	NVIC_ClearPendingIRQ(LEUART0_IRQn);
	NVIC_EnableIRQ(LEUART0_IRQn);
	LOG_INFO("LEUART Init end");
}

void LEUART0_IRQHandler(void){
	if (LEUART_IntGet(LEUART0) & LEUART_IF_STARTF) {
		LOG_INFO("LEUART IRQ SF");
		/* START FRAME '*' received -> send external signal to the stack */
		gecko_external_signal(LEUART_START_FRAME_EXT_SIGNAL);
		/* Clear interrupt */
		LEUART_IntClear(LEUART0, LEUART_IFC_STARTF);
	} else if (LEUART_IntGet(LEUART0) & LEUART_IF_SIGF) {
		LOG_INFO("LEUART IRQ SIG");
		/* SIGNAL FRAME '#' received -> send external signal to the stack */
		gecko_external_signal(LEUART_SIGNAL_FRAME_EXT_SIGNAL);
		/* Clear interrupt */
		LEUART_IntClear(LEUART0, LEUART_IFC_SIGF);
	}
}

bool DMATxCallback(void){
	LEUART_TxDMAEnable(false);
	return 0;
}

int DMARxCallback(unsigned int channel, unsigned int sequenceNo, void *userParam)
{
  /* We can receive at most 20 bytes and if that happens then it's functionality the same
   * as receiving the signal frame '#' so we can go and process the data
   */
	LOG_INFO("DMARxCallback");
  gecko_external_signal(LEUART_SIGNAL_FRAME_EXT_SIGNAL);
  return 0;
}

void initDMA(void){
	/* Initial DMA driver setup. The actual transfer is configured when the start-frame is received */
	DMADRV_Init();
	/* Request DMA channels */
	DMADRV_AllocateChannel(&dma_rx_channel, NULL);
	DMADRV_AllocateChannel(&dma_tx_channel, NULL);
}



