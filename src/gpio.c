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
#include <gpiointerrupt.h>
#include "native_gecko.h"
#include "em_gpio.h"
#include <string.h>


/*
 * MODIFICATION : Added conditional directives to select strong drive and weak drive on GPIO Pins.
 */

#define LED0_WEAK
//#define LED0_STRONG
#define LED1_WEAK
//#define LED1_STRONG
#define LEDDBG_WEAK
//#define LEDDBG_STRONG
#if BOARD
#define PMUXD1_WEAK
//#define PMUXD1_STRONG

#define GPSTOGGLE_WEAK
//#define GPSTOGGLE_STRONG
#define GPSRESET_WEAK
//#define GPSRESET_STRONG
#define GPSEXTINT_WEAK
//#define GPSEXTINT_STRONG
#define IMURESET_WEAK
//#define IMURESET_STRONG
#endif

/*
 * MODIFICATION : Modifications made here to include the port and pins of the LED and board GPIO
 */

#define SCL_port      (gpioPortC)
#define SCL_pin       (10)
#define SDA_port      (gpioPortC)
#define SDA_pin       (11)

// switch to board

#define LEDDBG_port	(gpioPortB)
#define LEDDBG_pin	(11)

#define PMUXD1_port	(gpioPortA)
#define PMUXD1_pin	(5)


#define PB0_port	(gpioPortB)
#define PB0_pin		(12)

#define PB1_port	(gpioPortB)
#define PB1_pin		(13)

#define GPSRESET_port	(gpioPortF)
#define GPSRESET_pin	(6)

#define GPSEXTINT_port	(gpioPortF)
#define GPSEXTINT_pin	(7)

#define IMURESET_port	(gpioPortD)
#define IMURESET_pin	(12)



// LCD Display GPIO
#define DISP_port  (gpioPortC)
#define DISP_pin   (7)

#define EXTCOMIN_port (gpioPortC)
#define EXTCOMIN_pin  (8)

#define EXTMODE_port (gpioPortC)
#define EXTMODE_pin  (6)

// MCU to BME280 I2C0. Check schematic first. Can be moved to dedicated BME280/I2C header file
#define SDABME_port	(gpioPortA)
#define SDABME_pin	(2)
#define SCLBME_port	(gpioPortA)
#define SCLBME_pin	(3)

// MCU to Display SPI. Check schematic first. Can be moved to dedicated Display header file
#define SPICS_port	(gpioPortC)
#define SPICS_pin	(9)
#define SPISDI_port	(gpioPortC)
#define SPISDI_pin	(10)
#define SPISCK_port	(gpioPortC)
#define SPISCK_pin	(11)

// MCU Rx to GPS Tx LEUART0. MCU Tx to GPS Rx LEUART. Check schematic first. Can be moved to dedicated GPS/LEUART header file
#define UARTRX_port	(gpioPortF)
#define UARTRX_pin	(3)
#define UARTTX_port	(gpioPortF)
#define UARTTX_pin	(4)

#define GPSTOGGLE_port	(gpioPortF)
#define GPSTOGGLE_pin	(5)

#define IMUEXTINT_port	(gpioPortD)
#define IMUEXTINT_pin	(13)
// MCU to BNO055 IMU I2C1. Check schematic first. Can be moved to dedicated BNO055/I2C header file
#define SDABNO_port	(gpioPortD)
#define SDABNO_pin	(10)
#define SCLBNO_port	(gpioPortD)
#define SCLBNO_pin	(11)

#define LEUART0_port gpioPortA
#define LEUART0_RX_pin 3
#define LEUART0_TX_pin 2


void gpioInit()
{
//MODIFICATION : Use of conditional directives to select the drive strength.
	GPIO_PinModeSet(BSP_VCOM_ENABLE_PORT, BSP_VCOM_ENABLE_PIN, gpioModeDisabled, 0);
	/*
	 * LED DEBUG (board)
	 */
#ifdef LEDDBG_STRONG
	GPIO_DriveStrengthSet(LEDDBG_port, gpioDriveStrengthStrongAlternateStrong);
#endif

#ifdef LEDDBG_WEAK
	GPIO_DriveStrengthSet(LEDDBG_port, gpioDriveStrengthWeakAlternateWeak);
#endif
	GPIO_PinModeSet(LEDDBG_port, LEDDBG_pin, gpioModePushPull, false);
	/*
	 * PMUX D1 (board)
	 */
#ifdef PMUXD1_STRONG
	GPIO_DriveStrengthSet(PMUXD1_port, gpioDriveStrengthStrongAlternateStrong);
#endif

#ifdef PMUXD1_WEAK
	GPIO_DriveStrengthSet(PMUXD1_port, gpioDriveStrengthWeakAlternateWeak);
#endif
	GPIO_PinModeSet(PMUXD1_port, PMUXD1_pin, gpioModePushPull, false);


	/*
	 * GPS RESET_N (board) GPS RESET_N pin is pulled up internally
	 */
#ifdef GPSRESET_STRONG
	GPIO_DriveStrengthSet(GPSRESET_port, gpioDriveStrengthStrongAlternateStrong);
#endif

#ifdef GPSRESET_WEAK
	GPIO_DriveStrengthSet(GPSRESET_port, gpioDriveStrengthWeakAlternateWeak);
#endif
	GPIO_PinModeSet(GPSRESET_port, GPSRESET_pin, gpioModeWiredAnd, true);

	/*
	 * GPS EXTINT (board) leave open if unused
	 */
#ifdef GPSEXTINT_STRONG
	GPIO_DriveStrengthSet(GPSEXTINT_port, gpioDriveStrengthStrongAlternateStrong);
#endif

#ifdef GPSEXTINT_WEAK
	GPIO_DriveStrengthSet(GPSEXTINT_port, gpioDriveStrengthWeakAlternateWeak);
#endif
	GPIO_PinModeSet(GPSEXTINT_port, GPSEXTINT_pin, gpioModeWiredAnd, false);
	GPIO_PinOutSet(GPSEXTINT_port, GPSEXTINT_pin);

	/*
	 * IMU nRESET (board) IMU nRESET pin is active low
	 */
#ifdef IMURESET_STRONG
	GPIO_DriveStrengthSet(IMURESET_port, gpioDriveStrengthStrongAlternateStrong);
#endif

#ifdef IMURESET_WEAK
	GPIO_DriveStrengthSet(IMURESET_port, gpioDriveStrengthWeakAlternateWeak);
#endif
	GPIO_PinModeSet(IMURESET_port, IMURESET_pin, gpioModeWiredAnd, true);	// floating
//	GPIO_PinOutSet(IMURESET_port, IMURESET_pin);

	/*
	 * IMU EXTINT (not actually EXT) (board) is output to MCU
	 */
	GPIO_PinModeSet(IMUEXTINT_port, IMUEXTINT_pin, gpioModeInputPull, true);
	bnoEnableInterrupts();
	/*
	 * GPS TOGGLE (board)
	 */
#ifdef GPSTOGGLE_STRONG
	GPIO_DriveStrengthSet(GPSTOGGLE_port, gpioDriveStrengthStrongAlternateStrong);
#endif

#ifdef GPSTOGGLE_WEAK
	GPIO_DriveStrengthSet(GPSTOGGLE_port, gpioDriveStrengthWeakAlternateWeak);
#endif
	GPIO_PinModeSet(GPSTOGGLE_port, GPSTOGGLE_pin, gpioModePushPull, false);
	/*
	 * PB[0:1] are pushbutton inputs
	 */
	GPIO_PinModeSet(PB0_port, PB0_pin, gpioModeInputPull, true);
	GPIO_PinModeSet(PB1_port, PB1_pin, gpioModeInputPull, true);
	enable_button_interrupts();
}

/**
 * @brief get_leds
 * Get LED statuses as two least significant bits of a return byte.
 * @return uint8_t LED status byte.
 */
uint8_t get_leds(void)
{
#if BOARD
	return GPIO_PinOutGet(LEDDBG_port, LEDDBG_pin);	// board
#endif
}

/**
 * @brief LED control function
 * bit 0 = LED0
 * bits 1-7 = don't care
 */
void set_leds(uint8_t control_byte)
{
  /* LED DEBUG control */
  if ((control_byte & 0x01) == 1) {
    GPIO_PinOutClear(LEDDBG_port, LEDDBG_pin);
  } else {
    GPIO_PinOutSet(LEDDBG_port, LEDDBG_pin);
  }
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

/*
 * disable the I2C0 BNO055 SDA
 */
void bnoSDADisable()
{
	GPIO_PinOutClear(SDABNO_port,SDABNO_pin);
}
/*
 * disable the I2C0 BNO055 SCL
 */
void bnoSCLDisable()
{
	GPIO_PinOutClear(SCLBNO_port,SCLBNO_pin);
}
void gpioGpsToggleSetOn()
{
	GPIO_PinOutSet(GPSTOGGLE_port,GPSTOGGLE_pin);
}
void gpioGpsToggleSetOff()
{
	GPIO_PinOutClear(GPSTOGGLE_port,GPSTOGGLE_pin);
}

void gpioPmuxD1SetOn()
{
	GPIO_PinOutSet(PMUXD1_port,LEDDBG_pin);
}
void gpioPmuxD1SetOff()
{
	GPIO_PinOutClear(PMUXD1_port,LEDDBG_pin);
}

void gpioLedDbgSetOn()
{
	GPIO_PinOutClear(LEDDBG_port,LEDDBG_pin);
}
void gpioLedDbgSetOff()
{
	GPIO_PinOutSet(LEDDBG_port,LEDDBG_pin);
}

// RESET_N pin is internally pulled up so MCU pin output should be pulled down to reset
void gpioGpsResetSetOn()
{
	GPIO_PinOutSet(GPSRESET_port, GPSRESET_pin);
}
void gpioGpsResetSetOff()
{
	GPIO_PinOutClear(GPSRESET_port, GPSRESET_port);
}
// EXTINT pin should be left open if unused
void gpioGpsExtIntSetOn()
{
	GPIO_PinOutSet(GPSEXTINT_port,GPSEXTINT_pin);
}
void gpioGpsExtIntSetOff()
{
	GPIO_PinOutClear(GPSEXTINT_port,GPSEXTINT_port);
}

// nRESET pin is active low so MCU pin output should be pulled down to reset
void gpioImuResetSetOn()
{
	GPIO_PinOutSet(IMURESET_port,IMURESET_pin);
}
void gpioImuResetSetOff()
{
	GPIO_PinOutClear(IMURESET_port,IMURESET_port);
}
// EXTINT pin is read only
void gpioGpsExtIntRead()
{
	GPIO_PinInGet(IMUEXTINT_port,IMUEXTINT_pin);
}

/*
 * disable the I2C0 BME280 SDA
 */
void bmeSDADisable()
{
	GPIO_PinOutClear(SDABME_port,SDABME_pin);
}
/*
 * disable the I2C0 BME280 SCL
 */
void bmeSCLDisable()
{
	GPIO_PinOutClear(SCLBME_port,SCLBME_pin);
}

/***************************************************************************//**
 * This is a callback function that is invoked each time a GPIO interrupt
 * in the IMU occurs. Pin number is passed as parameter.
 *
 * @param[in] pin  Pin number where interrupt occurs
 *
 * @note This function is called from ISR context and therefore it is
 *       not possible to call any BGAPI functions directly. The button state
 *       change is signaled to the application using gecko_external_signal()
 *       that will generate an event gecko_evt_system_external_signal_id
 *       which is then handled in the main loop.
 ******************************************************************************/
void bnoInterrupt(uint8_t pin)
{
	gpioLedDbgSetOn();
	if (GPIO_PinInGet(IMUEXTINT_port, IMUEXTINT_pin) == 1) {
//		BNO055ResetInt();
		gpioLedDbgSetOn();
	}
}

void bnoEnableInterrupts()
{
	GPIOINT_Init();

	GPIO_ExtIntConfig(IMUEXTINT_port, IMUEXTINT_pin, IMUEXTINT_pin, true, false, true);

	GPIOINT_CallbackRegister(IMUEXTINT_pin, bnoInterrupt);
}

void button_interrupt(uint8_t pin){
	switch(pin){
		case PB0_pin:
			if (GPIO_PinInGet(PB0_port, PB0_pin) == 1) {
//				gpioLedDbgSetOn();
			    gecko_external_signal(PB_PAGE2);
			}
		break;
		case PB1_pin:
			if (GPIO_PinInGet(PB1_port, PB1_pin) == 1) {
//				gpioLedDbgSetOff();
			    gecko_external_signal(PB_PAGE1);
			}
		break;
	}
}
/*******************************************************************************
 * Enable button interrupts for PB0, PB1. Both GPIOs are configured to trigger
 * an interrupt on the rising edge (button released).
 ******************************************************************************/
void enable_button_interrupts(void){
  GPIOINT_Init();

  /* configure interrupt for PB0 and PB1, rising edges */
  GPIO_ExtIntConfig(PB0_port, PB0_pin, PB0_pin,
                    true, false, true);
  GPIO_ExtIntConfig(PB1_port, PB1_pin, PB1_pin,
                    true, false, true);


  /* register the callback function that is invoked when interrupt occurs */
  GPIOINT_CallbackRegister(PB0_pin, button_interrupt);
  GPIOINT_CallbackRegister(PB1_pin, button_interrupt);

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
