/**
 * @file	serial.h
 *
 * @date Mar 19, 2020
 * @author Konstantin Smola, (c) 2020
 */

#pragma once

#include "hal.h"

#include "periodic_thread_controller.h"

#define TIMEREAD ((sysinterval_t)chTimeMS2I(10)) //10ms timeout

#define SERBUFFLEN 64
#define AUX_SERIAL_PORT (&SD6)

class Logging;

typedef enum {UNKNOWN, HEADER_FOUND, IDENTIFIED} innovate_serial_id_state_t;

extern uint8_t ser_buffer[SERBUFFLEN]; //buffer for incoming serial data
extern uint16_t innovate_msg_len;
extern innovate_serial_id_state_t innovate_serial_id_state;

class SerialWrite final : public PeriodicController<512> {
public:
	SerialWrite();
	void PeriodicTask(efitime_t nowNt) override;
};

class SerialRead final : public ThreadController<256> {
public:
	SerialRead();
	void ThreadTask();

	uint16_t len = SERBUFFLEN;

private:
};