/*
 * @File   : irq.h
 * @brief  : header source file containing function prototypes for handling the interrupt for Low Energy Timer
 * @Author : Madhukar Arora
 *
 */
#ifndef SRC_IRQ_H_
#define SRC_IRQ_H_

//INCLUDES
#include "em_letimer.h"
#include "em_i2c.h"
#include "em_core.h"
#include "gpio.h"
#include "scheduler.h"
#include "log.h"


//FUNCTION PROTOTYPES
void LETIMER0_IRQHandler(void);
void I2C0_IRQHandler(void);

#endif /* SRC_IRQ_H_ */
