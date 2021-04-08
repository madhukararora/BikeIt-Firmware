/*
 * @File   : timers.c
 * @brief  : source file containing function definition for timer configuration
 * @Author : Madhukar Arora
 * Reference : https://www.silabs.com/community/wireless/bluetooth/knowledge-base.entry.html/2017/08/17/using_low_energytim-9RYB
 * 			   Reference : Utilized Silicon Labs' EMLIB peripheral libraries to implement functionality.
 */

#include "timers.h"



/*
 * Reference used linked below to configure the LETIMER
 * https://www.silabs.com/community/wireless/bluetooth/knowledge-base.entry.html/2017/08/17/using_low_energytim-9RYB
 */
void letimer0_Init(void){

	/* Set configurations for LETIMER 0 */
	const LETIMER_Init_TypeDef LETIMER_INIT_CONFIG =
	{
			.enable = true, /* Start counting when init completed*/
			.debugRun = false, /* Counter shall not keep running during debug halt. */
			.comp0Top = true, /* Load COMP0 register into CNT when counter underflows. COMP is used as TOP */
			.bufTop = false, /* Don't load COMP1 into COMP0 when REP0 reaches 0. */
			.out0Pol = 0, /* Idle value for output 0. */
			.out1Pol = 0, /* Idle value for output 1. */
			.ufoa0 = letimerUFOANone, /* No output on output 0 */
			.ufoa1 = letimerUFOANone, /* No output on output 1*/
			.repMode = letimerRepeatFree /* Count while REP != 0 */
	};

	/*Initialize LETIMER*/
	LETIMER_Init(LETIMER0,&LETIMER_INIT_CONFIG);
	LETIMER_CompareSet(LETIMER0,0,COMP0_VALUE);
	//Load max count value in Comparator 1 to avoid firing COMP1 Interrupt
	LETIMER_CompareSet(LETIMER0, 1,0xFFFF);


	//Disable COMP0 Interrupts
	LETIMER_IntDisable(LETIMER0,LETIMER_IEN_COMP0);

	//Enable Underflow Interrupt
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_UF);
	NVIC_EnableIRQ(LETIMER0_IRQn);

	LETIMER_Enable(LETIMER0,true);
}


/*
 * Modifying this routine to enable interrupts on comparator 1 to
 * generate an interrupt at required delay
 * Supports a maximum delay of 3s.
 */
void timerWaitUs(uint32_t us_delay){

	uint32_t timer_cnt = 0, tick_cnt = 0, difference = 0;

	tick_cnt = TICK_CNT(us_delay);


	timer_cnt = LETIMER_CounterGet(LETIMER0);
	if(tick_cnt <= timer_cnt){
		difference = timer_cnt - tick_cnt;
	}
	else{
		//roll-over condition
		difference = timer_cnt + LETIMER_CompareGet(LETIMER0,0) - tick_cnt;
	}

	//load value into COMP1 to generate required delay
	LETIMER_CompareSet(LETIMER0,1,difference);

	//Enable interrupt on Comparator 1 for Required Delay
	LETIMER_IntEnable(LETIMER0,LETIMER_IEN_COMP1);

}


