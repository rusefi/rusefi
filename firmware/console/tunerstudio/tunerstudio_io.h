/**
 * @file	tunerstudio_io.h
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef CONSOLE_TUNERSTUDIO_TUNERSTUDIO_IO_H_
#define CONSOLE_TUNERSTUDIO_TUNERSTUDIO_IO_H_

#include "main.h"

#if EFI_PROD_CODE
#include "usbconsole.h"
#include "pin_repository.h"
#endif

#define PROTOCOL  "001"
#define TS_SIGNATURE "MShift v0.01"

#define TS_RESPONSE_OK 0x00
#define TS_RESPONSE_BURN_OK 0x04
#define TS_RESPONSE_COMMAND_OK 0x07
#define TS_RESPONSE_CRC_FAILURE 0x82

typedef enum {
	TS_PLAIN = 0,
	TS_CRC = 1
} ts_response_format_e;

typedef struct {
	BaseChannel * channel;
	uint8_t *writeBuffer;
	char crcReadBuffer[300];
} ts_channel_s;

#define TS_HELLO_COMMAND_DEPRECATED 'H'
#define TS_HELLO_COMMAND 'S'
#define TS_LEGACY_HELLO_COMMAND 'Q'
#define TS_OUTPUT_COMMAND 'O'
#define TS_READ_COMMAND 'R'
#define TS_PAGE_COMMAND 'P'
#define TS_COMMAND_F 'F'
#define TS_EXECUTE 'E'
#define TS_GET_TEXT 'G'

#define TS_SINGLE_WRITE_COMMAND 'W'
#define TS_CHUNK_WRITE_COMMAND 'C'
#define TS_BURN_COMMAND 'B'

#define CRC_VALUE_SIZE 4
// todo: double-check this
#define CRC_WRAPPING_SIZE (CRC_VALUE_SIZE + 3)

BaseChannel * getTsSerialDevice(void);
void startTsPort(void);

void tunerStudioWriteData(ts_channel_s *tsChannel, const uint8_t * buffer, int size);
void tunerStudioWriteCrcPacket(ts_channel_s *tsChannel, const uint8_t command, const void *buf, const uint16_t size);
void tsSendResponse(ts_channel_s *tsChannel, ts_response_format_e mode, const uint8_t * buffer, int size);

#endif /* CONSOLE_TUNERSTUDIO_TUNERSTUDIO_IO_H_ */
