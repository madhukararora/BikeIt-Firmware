/*
 * @File   : scheduler.h
 * @brief  : header source file containing function prototypes for scheduler
 * @Author : Madhukar Arora
 *
 */
#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

//INCLUDES
#include <stdbool.h>
#include "em_letimer.h"
#include "em_core.h"
#include "em_i2c.h"
#include "i2c.h"
#include "sleep.h"
#include "ble.h"



//GLOBAL VARIABLES
volatile uint32_t eventFlag; //variable to get the event from the IRQ

struct{
	uint8_t data_8;
	uint16_t data_16;
	uint16_t temp_code;
	float tempC;
}temp_Si7021;


typedef enum uint32_t{
	NO_PENDING_EVENTS = 0x0,
	DELAY_GENERATED = (0x1UL << 0),
	TIMER_UF = (0x1UL << 1),
	I2C_TRANSFER_DONE = (0x1UL << 2),
}scheduler_events_t;


typedef enum{
	POWER_ON,
	WRITE_BEGIN,
	WRITE_DONE,
	READ_BEGIN,
	READ_DONE,
	POWER_OFF
}scheduler_states_t;



//FUNCTION PROTOTYPES

/*
 * @function : scheduler_Init
 * @brief    : Initialize the eventFlag to 0
 * @param    : void
 * @return   : void
 */
void scheduler_Init(void);

/*
 * @function : scheduler_GetEvent
 * @brief    : function to check if the given event is set and clear if set.
 * @param    : event to be checked
 * @return   : returns true if event is present
 */
bool scheduler_GetEvent(uint32_t event);

/*
 * @function : scheduler_EventsPresent
 * @brief    : check for any pending events
 * @param    : void
 * @return   : returns true or false based on any pending events.
 */
bool scheduler_EventsPresent(void);

/*
 * @function : scheduler_SetEvent
 * @brief    : read/write modifies on the eventFlag variable to add new events
 * @param    : event from IRQ
 * @return   : void
 */
void scheduler_SetEvent(uint32_t event);

/*
 * @function : process_event
 * @brief    : Event Driven State Machine to process the different events
 * @param    : event to process
 * @return   : void
 */
void process_event(struct gecko_cmd_packet* evt);

/***************************************************************************//**
 *  Handling of external signal events.
 *
 *  @param[in] signal  External signal handle that is serviced by this function.
 ******************************************************************************/
void handle_external_signal_event(uint8_t signal);

#endif /* SRC_SCHEDULER_H_ */
