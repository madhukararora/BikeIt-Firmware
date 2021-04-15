/*
 * @File   : oscillators.c
 * @brief  : source file containing function definition for oscillator configuration
 * @Author : Madhukar Arora
 * Reference : Utilized Silicon Labs' EMLIB peripheral libraries to implement functionality.
 */



#include "oscillators.h"

/*
 * oscillatorInit function to configure the different clock oscillators based on the Energy Mode defined in main.h source file.
 */

void oscillatorInit(void){

	//uint32_t test_freq  = 0; /*remove comment to test the frequency of the clock branch */
#if (LOW_ENERGY_MODE == 3)
	//Enable the oscillator
	CMU_OscillatorEnable(cmuOsc_ULFRCO,true,true);
	//Select ULFRCO as clock source for LFA
	CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_ULFRCO);

	//Prescalar
	CMU_ClockDivSet(cmuClock_LETIMER0,cmuClkDiv_1);

	//Enable the clock
	CMU_ClockEnable(cmuClock_LFA,true);
#endif

#if (LOW_ENERGY_MODE == 0 | LOW_ENERGY_MODE == 1 | LOW_ENERGY_MODE == 2)
	CMU_OscillatorEnable(cmuOsc_LFXO,true,true);
	CMU_ClockEnable(cmuClock_HFLE, true);
	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO); // Set a reference clock
	////Select LFXO as clock source for LFA

	CMU_LFXOInit_TypeDef lfxoInit = CMU_LFXOINIT_DEFAULT;
	CMU_LFXOInit(&lfxoInit);
	CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFXO);

	//Prescalar
	CMU_ClockDivSet(cmuClock_LETIMER0,cmuClkDiv_4);

	//Enable the clock
	CMU_ClockEnable(cmuClock_LFA,true);
#endif
	//test_freq = CMU_ClockFreqGet(cmuClock_LFA); /*, remove comment to test the frequency of the clock branch*/

	//Enabling required clocks
	CMU_ClockEnable(cmuClock_GPIO,true); //GPIO  clock
	CMU_ClockEnable(cmuClock_LETIMER0,true); //LETIMER CLOCK




}
