/*
 * @File   : oscillators.h
 * @brief  : header source file containing function prototypes for oscillator config
 * @Author : Madhukar Arora
 *
 */

#ifndef SRC_OSCILLATORS_H_
#define SRC_OSCILLATORS_H_

//INCLUDES
#include "em_cmu.h"
#include "main.h"



//FUNCTION PROTOTYPES
/*
 * @function : oscillatorInit()
 * @brief    : oscillatorInit function to configure the different clock oscillators based on the Energy Mode defined in main.h source file.
 * @param    : void
 * @return   : void
 */
void oscillatorInit(void);


#endif /* SRC_OSCILLATORS_H_ */
