/**
 * @file	tunerstudio_io.h
 * @file TS protocol commands and methods are here
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once
#include "global.h"

#if EFI_USB_SERIAL
#include "usbconsole.h"
#endif // EFI_USB_SERIAL

#if EFI_PROD_CODE
#include "pin_repository.h"
#endif

typedef enum {
	TS_PLAIN = 0,
	TS_CRC = 1
} ts_response_format_e;

struct ts_channel_s {
#if ! EFI_UNIT_TEST
	BaseChannel * channel = nullptr;
#endif
	/**
	 * See 'blockingFactor' in rusefi.ini
	 */
	char scratchBuffer[BLOCKING_FACTOR + 30];

#if TS_UART_DMA_MODE || PRIMARY_UART_DMA_MODE || TS_UART_MODE
	UARTDriver *uartp = nullptr;
#endif // TS_UART_DMA_MODE

	bool wasReady = false;
};

#define CRC_VALUE_SIZE 4
// todo: double-check this
#define CRC_WRAPPING_SIZE (CRC_VALUE_SIZE + 3)

void startTsPort(ts_channel_s *tsChannel);
bool stopTsPort(ts_channel_s *tsChannel);

// that's 1 second
#define BINARY_IO_TIMEOUT TIME_MS2I(1000)

// that's 1 second
#define SR5_READ_TIMEOUT TIME_MS2I(1000)

void sr5WriteData(ts_channel_s *tsChannel, const uint8_t * buffer, int size);
void sr5WriteCrcPacketSmall(ts_channel_s* tsChannel, uint8_t responseCode, const uint8_t* buf, size_t size);
void sr5WriteCrcPacketLarge(ts_channel_s* tsChannel, uint8_t responseCode, const uint8_t* buf, size_t size);
void sr5WriteCrcPacket(ts_channel_s *tsChannel, uint8_t responseCode, const uint8_t* buf, size_t size);
void sr5SendResponse(ts_channel_s *tsChannel, ts_response_format_e mode, const uint8_t * buffer, int size);
void sendOkResponse(ts_channel_s *tsChannel, ts_response_format_e mode);
int sr5ReadData(ts_channel_s *tsChannel, uint8_t * buffer, int size);
int sr5ReadDataTimeout(ts_channel_s *tsChannel, uint8_t * buffer, int size, int timeout);
bool sr5IsReady(ts_channel_s *tsChannel);
void sr5FlushData(ts_channel_s *tsChannel);

