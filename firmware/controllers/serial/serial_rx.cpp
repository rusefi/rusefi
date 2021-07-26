/**
 * @file	serial_rx.cpp
 *
 * This file handles auxilery serial communication.
 *
 * @date Mar 19, 2020
 * @author Konstantin Smola, (c) 2020
 */

#include "pch.h"

#if EFI_AUX_SERIAL
#include "serial.h"
#include "serial_hw.h"
#include "serial_sensor.h"

#include "vehicle_speed.h"
#include "thread_priority.h"

uint8_t ser_buffer[SERBUFFLEN] = {};
size_t innovate_msg_len = 1;
innovate_serial_id_state_t innovate_serial_id_state = UNKNOWN;
uint8_t sb = 0;
bool clear_ser_buffer = false;

SerialRead::SerialRead()
	: ThreadController("AUX Serial RX", PRIO_AUX_SERIAL) {
}

void SerialRead::ThreadTask() {
	while (true) {
		if (CONFIG(enableInnovateLC2)) {
			len = innovate_msg_len;
		}

        if (len >= SERBUFFLEN)
		  len = SERBUFFLEN;

		if (sdReadTimeout(AUX_SERIAL_DEVICE,  &ser_buffer[sb], len, TIME_100MSEC) == len) {
			ParseSerialData();
		} else {
			ResetSerialSensor();
		}
		
		//clear buffer every frame to avoid parsing old data
		if (clear_ser_buffer) {
			ClearSerialBuffer();
			clear_ser_buffer = false;
		}
	}
}

#endif // EFI_AUX_SERIAL
