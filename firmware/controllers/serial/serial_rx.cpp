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

uint8_t ser_buffer[SERBUFFLEN] = {};
uint16_t innovate_msg_len = SERBUFFLEN;
innovate_serial_id_state_t innovate_serial_id_state = UNKNOWN;

SerialRead::SerialRead()
	: ThreadController("AUX Serial RX", NORMALPRIO) {
}

void SerialRead::ThreadTask() {
	while (true) {
		if (CONFIG(enableInnovateLC2)) {
			len = innovate_msg_len;
		}

		if ((sdReadTimeout(AUX_SERIAL_PORT, ser_buffer, len, TIMEREAD)) > 0)
			ParseSerialData();
	}
}

#endif // EFI_AUX_SERIAL
