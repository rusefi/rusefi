/**
 * @file	can_sniffer.cpp
 *
 * Implementation of slcan protocol
 * Inspired by https://github.com/smartgauges/ucds
 *
 * @date Jul 24, 2026
 * @author Andrey Gusakov
 */

#include "pch.h"

#include "can_sniffer.h"

#if CAN_SNIFFER
#include "can_msg_tx.h"

void CanSniffer::ThreadTask() {
	while (!chThdShouldTerminateX()) {
		size_t len = readLine();
		if (len > 0) {
			executeCommand();
		}
	}

	chThdExit((msg_t)0x0);
}

// puts one ascii hex digit (0-9, A-F) to a string for val in range
// 0-15, returns a pointer to the next position in the string
char * CanSniffer::put_hex_digit(char * str, uint8_t val){
	char c;
	val &= 0x0f;
	if(val < 10){
		c = val + '0';
	}else{
		c = val + 'A' - 10;
	}
	*str++ = c;
	return str;
}

// puts a byte as ascii hex to a string and returns a pointer
// to the nex position in the string.
char * CanSniffer::put_hex_byte(char * str, uint8_t val)
{
	str = put_hex_digit(str, val >> 4);
	str = put_hex_digit(str, val);
	return str;
}

uint8_t CanSniffer::read_hex_number_8(const char * str, uint8_t len)
{
	uint8_t d = 0;
	while(len--){
		d <<= 4;
		uint8_t v = 0;
		if((*str >= '0') && (*str <= '9')) v = *str - '0';
		else if((*str >= 'A') && (*str <= 'F')) v = *str - 'A' + 10;
		else if((*str >= 'a') && (*str <= 'f')) v = *str - 'a' + 10;

		d |= v;
		str++;
	}
	return d;
}

uint32_t CanSniffer::read_hex_number(const char * str, uint8_t len)
{
	uint32_t d = 0;
	while(len--){
		d <<= 4;
		uint8_t v = 0;
		if((*str >= '0') && (*str <= '9')) v = *str - '0';
		else if((*str >= 'A') && (*str <= 'F')) v = *str - 'A' + 10;
		else if((*str >= 'a') && (*str <= 'f')) v = *str - 'a' + 10;

		d |= v;
		str++;
	}
	return d;
}

void CanSniffer::handle_can_message(const size_t busIndex, const CANRxFrame &cmsg, efitick_t nowNt)
{
	/*
	if (!m_started)
		return;
	*/
	if (!terminal_open)
		return;
	// TODO: check busIndex?


	//       cmd  id  dlc data   \r  0
	char buf[ 1  + 8 + 1 + 8*2 + 2*2 + 1 + 1];
	char * str = buf;

	uint32_t id = CAN_ID(cmsg);

	if (CAN_ISX(cmsg)) { // standard identifier
		*str++ = CAN_ISRTR(cmsg) ? 'r':'t';
		str = put_hex_digit(str, id >> 8);
		str = put_hex_byte(str, id & 0xff);
	}
	else{ // extended identifier
		*str++ = CAN_ISRTR(cmsg) ? 'R':'T';
		str = put_hex_byte(str, (id >> 24) & 0xff);
		str = put_hex_byte(str, (id >> 16) & 0xff);
		str = put_hex_byte(str, (id >>  8) & 0xff);
		str = put_hex_byte(str, (id >>  0) & 0xff);
	}

	str = put_hex_digit(str, cmsg.DLC);

	if (!CAN_ISRTR(cmsg)){ // no data on RTR
		for(uint8_t i = 0; i < cmsg.DLC; i++){
			str = put_hex_byte(str, cmsg.data8[i]);
		}
	}

	// TODO:
/*
	if (ts) {
		uint16_t ts = tick.msec;
		str = put_hex_byte(str, (ts >> 8) & 0xff);
		str = put_hex_byte(str, (ts >> 0) & 0xff);
	}
*/

	*str++ = '\r';
	*str++ = 0;

	putstr(buf);
}

bool CanSniffer::can_init(slcan_can_mode_e mode) {
	switch (mode) {
	case can_mode_close:
		efiPrintf("sniffer wants to close CAN");
		break;
	case can_mode_normal:
		// TODO: return error if baud is not equal to what ECU alrady set?
		efiPrintf("sniffer wants to open CAN with baud %d", baud);
		break;
	case can_mode_listen:
	case can_mode_loopback:
		break;
	}

	return true;
}

uint8_t CanSniffer::slcan_get_status() {
	uint8_t s = 0;

	// TODO:
	//s |= slcan->fifo_rx_full ? 0x01 : 0;
	//s |= slcan->fifo_tx_full ? 0x02 : 0;

	return s;
}

void CanSniffer::execute_status_command() {
	uint8_t s = slcan_get_status();

	char buf[6];
	char * str = buf;

	*str++ = 'F';
	str = put_hex_byte(str, s);
	*str++ = '\r';
	*str++ = 0;

	putstr(buf);
}

bool CanSniffer::send_can_message_from_string(const char *str) {
	char cmd = *str++; // command char
	bool IDE = cmd == 'T' || cmd == 'R'; // upercase means EID
	bool RTR = cmd == 'r' || cmd == 'R'; // the upper or lowercase r means RTR

	/* TODO: support RTR */
	if (RTR)
		return false;

	uint32_t id;
	uint8_t dlc;

	if(!IDE){ //standard identifier
		id = read_hex_number(str, 3);
		str += 3;
		if(id > 0x7ff) return false;
	}else{
		id = read_hex_number(str, 8);
		str += 8;
		if(id > 0x1fffffff) return false;
	}

	dlc = read_hex_number_8(str, 1);
	str += 1;
	if(dlc > 8)
		return false;

	CanTxMessage cmsg(CanCategory::SNIFFER, id, dlc, /*bus*/0, /*isExtended*/IDE);

	if (!RTR) {
		for (uint8_t i = 0; i < dlc; i++) {
			cmsg[i] = read_hex_number_8(str, 2);
			str += 2;
		}
	}

	return true;
}

size_t CanSniffer::readLine() {
	size_t offset = 0;

	do {
		size_t transferred = chnReadTimeout(m_channel, (uint8_t *)&line[offset], 1, TIME_MS2I(100));
		// timeout?
		if (transferred == 0) {
			return 0;
		}

		if (line[offset] == '\r') {
			line[offset + 1] = '\0';
			return offset;
		}

		offset++;
		if (offset >= sizeof(line) - 2) {
			// no space left for \r\0 - overflow
			return 0;
		}
	} while (1);

	return 0;
}

void CanSniffer::executeCommand() {
	char *str = line;

	switch(str[0]) {
		case 't': // transmit standard ID messages
		case 'r':
			if (transmit_enabled && send_can_message_from_string(str)) {
				putstr("z\r");
				//putstr(loopback, str);
			}
			else {
				putstr("\a"); // bell
			}
			break;

		case 'T': // transmit extended ID messages
		case 'R':
			if (transmit_enabled && send_can_message_from_string(str)) {
				putstr("Z\r");
			}
			else {
				putstr("\a"); // bell
			}
			break;

		case 'O': //open terminal
			if (baudrate_configured && !terminal_open && can_init(can_mode_normal)) {
				terminal_open = 1;
				transmit_enabled = 1;
				putstr("\r");
			}
			else {
				putstr("\a"); // bell
			}
			break;

		case 'L': // open terminal listen only
			if (baudrate_configured && !terminal_open && can_init(can_mode_listen)) {
				terminal_open = 1;
				transmit_enabled = 0;
				putstr("\r");
			}
			else {
				putstr("\a"); // bell
			}
			break;

		case 'l': // (lower case L) open terminal in loopback mode (non standard feature)
			if (baudrate_configured && !terminal_open && can_init(can_mode_loopback)) {
				terminal_open = 1;
				transmit_enabled = 1;
				putstr("\r");
			}
			else {
				putstr("\a"); // bell
			}
			break;

		case 'C': //close terminal
			if (terminal_open) {
				terminal_open = 0;
				transmit_enabled = 0;
				baud = 0;
				ts = 0;
				baudrate_configured = 0;
				//can_init(can_mode_close);
				putstr("\r");
			}
			else {
				putstr("\a"); // bell
			}
			break;

		case 'F': // request status
			execute_status_command();
			break;

		case 'S': //select bitrate e.g. "S4" for 125kbit
			if (terminal_open || str[1] < '0' || str[1] > '8') {
				// any other character is also error
				// or terminal is open, we cannot set a baud rate
				putstr("\a"); // bell
			}
			else {
				baudrate_configured = 1;
				baud = str[1] - '0';
				putstr("\r");
			}
			break;

		case 'U': //setup uart baudrate
			putstr("\r");
			break;

		case 'P': //single poll
			putstr("\a"); // bell
			break;

		case 'A': //auto poll
			putstr("\a"); // bell
			break;

		case 'X': //set auto poll/send
			putstr("\r");
			break;

		case 'W': //filter
			putstr("\a"); // bell
			break;

		case 'Z': //timestamp
			if (terminal_open || (str[1] != '0' && str[1] != '1')) {
				putstr("\a"); // bell
			}
			else {
				ts = str[1] - '0';
				putstr("\r");
			}
			break;

		case 'v'://version
		case 'V'://version
			putstr("V1220\r");
			break;

		case 'N'://serial number
			putstr("N1234\r");
			break;
	}
}

void CanSniffer::putstr(const char * s)
{
	size_t l = strlen(s);

	chnWriteTimeout(m_channel, (uint8_t *)s, l, TIME_MS2I(100));
}

#endif