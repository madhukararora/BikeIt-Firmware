/*
 * @File   : irq.c
 * @brief  : source file containing function definition for handling the interrupt for Low Energy Timer
 * @Author : Madhukar Arora
 * Reference : Utilized Silicon Labs' EMLIB peripheral libraries to implement functionality.
 */

#include "irq.h"

extern volatile bool BME_TRANSFER_DONE;
/*
 * Interrupt Handler for the LETIMER0
 * @param : void
 * @return : void
 */
void LETIMER0_IRQHandler(void){


	CORE_DECLARE_IRQ_STATE;

	uint32_t timer_status = LETIMER_IntGet(LETIMER0);

	//clear the interrupt
	LETIMER_IntClear(LETIMER0,timer_status);

	if(timer_status & LETIMER_IF_UF){
		CORE_ENTER_CRITICAL();
		roll_over++;
		gecko_external_signal(TIMER_UF);
		CORE_EXIT_CRITICAL();
	}

	if(timer_status & LETIMER_IF_COMP1){
		//disable COMP1 interrupt
		LETIMER_IntDisable(LETIMER0,LETIMER_IEN_COMP1);
		CORE_ENTER_CRITICAL();
		gecko_external_signal(DELAY_GENERATED);
		CORE_EXIT_CRITICAL();
	}
}

/*
 * Interrupt Handler for I2C0
 * @param : void
 * @return : void
 */
void I2C0_IRQHandler(void){
	CORE_DECLARE_IRQ_STATE;
	I2C_TransferReturn_TypeDef i2c_status;
	i2c_status = I2C_Transfer(I2C0);
	if(i2c_status == i2cTransferDone){
		CORE_ENTER_CRITICAL();
		BME_TRANSFER_DONE = true;
		gecko_external_signal(I2C_TRANSFER_DONE);
		CORE_EXIT_CRITICAL();
	}
}



