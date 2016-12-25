/**
 * @file	tunerstudio_io.h
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2016
 */
#ifndef CONSOLE_TUNERSTUDIO_TUNERSTUDIO_IO_H_
#define CONSOLE_TUNERSTUDIO_TUNERSTUDIO_IO_H_

#include "main.h"

#if EFI_PROD_CODE
#include "usbconsole.h"
#include "pin_repository.h"
#endif

// that's hex for "~\n", see
#define BINARY_SWITCH_TAG 0x7e0a
#define SWITCH_TO_BINARY_COMMAND "~"

#define PROTOCOL  "001"
#define TS_SIGNATURE "MShift v0.01"

#define TS_RESPONSE_OK 0x00
#define TS_RESPONSE_BURN_OK 0x04
#define TS_RESPONSE_COMMAND_OK 0x07

#define TS_RESPONSE_UNDERRUN 0x80
#define TS_RESPONSE_CRC_FAILURE 0x82

typedef enum {
	TS_PLAIN = 0,
	TS_CRC = 1
} ts_response_format_e;

// see 'blockingFactor' in rusefi.ini todo: move to rusefi_config.txt
// see BLOCKING_FACTOR in java console code
#define BLOCKING_FACTOR 400

typedef struct {
	BaseChannel * channel;
	uint8_t writeBuffer[4];
	/**
	 * we use 'blockingFactor = 320' in rusefi.ini
	 * todo: should we just do (320 + CRC_WRAPPING_SIZE) ?
	 */
	char crcReadBuffer[BLOCKING_FACTOR + 10];
} ts_channel_s;

#define TS_HELLO_COMMAND_DEPRECATED 'H' // 0x48
#define TS_HELLO_COMMAND 'S' // 0x53
#define TS_TEST_COMMAND 't' // 0x74
#define TS_LEGACY_HELLO_COMMAND 'Q' // 0x51
#define TS_OUTPUT_COMMAND 'O' // 0x4F
#define TS_READ_COMMAND 'R' // 0x52
#define TS_PAGE_COMMAND 'P' // 0x50
#define TS_COMMAND_F 'F' // 0x46
#define TS_EXECUTE 'E' // 0x45
#define TS_GET_TEXT 'G' // 0x47
#define TS_GET_FILE_RANGE '2'

#define TS_SINGLE_WRITE_COMMAND 'W' // 0x57
#define TS_CHUNK_WRITE_COMMAND 'C' // 0x43
#define TS_BURN_COMMAND 'B' // 0x42
#define TS_IO_TEST_COMMAND 'w' // 0x77

#define TS_CRC_CHECK_COMMAND 'k' // 0x6B

#define CRC_VALUE_SIZE 4
// todo: double-check this
#define CRC_WRAPPING_SIZE (CRC_VALUE_SIZE + 3)

BaseChannel * getTsSerialDevice(void);
void startTsPort(void);

// that's 3 seconds
#define BINARY_IO_TIMEOUT MS2ST(3000)

void tunerStudioWriteData(ts_channel_s *tsChannel, const uint8_t * buffer, int size);
void tunerStudioWriteCrcPacket(ts_channel_s *tsChannel, const uint8_t command, const void *buf, const uint16_t size);
void tsSendResponse(ts_channel_s *tsChannel, ts_response_format_e mode, const uint8_t * buffer, int size);

#endif /* CONSOLE_TUNERSTUDIO_TUNERSTUDIO_IO_H_ */
