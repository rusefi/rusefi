/**
 * @file	tunerstudio_io.cpp
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "tunerstudio_io.h"
#include "console_io.h"
#include "engine.h"

EXTERN_ENGINE;

extern Logging tsLogger;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "pin_repository.h"
#include "usbconsole.h"
#include "map_averaging.h"
extern SerialUSBDriver SDU1;
#define CONSOLE_DEVICE &SDU1

#define TS_SERIAL_UART_DEVICE &SD3

static SerialConfig tsSerialConfig = { 0, 0, USART_CR2_STOP1_BITS | USART_CR2_LINEN, 0 };

void startTsPort(void) {
	if (isSerialOverUart()) {
		print("TunerStudio over USB serial");
		/**
		 * This method contains a long delay, that's the reason why this is not done on the main thread
		 */
		usb_serial_start();
	} else if (boardConfiguration->useSerialPort) {

		print("TunerStudio over USART");
		mySetPadMode("tunerstudio rx", TS_SERIAL_RX_PORT, TS_SERIAL_RX_PIN, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
		mySetPadMode("tunerstudio tx", TS_SERIAL_TX_PORT, TS_SERIAL_TX_PIN, PAL_MODE_ALTERNATE(TS_SERIAL_AF));

		tsSerialConfig.speed = boardConfiguration->tunerStudioSerialSpeed;

		sdStart(TS_SERIAL_UART_DEVICE, &tsSerialConfig);
	}
}

#endif /* EFI_PROD_CODE */

BaseChannel * getTsSerialDevice(void) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	if (isSerialOverUart()) {
		// if console uses UART then TS uses USB
		return (BaseChannel *) &SDU1;
	} else {
		return (BaseChannel *) TS_SERIAL_UART_DEVICE;
	}
#else
	return (BaseChannel *) TS_SIMULATOR_PORT;
#endif
}

void tunerStudioWriteData(ts_channel_s *tsChannel, const uint8_t * buffer, int size) {
	int transferred = chSequentialStreamWrite(tsChannel->channel, buffer, size);
	if (transferred != size) {
		scheduleMsg(&tsLogger, "!!! NOT ACCEPTED %d out of %d !!!", transferred, size);
	}
}

/**
 * Adds size to the beginning of a packet and a crc32 at the end. Then send the packet.
 */
void tunerStudioWriteCrcPacket(ts_channel_s *tsChannel, const uint8_t command, const void *buf, const uint16_t size) {

	uint8_t *writeBuffer = tsChannel->writeBuffer;

	// todo: max size validation
	*(uint16_t *) writeBuffer = SWAP_UINT16(size + 1);   // packet size including command
	*(uint8_t *) (writeBuffer + 2) = command;
	if (size != 0)
		memcpy(writeBuffer + 3, buf, size);
	// CRC on whole packet
	uint32_t crc = crc32((void *) (writeBuffer + 2), (uint32_t) (size + 1));
	*(uint32_t *) (writeBuffer + 2 + 1 + size) = SWAP_UINT32(crc);

//	scheduleMsg(logger, "TunerStudio: CRC command %x size %d", command, size);

	tunerStudioWriteData(tsChannel, writeBuffer, size + 2 + 1 + 4);      // with size, command and CRC
}

void tsSendResponse(ts_channel_s *tsChannel, ts_response_format_e mode, const uint8_t * buffer, int size) {
	if (mode == TS_CRC) {
		tunerStudioWriteCrcPacket(tsChannel, TS_RESPONSE_OK, buffer, size);
	} else {
		if (size > 0)
			tunerStudioWriteData(tsChannel, buffer, size);
	}
}

