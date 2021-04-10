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
#if BOARD
#define LEDDBG_WEAK
//#define LEDDBG_STRONG
#define GPSTOGGLE_WEAK
//#define GPSTOGGLE_STRONG
#define GPSRESET_WEAK
//#define GPSRESET_STRONG
#define GPSEXTINT_WEAK
//#define GPSEXTINT_STRONG
#endif

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

#if BOARD
#define LEDDBG_port	(gpioPortB)	// board
#define LEDDBG_pin	(11)	// board

#define GPSTOGGLE_port	(gpioPortF)
#define GPSTOGGLE_pin	(5)

#define GPSRESET_port	(gpioPortF)
#define GPSRESET_pin	(6)

#define GPSEXTINT_port	(gpioPortF)
#define GPSEXTINT_pin	(7)

#define DISP_port  (gpioPortC)
#define DISP_pin   (7)

#define EXTCOMIN_port (gpioPortC)
#define EXTCOMIN_pin  (8)

#define EXTMODE_port (gpioPortC)
#define EXTMODE_pin  (6)
#endif
#if DEVKIT
#define DISP_port  (gpioPortD)
#define DISP_pin   (15)

#define EXTCOMIN_port (gpioPortD)
#define EXTCOMIN_pin  (13)
#endif

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

#if BOARD
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

	/*
	 * GPS TOGGLE (board)
	 */
#ifdef GPSTOGGLE_STRONG
	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
#endif

#ifdef GPSTOGGLE_WEAK
	GPIO_DriveStrengthSet(GPSTOGGLE_port, gpioDriveStrengthWeakAlternateWeak);
#endif
	GPIO_PinModeSet(GPSTOGGLE_port, GPSTOGGLE_pin, gpioModePushPull, false);

	/*
	 * GPS RESET_N (board) GPS RESET_N pin is pulled up internally
	 */
#ifdef GPSRESET_STRONG
	GPIO_DriveStrengthSet(GPSRESET_port, gpioDriveStrengthStrongAlternateStrong);
#endif

#ifdef GPSRESET_WEAK
	GPIO_DriveStrengthSet(GPSRESET_port, gpioDriveStrengthWeakAlternateWeak);
#endif
	GPIO_PinModeSet(GPSRESET_port, GPSRESET_pin, gpioModePushPull, false);	// may need different mode than gpioModePushPull

	/*
	 * GPS EXTINT (board) leave open if unused
	 */
#ifdef GPSEXTINT_STRONG
	GPIO_DriveStrengthSet(GPSEXTINT_port, gpioDriveStrengthStrongAlternateStrong);
#endif

#ifdef GPSEXTINT_WEAK
	GPIO_DriveStrengthSet(GPSEXTINT_port, gpioDriveStrengthWeakAlternateWeak);
#endif
	GPIO_PinModeSet(GPSEXTINT_port, GPSEXTINT_pin, gpioModePushPull, false);
#endif
}

/**
 * @brief get_leds
 * Get LED statuses as two least significant bits of a return byte.
 * @return uint8_t LED status byte.
 */
uint8_t get_leds(void)
{
#if DEVKIT
	return ((GPIO_PinOutGet(LED1_port, LED1_pin) << 1 ) | GPIO_PinOutGet(LED0_port, LED0_pin));	// devkit
#endif
#if BOARD
	return GPIO_PinOutGet(LED0_port, LED0_pin);	// board
#endif
}

/**
 * @brief LED control function
 * bit 0 = LED0
 * bit 1 = LED1
 * bits 2-7 = don't care
 */
void set_leds(uint8_t control_byte)
{
#if DEVKIT
  /* LED 0 control */
  if ((control_byte & 0x01) == 1) {
    GPIO_PinOutSet(LED0_port, LED0_pin);
  } else {
    GPIO_PinOutClear(LED0_port, LED0_pin);
  }

  /* LED 1 control */
  if (((control_byte >> 1) & 0x01) == 1) {
    GPIO_PinOutSet(LED1_port, LED1_pin);
  } else {
    GPIO_PinOutClear(LED1_port, LED1_pin);
  }
#endif
#if BOARD
  /* LED DEBUG control */
  if ((control_byte & 0x01) == 1) {
    GPIO_PinOutSet(LEDDBG_port, LEDDBG_pin);
  } else {
    GPIO_PinOutClear(LEDDBG_port, LEDDBG_pin);
  }
#endif
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
#if BOARD
void gpioLedDbgSetOn()
{
	GPIO_PinOutSet(LEDDBG_port,LEDDBG_pin);
}
void gpioLedDbgSetOff()
{
	GPIO_PinOutClear(LEDDBG_port,LEDDBG_pin);
}

void gpioGpsToggleSetOn()
{
	GPIO_PinOutSet(GPSTOGGLE_port,GPSTOGGLE_pin);
}
void gpioGpsToggleSetOff()
{
	GPIO_PinOutClear(GPSTOGGLE_port,GPSTOGGLE_pin);
}
// RESET_N pin is internally pulled up so MCU pin output should be pulled down
void gpioGpsResetSetOn()
{
	GPIO_PinOutSet(GPSRESET_port,GPSRESET_pin);
}
void gpioGpsResetSetOff()
{
	GPIO_PinOutClear(GPSRESET_port,GPSRESET_port);
}
// RESET_N pin should be left open if unused
void gpioGpsExtIntSetOn()
{
	GPIO_PinOutSet(GPSEXTINT_port,GPSEXTINT_pin);
}
void gpioGpsExtIntSetOff()
{
	GPIO_PinOutClear(GPSEXTINT_port,GPSEXTINT_port);
}
#endif
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



/*Enable the display on the Devkit or Board*/
void gpioEnableDisplay(void){
	GPIO_PinOutSet(DISP_port,DISP_pin);
}

/*COM Inversion Line. Must be pulsed periodically to prevent static build up in the display*/
void gpioSetDisplayExtcomin(bool high){
	if(high){
		GPIO_PinOutSet(EXTCOMIN_port,EXTCOMIN_pin);
	}
	else{
		GPIO_PinOutClear(EXTCOMIN_port,EXTCOMIN_pin);
	}
}

#if BOARD
/*COM Inversion Line Mode Switch. Default: Set High to enable SW COM Inversion. Set Low for SPI HW COM Inversion*/
void gpioSetDisplayExtmode(bool high){
	if(high){
		GPIO_PinOutSet(EXTMODE_port,EXTMODE_pin);
	}
	else{
		GPIO_PinOutClear(EXTMODE_port,EXTMODE_pin);
	}
}
#endif
