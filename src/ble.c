/*
 * @File   : ble.c
 * @brief  : source file containing function definition for BLE
 * @Author : Madhukar Arora
 * @References : Used Silicon Labs Example Provided in Class Lecture 10 as reference.
 *
 */


#include "ble.h"
#include "display.h"
#include "log.h"

#define TICKS_PER_SECOND    (32768)

void measure_navigation(char gpsarr[])
{
	uint8_t ln_buffer[28]; /* Stores the location and navigation data in the Location and Navigation (LN) format. */
	uint16_t flags = 0x05;   /* LN Flag set to bit 1 for instantaneous speed and bit 2 for location present*/

	char cheader[7], cutctime[10], clatitude[11], clongitude[12], cgspeed[5] = {'\0'};
//	strncpy(cheader, gpsarr, 6);
//	strncpy(cutctime, &gpsarr[7], 9);
	strncpy(clatitude, &gpsarr[19], 10);
	strncpy(clongitude, &gpsarr[32], 11);
	strncpy(cgspeed, &gpsarr[46], 4);
//	displayPrintf(DISPLAY_ROW_BTADDR,"Header:%s", cheader);
//	displayPrintf(DISPLAY_ROW_BTADDR2,"UTC:%s", cutctime);
//	displayPrintf(DISPLAY_ROW_CONNECTION,"Lat:%s", clatitude);
//	displayPrintf(DISPLAY_ROW_PASSKEY,"Lon:%s", clongitude);
//	displayPrintf(DISPLAY_ROW_ACTION,"Spd:%s", cgspeed);

	// convert strings to floats
	float flon = strtof(clongitude, NULL) * 1000;
	float flat = strtof(clatitude, NULL) * 1000;
	float fgspeed = strtof(cgspeed, NULL) * 100;

	uint16_t groundspeed = (uint16_t)(FLT_TO_UINT32(fgspeed, 0) >> 1);	// convert speed in knots to m/s. units is 1/100 of a m/s
	int32_t longitude = FLT_TO_UINT32(flon, 0);   // Stores the longitude data read from the sensor in the correct format
	int32_t latitude = FLT_TO_UINT32(flat, 0);  // Stores the latitude data read from the sensor in the correct format
	uint8_t *p = ln_buffer; /* Pointer to LN buffer needed for converting values to bitstream. */

	/* Convert flags to bitstream and append them in the LN data buffer (ln_buffer) */
	UINT16_TO_BITSTREAM(p, flags);

	/* Convert temperature to bitstream and place it in the ES Pressure data buffer (es_pressure_buffer) */
	UINT16_TO_BITSTREAM(p, groundspeed);
	UINT32_TO_BITSTREAM(p, latitude);	// issue with sign conversion
	UINT32_TO_BITSTREAM(p, longitude);	// issue with sign conversion

	/* Send indication of the temperature in ln_buffer to all "listening" clients.
	 * This enables the Location and Navigation in the EFR Connect app to display the temperature.
	 *  0xFF as connection ID will send indications to all connections. */
	gecko_cmd_gatt_server_send_characteristic_notification(
			0xFF, gattdb_location_and_speed, 12, ln_buffer);
}

void measure_pressure(float pressPa)
{
	uint8_t es_pressure_buffer[4]; /* Stores the pressure data in the Environmental Sensing (ES) format. */

	uint32_t pressure;   /* Stores the pressure data read from the sensor in the correct format */
	uint8_t *p = es_pressure_buffer; /* Pointer to ES pressure buffer needed for converting values to bitstream. */

	/* Convert sensor data to correct pressure format (resolution of 0.1 Pa) */
	pressure = FLT_TO_UINT32(pressPa * 10, 0);
	/* Convert temperature to bitstream and place it in the ES Pressure data buffer (es_pressure_buffer) */
	UINT32_TO_BITSTREAM(p, pressure);

	/* Send indication of the temperature in es_pressure_buffer to all "listening" clients.
	 * This enables the Environmental Sensing in the EFR Connect app to display the temperature.
	 *  0xFF as connection ID will send indications to all connections. */
	gecko_cmd_gatt_server_send_characteristic_notification(
			0xFF, gattdb_pressure, 4, es_pressure_buffer);
}

void measure_temperature(float tempC)
{
	uint8_t htm_temperature_buffer[5]; /* Stores the temperature data in the Health Thermometer (HTM) format. */
	uint8_t flags = 0x00;   /* HTM flags set as 0 for Celsius, no time stamp and no temperature type. */

	uint32_t temperature;   /* Stores the temperature data read from the sensor in the correct format */
	uint8_t *p = htm_temperature_buffer; /* Pointer to HTM temperature buffer needed for converting values to bitstream. */

	/* Convert flags to bitstream and append them in the HTM temperature data buffer (htm_temperature_buffer) */
	UINT8_TO_BITSTREAM(p, flags);

	/* Convert sensor data to correct temperature format */
	temperature = FLT_TO_UINT32(tempC * 1000, -3);
	/* Convert temperature to bitstream and place it in the HTM temperature data buffer (htm_temperature_buffer) */
	UINT32_TO_BITSTREAM(p, temperature);

	/* Send indication of the temperature in htm_temperature_buffer to all "listening" clients.
	 * This enables the Health Thermometer in the EFR Connect app to display the temperature.
	 *  0xFF as connection ID will send indications to all connections. */
	gecko_cmd_gatt_server_send_characteristic_notification(
			0xFF, gattdb_temperature_measurement, 5, htm_temperature_buffer);
}




void ble_EventHandler(struct gecko_cmd_packet* evt){

	static uint8_t connection_handle = 0;
	int rssi = 0;


	/* Handle events */
	switch (BGLIB_MSG_ID(evt->header)) {


	case gecko_evt_system_boot_id: /*indicates device has started and radio is ready*/

		/* Set advertising parameters.
		 * Advertising minimum and max to 250 ms.
		 * 250 * 1.6 = 400ms
		 */
		gecko_cmd_le_gap_set_advertise_timing(0, 400, 400, 0, 0);



		/* Start general advertising and enable connections. */
		gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
		break;

	case gecko_evt_le_connection_opened_id: /*indicates new connection was opened and role of BT Module*/

		LOG_INFO("connection opened\r\n");

		/*EXTRA CREDIT : Enable timer when bluetooth connection opened*/
		letimer0_Init();

		connection_handle = evt->data.evt_le_connection_opened.connection;
		//connection interval 75ms, slave latency 300ms. 1.25ms unit
		gecko_cmd_le_connection_set_parameters(connection_handle,60,60,3,600);
		break;

	case gecko_evt_le_connection_closed_id: /*indicates connection was closed*/

		/*disable timer when connection closed*/
		LETIMER_IntDisable(LETIMER0,LETIMER_IEN_UF);

		connection_handle = 0;

		//force radio to idle state and allow device to sleep
		gecko_cmd_system_halt(1);

		//set global maximum TX power to 0
		gecko_cmd_system_set_tx_power(TX_PWR_0DB);

		//start advertising with specified discoverable and connectable modes
		gecko_cmd_system_halt(0);
		gecko_cmd_le_gap_start_advertising(0,le_gap_general_discoverable,le_gap_connectable_scannable);

		break;

	case gecko_evt_le_connection_rssi_id:/*when RSSI command is completed*/ // most likely will want to remove this since we have little gain
		rssi = evt->data.evt_le_connection_rssi.rssi;

		if(rssi > RSSI_NEG35DB){
			gecko_cmd_system_set_tx_power(TX_PWR_MIN);
		}
		else if(rssi > RSSI_NEG45DB && rssi <= RSSI_NEG35DB){
			gecko_cmd_system_set_tx_power(TX_PWR_NEG20DB);
		}
		else if(rssi > RSSI_NEG55DB && rssi <= RSSI_NEG45DB){
			gecko_cmd_system_set_tx_power(TX_PWR_NEG15DB);
		}
		else if(rssi > RSSI_NEG65DB && rssi <= RSSI_NEG55DB){
			gecko_cmd_system_set_tx_power(TX_PWR_NEG5DB);
		}
		else if(rssi > RSSI_NEG75DB && rssi <= RSSI_NEG65DB){
			gecko_cmd_system_set_tx_power(TX_PWR_0DB);
		}
		else if(rssi > RSSI_NEG85DB && rssi <= RSSI_NEG75DB){
			gecko_cmd_system_set_tx_power(TX_PWR_5DB);
		}
		else{
			gecko_cmd_system_set_tx_power(TX_PWR_MAX);
		}

		break;

	case gecko_evt_system_external_signal_id:/*indicates external signal has been received*/
		// handle external event from IMU
//		handle_external_signal_event(evt->data.evt_system_external_signal.extsignals);
		//	printf("Signal is %x\r\n", signal);
		switch (evt->data.evt_system_external_signal.extsignals){
		case EXT_SIGNAL_IMU_WAKEUP:
//			CORE_DECLARE_IRQ_STATE;
//			CORE_ENTER_CRITICAL();
//			printf("IMU wakeup\r\n");
//			CORE_EXIT_CRITICAL();
			break;
		default:
			break;
		}
		break;
		/* This event is generated when a connected client has either
		 * 1) changed a Characteristic Client Configuration, meaning that they have enabled
		 * or disabled Notifications or Indications, or
		 * 2) sent a confirmation upon a successful reception of the indication. */
	case gecko_evt_gatt_server_characteristic_status_id:
		/* Check that the characteristic in question is temperature - its ID is defined
		 * in gatt.xml as "temperature_measurement". Also check that status_flags = 1, meaning that
		 * the characteristic client configuration was changed (notifications or indications
		 * enabled or disabled). */
		if ((evt->data.evt_gatt_server_characteristic_status.characteristic == (gattdb_temperature_measurement || gattdb_pressure || gattdb_location_and_speed))
				&& (evt->data.evt_gatt_server_characteristic_status.status_flags == gatt_server_client_config)) {
			if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == (gatt_indication || gatt_notification)) {
				/* Indications have been turned ON - start the repeating timer. The 1st parameter '32768'
				 * tells the timer to run for 1 second (32.768 kHz oscillator), the 2nd parameter is
				 * the timer handle and the 3rd parameter '0' tells the timer to repeat continuously until
				 * stopped manually.*/
				gecko_cmd_hardware_set_soft_timer(TICKS_PER_SECOND, 0, 0);
			} else if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == gatt_disable) {
				/* Indications have been turned OFF - stop the timer. */
				gecko_cmd_hardware_set_soft_timer(0, 0, 0);
			}
		}
		if(evt->data.evt_gatt_server_characteristic_status.status_flags == gatt_server_confirmation)
		{
			gecko_cmd_le_connection_get_rssi(evt->data.evt_gatt_server_characteristic_status.connection);
		}
		break;

	case gecko_evt_gatt_server_user_write_request_id:
		if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_digital_out) {
			// write user supplied value to LEDs
			set_leds(evt->data.evt_gatt_server_attribute_value.value.data[0]);
			gecko_cmd_gatt_server_send_user_write_response(evt->data.evt_gatt_server_user_write_request.connection, gattdb_digital_out, bg_err_success);
		}
		break;
	case gecko_evt_gatt_server_user_read_request_id:
	  {
	    uint8_t status_byte = get_leds();
	    gecko_cmd_gatt_server_send_user_read_response(evt->data.evt_gatt_server_user_read_request.connection, gattdb_digital_out, bg_err_success, 1, &status_byte);
	  }
	  break;
	default:
		break;
	}
}


