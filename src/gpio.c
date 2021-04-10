/*
 * gpio.c
 *
 *  Created on: Dec 12, 2018
 *      Author: Dan Walkes
 *
 *
 *      Edited By : Madhukar Arora
 *      Organization : University of Colorado Boulder
 *      Reference :  Utilized Silicon Labs' EMLIB peripheral libraries to implement functionality.
 *
 *      NOTE : All modifications made to the starter code are commented with the Modification label and a description.
 */
#include "gpio.h"
#include "em_gpio.h"
#include <string.h>


/*
 * MODIFICATION : Added conditional directives to select strong drive and weak drive on LED Pins.
 */

#define LED0_WEAK
//#define LED0_STRONG
#define LED1_WEAK
//#define LED1_STRONG
#define LEDDBG_WEAK
//#define LEDDBG_STRONG

/*
 * MODIFICATION : Modifications made here to include the port and pins of the LED
 */
#define	LED0_port (gpioPortF)
#define LED0_pin  (4)
#define LED1_port (gpioPortF)
#define LED1_pin  (5)

#define SI7021EN_port (gpioPortD)
#define SI7021EN_pin  (15)

#define SCL_port      (gpioPortC)
#define SCL_pin       (10)
#define SDA_port      (gpioPortC)
#define SDA_pin       (11)

#define LEDDBG_port	(gpioPortB)	// board
#define LEDDBG_pin	(11)

#define Display_Port  (gpioPortD)
#define Display_Pin   (15)

#define Extcomin_Port (gpioPortD)
#define Extcomin_Pin  (13)

void gpioInit()
{
	/*
	 * LED 0
	 */
//MODIFICATION : Use of conditional directives to select the drive strength.
#ifdef LED0_STRONG
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
#endif

#ifdef LED0_WEAK
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
#endif
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);

	/*
	 * LED 1
	 */
#ifdef LED1_STRONG
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
#endif

#ifdef LED1_WEAK
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
#endif
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);

	/*
	 * LED DEBUG (board)
	 */
#ifdef LEDDBG_STRONG
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
#endif

#ifdef LEDDBG_WEAK
	GPIO_DriveStrengthSet(LEDDBG_port, gpioDriveStrengthWeakAlternateWeak);
#endif
	GPIO_PinModeSet(LEDDBG_port, LEDDBG_pin, gpioModePushPull, false);
}

/**
 * @brief get_leds
 * Get LED statuses as two least significant bits of a return byte.
 * @return uint8_t LED status byte.
 */
uint8_t get_leds(void)
{
//	return ((GPIO_PinOutGet(LED1_port, LED1_pin) << 1 ) | GPIO_PinOutGet(LED0_port, LED0_pin));	// devkit
	return GPIO_PinOutGet(LED0_port, LED0_pin);	// board
}

/**
 * @brief LED control function
 * bit 0 = LED0
 * bit 1 = LED1
 * bits 2-7 = don't care
 */
void set_leds(uint8_t control_byte)
{

  /* LED 0 control */
  if ((control_byte & 0x01) == 1) {
    GPIO_PinOutSet(LED0_port, LED0_pin);
  } else {
    GPIO_PinOutClear(LED0_port, LED0_pin);
  }

//  /* LED 1 control */
//  if (((control_byte >> 1) & 0x01) == 1) {
//    GPIO_PinOutSet(LED1_port, LED1_pin);
//  } else {
//    GPIO_PinOutClear(LED1_port, LED1_pin);
//  }
}

void gpioLed0SetOn()
{
	GPIO_PinOutSet(LED0_port,LED0_pin);
}
void gpioLed0SetOff()
{
	GPIO_PinOutClear(LED0_port,LED0_pin);
}
void gpioLed1SetOn()
{
	GPIO_PinOutSet(LED1_port,LED1_pin);
}
void gpioLed1SetOff()
{
	GPIO_PinOutClear(LED1_port,LED1_pin);
}
void gpioLedDbgSetOn()
{
	GPIO_PinOutSet(LEDDBG_port,LEDDBG_pin);
}
void gpioLedDbgSetOff()
{
	GPIO_PinOutClear(LEDDBG_port,LEDDBG_pin);
}

/*
 * sets the enable pin to the temperature sensor
 */
void si7021_enable()
{
	GPIO_PinOutSet(SI7021EN_port,SI7021EN_pin);
}

/*
 * clears the enable pin to the temperature sensor
 */
void si7021_disable()
{
	GPIO_PinOutClear(SI7021EN_port,SI7021EN_pin);
}
/*
 * disable the I2C0 SDA
 */
void sda_disable()
{
	GPIO_PinOutClear(SDA_port,SDA_pin);
}
/*
 * disable the I2C0 SCL
 */
void scl_disable(){
	GPIO_PinOutClear(SCL_port,SCL_pin);
}



/*Enable the display on the Dev Board*/
void gpioEnableDisplay(void){
	GPIO_PinOutSet(Display_Port,Display_Pin);
}

/*COM Inversion Line. Must be pulsed periodically to prevent static build up in the display*/
void gpioSetDisplayExtcomin(bool high){
	if(high){
		GPIO_PinOutSet(Extcomin_Port,Extcomin_Pin);
	}
	else{
		GPIO_PinOutClear(Extcomin_Port,Extcomin_Pin);
	}
}
