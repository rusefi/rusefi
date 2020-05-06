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
#define AFR_MULTIPLIER 147

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
	float warmup;
	sensor_error_code_t error_code;
} sensor_data_t;

sensor_data_t innovate_o2_sensor[NUM_INNOVATE_O2_SENSORS];

void SerialSendTest()
{
	// char data[5]; // = "Hello world!\n\r";
	// data[0] = 178;
	// data[1] = 130;
	// data[2] = 71;
	// data[3] = 19;
	// data[4] = 1;
	// data[5] = 81;
	// sdWrite(&SD6, (uint8_t *)data, 6);
}

uint16_t tmsglen;

void IdentifyInnovateSerialMsg() // this identifies an innovate LC1/LC2 o2 sensor by it's first word (header)
{
	if (CONFIG(enableInnovateLC2))
	{
		if (((ser_buffer[0]) & 162) != 162) //not serial header word
		{
			innovate_serial_id_state = UNKNOWN;
			innovate_msg_len = SERBUFFLEN;
		}

		switch (innovate_serial_id_state)
		{
		case UNKNOWN:
			// read one byte, identify with mask, advance and read next byte
			if (((ser_buffer[0]) & 162) == 162) // check if it's the first byte of header
			{
				// first byte identified, now continue reading and advance statemachine
				innovate_serial_id_state = HEADER_FOUND;
			}
			else
			{
				innovate_serial_id_state = UNKNOWN;
			}
			break;

		case HEADER_FOUND:
			// now we should have both header bytes in array, and we can read the total packet length
			// we could check if both header bytes mask OK: (skip for now)
			//	if ((((ser_buffer[0] << 8) | ser_buffer[1]) & 41600) == 41600) //1010001010000000 mask
			tmsglen = (((ser_buffer[0] << 8) | ser_buffer[1]) & 383); //0000000101111111 mask

			if (tmsglen)
			{
				tmsglen += 1; // length in words including header (2 bytes)
				tmsglen *= 2; // length in bytes (incl header)
				innovate_msg_len = tmsglen;

				innovate_serial_id_state = IDENTIFIED; //advance state machine
			}
			else
			{
				innovate_serial_id_state = UNKNOWN;
			}

			break;

		case IDENTIFIED:
			// serial packet fully identified
			ParseInnovateSerialMsg(); //takes about 570ns
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

	for (size_t i = 0; i < ((tmsglen - 2) / 4) && i < NUM_INNOVATE_O2_SENSORS; i++)
	{
		innovate_o2_sensor[i].function_code = (ser_buffer[2 + i * 4] >> 2 & 0x7);
		// innovate_o2_sensor[i].AFR_multiplier = ((ser_buffer[2 + i * 4] << 7 | ser_buffer[3 + i * 4]) & 0xFF);
		innovate_o2_sensor[i].AFR_multiplier = AFR_MULTIPLIER;

		switch (innovate_o2_sensor[i].function_code)
		{
		case 0: //Lambda valid and aux data valid, normal operation
		case 1: //Lambda value contains o2 level in 1/10%
			innovate_o2_sensor[i].lambda = ((ser_buffer[4 + i * 4] << 7 | ser_buffer[5 + i * 4]) & 0x1FFF);
			raw_afr = ((innovate_o2_sensor[i].lambda + 500) * innovate_o2_sensor[i].AFR_multiplier);
			if (innovate_o2_sensor[i].function_code) //case 1
				innovate_o2_sensor[i].AFR = raw_afr * 0.001;
			else // case 0
				innovate_o2_sensor[i].AFR = raw_afr * 0.0001;

			if (innovate_o2_sensor[i].AFR > AFRMAX)
				innovate_o2_sensor[i].AFR = AFRMAX;
			else if (innovate_o2_sensor[i].AFR < AFRMIN)
				innovate_o2_sensor[i].AFR = AFRMIN;

			InnovateLC2AFR = innovate_o2_sensor[i].AFR; //only using one sensor right now

			break;
		// this is invalid o2 data, so we can ignore it:
		//  case 2: // Free air Calib in progress, Lambda data not valid
		//    break;
		//  case 3: // Need Free air Calibration Request, Lambda data not valid
		//    break;
		case 4: // Warming up, Lambda value is temp in 1/10% of operating temp
			InnovateLC2AFR = AFR_DEFAULT;
			innovate_o2_sensor[i].warmup = ((ser_buffer[4 + i * 4] << 7 | ser_buffer[5 + i * 4]) & 0x1FFF);
			//catch potential overflow:
			if (innovate_o2_sensor[i].warmup >= 1023)
				innovate_o2_sensor[i].warmup = 1023;
			else if (innovate_o2_sensor[i].warmup <= 0)
				innovate_o2_sensor[i].warmup = 0;
			break;
			//  case 5: // Heater Calibration, Lambda value contains calibration countdown
			//    break;
		case 6: // Error code in Lambda value
			InnovateLC2AFR = AFR_DEFAULT;
			innovate_o2_sensor[i].error_code = (sensor_error_code_t)((ser_buffer[4 + i * 4] << 7 | ser_buffer[5 + i * 4]) & 0x1FFF);
			//catch potential overflow:
			if (innovate_o2_sensor[i].error_code >= (sensor_error_code_t)1023)
				innovate_o2_sensor[i].error_code = (sensor_error_code_t)1023;
			else if (innovate_o2_sensor[i].error_code <= 0)
				innovate_o2_sensor[i].error_code = (sensor_error_code_t)0;
			break;
			//  case 7: // reserved
			//    break;
		default:
			break;
		}
	}
}

void ParseSerialData()
// static void ParseSerialData(UARTDriver *uartp)
{
	if (CONFIG(enableInnovateLC2))
		IdentifyInnovateSerialMsg();
}

#endif