/**
 * @file	tunerstudio_io.cpp
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#include "main.h"
#include "tunerstudio_io.h"
#include "console_io.h"
#include "engine.h"
#if EFI_SIMULATOR || defined(__DOXYGEN__)
#include "rusEfiFunctionalTest.h"
#endif

EXTERN_ENGINE;

extern LoggingWithStorage tsLogger;

#if EFI_PROD_CODE || defined(__DOXYGEN__)
#include "pin_repository.h"
#include "usbconsole.h"
#include "map_averaging.h"
extern SerialUSBDriver SDU1;
#define CONSOLE_DEVICE &SDU1

#define TS_SERIAL_UART_DEVICE &SD3

static SerialConfig tsSerialConfig = { 0, 0, USART_CR2_STOP1_BITS | USART_CR2_LINEN, 0 };

void startTsPort(void) {
#if EFI_USB_SERIAL || defined(__DOXYGEN__)
	if (isCommandLineConsoleOverTTL()) {
		print("TunerStudio over USB serial");
		/**
		 * This method contains a long delay, that's the reason why this is not done on the main thread
		 */
		usb_serial_start();
	} else
#endif
	{
		if (boardConfiguration->useSerialPort) {

			print("TunerStudio over USART");
			mySetPadMode2("tunerstudio rx", engineConfiguration->binarySerialRxPin, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
			mySetPadMode2("tunerstudio tx", engineConfiguration->binarySerialTxPin, PAL_MODE_ALTERNATE(TS_SERIAL_AF));

			tsSerialConfig.speed = boardConfiguration->tunerStudioSerialSpeed;

			sdStart(TS_SERIAL_UART_DEVICE, &tsSerialConfig);
		}
	}
}

#endif /* EFI_PROD_CODE */

BaseChannel * getTsSerialDevice(void) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	if (isCommandLineConsoleOverTTL()) {
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
        efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 64, "tunerStudioWriteData");
#if EFI_SIMULATOR || defined(__DOXYGEN__)
			logMsg("chSequentialStreamWrite [%d]\r\n", size);
#endif
	int transferred = chnWriteTimeout(tsChannel->channel, buffer, size, BINARY_IO_TIMEOUT);
#if EFI_SIMULATOR || defined(__DOXYGEN__)
			logMsg("transferred [%d]\r\n", transferred);
#endif
	if (transferred != size) {
#if EFI_SIMULATOR || defined(__DOXYGEN__)
			logMsg("!!! NOT ACCEPTED %d out of %d !!!", transferred, size);
#endif
		scheduleMsg(&tsLogger, "!!! NOT ACCEPTED %d out of %d !!!", transferred, size);
	}
}

/**
 * Adds size to the beginning of a packet and a crc32 at the end. Then send the packet.
 */
void tunerStudioWriteCrcPacket(ts_channel_s *tsChannel, const uint8_t responseCode, const void *buf, const uint16_t size) {
	uint8_t *writeBuffer = tsChannel->writeBuffer;

	*(uint16_t *) writeBuffer = SWAP_UINT16(size + 1);   // packet size including command
	*(uint8_t *) (writeBuffer + 2) = responseCode;
	tunerStudioWriteData(tsChannel, writeBuffer, 3);      // header

	// CRC on whole packet
	uint32_t crc = crc32((void *) (writeBuffer + 2), 1); // command part of CRC
	crc = crc32inc((void *) buf, crc, (uint32_t) (size)); // combined with packet CRC
	*(uint32_t *) (writeBuffer) = SWAP_UINT32(crc);

	if (size > 0) {
		tunerStudioWriteData(tsChannel, (const uint8_t*)buf, size);      // body
	}

	tunerStudioWriteData(tsChannel, writeBuffer, 4);      // CRC footer
}

void tsSendResponse(ts_channel_s *tsChannel, ts_response_format_e mode, const uint8_t * buffer, int size) {
	if (mode == TS_CRC) {
		tunerStudioWriteCrcPacket(tsChannel, TS_RESPONSE_OK, buffer, size);
	} else {
		if (size > 0)
			tunerStudioWriteData(tsChannel, buffer, size);
	}
}

