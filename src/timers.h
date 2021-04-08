/*
 * @File   : timers.h
 * @brief  : header source file containing function prototypes for low energy timer configuration
 * @Author : Madhukar Arora
 *
 */

#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_

//INCLUDES
#include "em_letimer.h"
#include "main.h"

//MACROS

#if (LOW_ENERGY_MODE == 0 | LOW_ENERGY_MODE == 1 | LOW_ENERGY_MODE == 2)
#define PRESCALAR             (4)
#endif

#if (LOW_ENERGY_MODE == 3)
#define PRESCALAR             (1)
#endif

#if (LOW_ENERGY_MODE == 0 | LOW_ENERGY_MODE == 1 | LOW_ENERGY_MODE == 2)
#define ACTUAL_CLK_FREQ       (32768/PRESCALAR)
#endif

#if (LOW_ENERGY_MODE == 3)
#define ACTUAL_CLK_FREQ       (1000/PRESCALAR)
#endif

#define COMP0_VALUE           (LETIMER_TOTAL_PERIOD * ACTUAL_CLK_FREQ)/1000 //for total time period
#define TICK_CNT(delay)       ((delay/1000) * ACTUAL_CLK_FREQ)/1000 //delay period is in us



//FUNCTION PROTOTYPES

/*
 * @function : letimer0_Init
 * @brief    : Initialize and configure the Low Energy Timer on the Blue Gecko
 * @param    : void
 * @return   : void
 */
void letimer0_Init(void);


/*
 * @function : timerWaitUs
 * @brief    : function generates an interrupt when the delay passed has been generated
 * @param    : delay to be generated in microseconds (MAX ALLOWED 3s)
 * @return   : void
 * @Reference : Instructors Lecture 6 explanation on functioning of timer.
 */
void timerWaitUs(uint32_t us_wait);


#endif /* SRC_TIMERS_H_ */
