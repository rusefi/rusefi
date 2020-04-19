/**
 * @file	serial_sensor.cpp
 *
 *
 * @date Mar 19, 2020
 * @author Konstantin Smola, (c) 2020
 */

#include "global.h"
#if EFI_AUX_SERIAL
#include "serial.h"
#include "serial_sensor.h"
#include "engine.h"

#define NUM_INNOVATE_O2_SENSORS 1
#define AFR_MULTIPLIER	147

EXTERN_ENGINE;

volatile float InnovateLC2AFR = 0;

typedef enum
{
	NO_ERROR = 0,
	HEATER_SHORTED = 1,
	HEATER_OPEN = 2,
	PUMPCELL_SHORTED = 3,
	PUMPCELL_OPEN = 4,
	REFCELL_SHORTED = 5,
	REFCELL_OPEN = 6,
	SYSTEM_ERROR = 7,
	SENSOR_TIMING_ERR = 8,
	SUPP_V_LOW = 9
} sensor_error_code_t;
typedef struct
{
	int function_code;
	float AFR;
	float AFR_multiplier;
	float lambda;
	int warmup;
	sensor_error_code_t error_code;
} sensor_data_t;

sensor_data_t innovate_o2_sensor[NUM_INNOVATE_O2_SENSORS-1]; 

void SerialSendTest()
{
	char data[5]; // = "Hello world!\n\r";
	data[0] = 178;
	data[1] = 130;
	data[2] = 91;
	data[3] = 19;
	data[4] = 0;
	data[5] = 2;
	sdWrite(&SD6, (uint8_t *)data, 6);
}

uint16_t tmsglen;

void IdentifyInnovateSerialMsg() // this identifies an innovate LC1/LC2 o2 sensor by it's first word (header)
{
	if (CONFIG(enableInnovateLC2))
	{
		if (((ser_buffer[0]) & 162) != 162) //not serial header word
			innovate_serial_id_state = UNKNOWN;

		switch (innovate_serial_id_state)
		{
		case UNKNOWN:
			// read one byte, identify with mask, advance and read next byte
			if (((ser_buffer[0]) & 162) == 162) // check if it's the first byte of header
			{
				// first byte identified, now read second byte and advance statemachine
				// advance statemachine prior, since it's irq driven
				innovate_serial_id_state = HEADER_FOUND;
				innovate_start_byte = 1;
			}
			else
			{
				innovate_start_byte = 0;
				innovate_msg_len = 1;
			}
			break;

		case HEADER_FOUND:
			// now we should have both header bytes in array, and we can read the total packet length
			// we could check if both header bytes mask OK: (skip for now)
			//	if ((((ser_buffer[0] << 8) | ser_buffer[1]) & 41600) == 41600) //1010001010000000 mask
			tmsglen = (((ser_buffer[0] << 8) | ser_buffer[1]) & 383) + 1; //0000000101111111 mask
			// this is the length in words including header (2 bytes)
			tmsglen *= 2; // this is length in bytes (incl header)
			//advance state machine and read rest of package
			innovate_serial_id_state = IDENTIFIED;
			innovate_start_byte = 2;
			innovate_msg_len = tmsglen - 2;
			break;

		case IDENTIFIED:
			// serial packet fully identified

			// HAL_GPIO_TogglePin(GPIOC, SERIAL_Pin);
			ParseInnovateSerialMsg();
			innovate_start_byte = 0; // now we can read entire packet
			innovate_msg_len = tmsglen;
			break;

		default:
			break;
		}
	}
}

void ParseInnovateSerialMsg()
{
	float raw_afr;
	//get error code and afr

	// 000 Lambda valid and Aux data valid, normal operation.
	// 001 Lambda value contains O2 level in 1/10%
	// 010 Free air Calib in progress, Lambda data not valid
	// 011 Need Free air Calibration Request, Lambda data not valid
	// 100 Warming up, Lambda value is temp in 1/10% of operating temp.
	// 101 Heater Calibration, Lambda value contains calibration countdown.
	// 110 Error code in Lambda value
	// 111 reserved

	for (size_t i = 0; i < ((innovate_msg_len) / 4) && i < NUM_INNOVATE_O2_SENSORS - 1; i++)
	{
		innovate_o2_sensor[i].function_code = (ser_buffer[2 + i * 4] >> 2 & 0x7);
		//catch potential overflow:
		if (innovate_o2_sensor[i].function_code >= 1023)
			innovate_o2_sensor[i].function_code = 1023;
		else if (innovate_o2_sensor[i].function_code <= 0)
			innovate_o2_sensor[i].function_code = 0;

		// innovate_o2_sensor[i].AFR_multiplier = ((ser_buffer[2 + i * 4] << 7 | ser_buffer[3 + i * 4]) & 0xFF);
		innovate_o2_sensor[i].AFR_multiplier = AFR_MULTIPLIER;

		switch (innovate_o2_sensor[i].function_code)
		{
		case 0: //Lambda valid and aux data valid, normal operation
		case 1: //Lambda value contains o2 level in 1/10%
			innovate_o2_sensor[i].lambda = ((ser_buffer[4 + i * 4] << 7 | ser_buffer[5 + i * 4]) & 0x1FFF);
			raw_afr = ((innovate_o2_sensor[i].lambda + 500) * innovate_o2_sensor[i].AFR_multiplier);
			if (innovate_o2_sensor[i].function_code)
				innovate_o2_sensor[i].AFR = raw_afr * 0.001;
			else
				innovate_o2_sensor[i].AFR = raw_afr * 0.0001;
			InnovateLC2AFR = innovate_o2_sensor[i].AFR;
			break;
		// this is invalid o2 data, so we can ignore it:
		//  case 2: // Free air Calib in progress, Lambda data not valid
		//    innovate_o2_sensor.AFR = 7.7;
		//    break;
		//  case 3: // Need Free air Calibration Request, Lambda data not valid
		//    innovate_o2_sensor.AFR = 7.8;
		//    break;
		case 4: // Warming up, Lambda value is temp in 1/10% of operating temp
			innovate_o2_sensor[i].warmup = ((ser_buffer[4 + i * 4] << 7 | ser_buffer[5 + i * 4]) & 0x1FFF);
			//catch potential overflow:
			if (innovate_o2_sensor[i].warmup >= 1023)
				innovate_o2_sensor[i].warmup = 1023;
			else if (innovate_o2_sensor[i].warmup <= 0)
				innovate_o2_sensor[i].warmup = 0;
			break;
			//  case 5: // Heater Calibration, Lambda value contains calibration countdown
			//    innovate_o2_sensor.AFR = 8.0;
			//    break;
		case 6: // Error code in Lambda value
			innovate_o2_sensor[i].error_code = (sensor_error_code_t)((ser_buffer[4 + i * 4] << 7 | ser_buffer[5 + i * 4]) & 0x1FFF);
			//catch potential overflow:
			if (innovate_o2_sensor[i].error_code >= (sensor_error_code_t)1023)
				innovate_o2_sensor[i].error_code = (sensor_error_code_t)1023;
			else if (innovate_o2_sensor[i].error_code <= 0)
				innovate_o2_sensor[i].error_code = (sensor_error_code_t)0;
			break;
			//  case 7: // reserved
			//    innovate_o2_sensor.AFR = 8.2;
			//    break;
		default:
			break;
		}
	}
}

void ParseSerialData()
{
	if (CONFIG(enableInnovateLC2))
		IdentifyInnovateSerialMsg();
}

#endif