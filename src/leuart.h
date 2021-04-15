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
#include "display.h"
#include "gpio.h"
#include "irq.h"
#include "ble.h"

#include "em_chip.h"
#include "em_device.h"

#include "em_cmu.h"
#include "em_int.h"
#include "em_emu.h"
//#include "em_dma.h"
#include "em_gpio.h"
#include "uartdrv.h"

uint8_t uartbuffer[100];
#define NUM_TX_BUFFERS          8

#define LEUART_START_FRAME_EXT_SIGNAL 9   /* Bit flag for external signal */
#define LEUART_SIGNAL_FRAME_EXT_SIGNAL  8 /* Bit flag for external signal */

void initLEUART(void);
void UART_rx_callback(UARTDRV_Handle_t handle, Ecode_t transferStatus, uint8_t *data,
                             UARTDRV_Count_t transferCount);

#endif /* SRC_LEUART_H_ */
