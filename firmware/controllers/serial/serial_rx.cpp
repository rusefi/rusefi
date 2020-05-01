/**
 * @file	serial_rx.cpp
 *
 * This file handles auxilery serial communication.
 *
 * @date Mar 19, 2020
 * @author Konstantin Smola, (c) 2020
 */

#include "globalaccess.h"

#if EFI_AUX_SERIAL
#include "engine.h"
#include "serial.h"
#include "serial_hw.h"
#include "serial_sensor.h"

#include "allsensors.h"
#include "vehicle_speed.h"

EXTERN_ENGINE;

static LoggingWithStorage logger("AUX Serial RX");

uint8_t ser_buffer[18] = {};
uint16_t innovate_msg_len = 1;
uint16_t innovate_start_byte = 0;

innovate_serial_id_state_t innovate_serial_id_state = UNKNOWN;

size_t len = 0;
uint16_t sb = 0; 

#define ONE_SEC TIME_MS2I(1000)

SerialRead::SerialRead()
	: ThreadController("AUX Serial RX", NORMALPRIO)
{
}

void SerialRead::ThreadTask()
{
	while (true)
	{
		if (CONFIG(enableInnovateLC2))
		{
			len = (size_t)innovate_msg_len;
			sb = innovate_start_byte;
		}

		// This should be a blocking read (TIME_INFINITE)
		// if ((sdReadTimeout(&SD6, &ser_buffer[sb], len, TIME_INFINITE)) == len)
		// {
		// 	ParseSerialData();
		// }

		// uartStartReceive(&UARTD6, len, &ser_buffer[sb]);
		// if (uartReceiveTimeout(&UARTD6, &len, &ser_buffer[sb], ONE_SEC))
		// {
		// 	ParseSerialData();
		// }

		// if ((uartStartReceive(&SD6, len, &ser_buffer[sb])) == len)
		// {
		// 	ParseSerialData();
		// }

	}
}

#endif // EFI_AUX_SERIAL
