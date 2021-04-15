/*
 * leuart.h
 *
 *  Created on: Apr 14, 2021
 *      Author: Dhruva
 */

#ifndef SRC_LEUART_H_
#define SRC_LEUART_H_

#include "log.h"
#include "scheduler.h"
#include "gpio.h"
#include "irq.h"
#include "ble.h"

#include "em_chip.h"
#include "em_device.h"

#include "em_cmu.h"
#include "em_emu.h"
#include "em_dma.h"
#include "em_rtc.h"
#include "em_gpio.h"
#include "em_leuart.h"

#include "dmadrv.h"

#define LEUART_START_FRAME_EXT_SIGNAL 9   /* Bit flag for external signal */
#define LEUART_SIGNAL_FRAME_EXT_SIGNAL  8 /* Bit flag for external signal */

unsigned int dma_rx_channel;                                                                             /* Variable to get the allocated DMA channel for RX */
unsigned int dma_tx_channel;                                                                             /* Variable to get the allocated DMA channel for TX */

void LEUART_Sync(LEUART_TypeDef *leuart, uint32_t mask);
void LEUART_TxDMAEnable(bool enable);
void LEUART_RxBlock();
void LEUART0_enter_DefaultMode_from_RESET(void);
void initLEUART(void);
void LEUART0_IRQHandler(void);
bool DMATxCallback(void);
int DMARxCallback(unsigned int channel, unsigned int sequenceNo, void *userParam);
void initDMA(void);

#endif /* SRC_LEUART_H_ */
