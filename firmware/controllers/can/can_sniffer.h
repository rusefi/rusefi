/**
 * @file	can_sniffer.h
 *
 * Implementation of slcan protocol
 *
 * @date Jul 24, 2026
 * @author Andrey Gusakov
 */

#pragma once

#include "can_rx.h"
#include "thread_controller.h"

#ifndef CAN_SNIFFER
#define CAN_SNIFFER TRUE
#endif

class CanSniffer final : protected ThreadController<UTILITY_THREAD_STACK_SIZE> {
public:
	CanSniffer(SerialUSBDriver& driver)
		: ThreadController("CAN Sniffer", PRIO_CAN_SNIFFER), m_channel(reinterpret_cast<BaseChannel*>(&driver))
	{
	}

	using ThreadController::start;
	using ThreadController::stop;

	void ThreadTask() override;

	void handle_can_message(const size_t busIndex, const CANRxFrame &cmsg, efitick_t nowNt);

private:
	typedef enum
	{
		can_mode_close,
		can_mode_normal,
		can_mode_listen,
		can_mode_loopback,
	} slcan_can_mode_e;

	bool can_init(slcan_can_mode_e mode);
	uint8_t slcan_get_status();
	void execute_status_command();
	bool send_can_message_from_string(const char * str);

	char *put_hex_digit(char * str, uint8_t val);
	char *put_hex_byte(char * str, uint8_t val);
	uint8_t read_hex_number_8(const char * str, uint8_t len);
	uint32_t read_hex_number(const char * str, uint8_t len);

	size_t readLine();
	void putstr(const char * s);

	void executeCommand();

	BaseChannel* const m_channel;
	char line[64];

	uint8_t terminal_open:1;
	uint8_t transmit_enabled:1;
	uint8_t baudrate_configured:1;
	uint8_t ts:1;
	uint8_t baud;
};
