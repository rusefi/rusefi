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

#define TIMEREAD ((sysinterval_t)chTimeMS2I(100)) //10ms timeout
EXTERN_ENGINE;

static LoggingWithStorage logger("AUX Serial RX");

uint8_t ser_buffer[SERBUFFLEN] = {};
uint16_t innovate_msg_len = SERBUFFLEN;
uint16_t innovate_start_byte = 0;

innovate_serial_id_state_t innovate_serial_id_state = UNKNOWN;

uint16_t len = SERBUFFLEN;
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
			// len = innovate_msg_len;
			// sb = innovate_start_byte;
		}

		// This should be a blocking read (TIME_INFINITE), but it isn't
		// if ((sdReadTimeout(&SD6, &ser_buffer[sb], len, TIMEREAD)) > 0)
		// if (sdAsynchronousRead(&SD6, ser_buffer, len) == len)
		// if (sdRead(&SD6, ser_buffer, len) == len)
		if ((sdReadTimeout(&SD6, ser_buffer, len, TIMEREAD)) > 0)
		{
			if (ser_buffer[1] == 178)
			{
				ser_buffer[1] = 130;
				palTogglePad(GPIOG, 13);
				// should never be the case
			}
			ParseSerialData();
		}

		// std::fill_n(ser_buffer, SERBUFFLEN, 0);

	}
}

#endif // EFI_AUX_SERIAL
