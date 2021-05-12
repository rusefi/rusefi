/**
 * @file	serial.h
 *
 * @date Mar 19, 2020
 * @author Konstantin Smola, (c) 2020
 */

#pragma once

#include "hal.h"

#include "periodic_thread_controller.h"

#define TIME_100MSEC ((sysinterval_t)chTimeMS2I(100))

#define SERBUFFLEN 64

constexpr uint8_t lc2_header_mask = 162;
constexpr uint16_t lc2_pcklen_mask = 383;

typedef enum {UNKNOWN, HEADER_FOUND, IDENTIFIED} innovate_serial_id_state_t;

extern uint8_t ser_buffer[SERBUFFLEN]; //buffer for incoming serial data
extern size_t innovate_msg_len;
extern innovate_serial_id_state_t innovate_serial_id_state;
extern uint8_t sb;
extern bool clear_ser_buffer;

class SerialRead final : public ThreadController<256> {
public:
	SerialRead();
	void ThreadTask();

	uint16_t len;
};