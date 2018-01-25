/**
 * @file	tunerstudio_io.cpp
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
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

#if HAL_USE_SERIAL_USB || defined(__DOXYGEN__)
extern SerialUSBDriver SDU1;
#define CONSOLE_USB_DEVICE &SDU1
#endif

#if TS_UART_DMA_MODE
// Async. FIFO buffer takes some RAM...
static uart_dma_s tsUartDma;

/* Common function for all DMA-UART IRQ handlers. */
static void tsCopyDataFromDMA() {
	chSysLockFromISR();
	// get 0-based DMA buffer position
	int dmaPos = TS_DMA_BUFFER_SIZE - dmaStreamGetTransactionSize(TS_DMA_UART_DEVICE->dmarx);
	// if the position is wrapped (circular DMA-mode enabled)
	if (dmaPos < tsUartDma.readPos)
		dmaPos += TS_DMA_BUFFER_SIZE;
	// we need to update the current readPos
	int newReadPos = tsUartDma.readPos;
	for (int i = newReadPos; i < dmaPos; ) {
		if (iqPutI(&tsUartDma.fifoRxQueue, tsUartDma.dmaBuffer[newReadPos]) != Q_OK) {
			break; // todo: ignore overflow?
		}
		// the read position should always stay inside the buffer range
		newReadPos = (++i) & (TS_DMA_BUFFER_SIZE - 1);
	}
	tsUartDma.readPos = newReadPos;
	chSysUnlockFromISR();
}

/* We use the same handler code for both halves. */
static void tsRxIRQHalfHandler(UARTDriver *uartp, uartflags_t full) {
	UNUSED(uartp);
	UNUSED(full);
	tsCopyDataFromDMA();
}

/* This handler is called right after the UART receiver has finished its work. */
static void tsRxIRQIdleHandler(UARTDriver *uartp) {
	UNUSED(uartp);
	tsCopyDataFromDMA();
}

/* Note: This structure is modified from the default ChibiOS layout! */
static UARTConfig tsDmaUartConfig = { 
	NULL, NULL, NULL, NULL, NULL, 
	0, 0, 0/*USART_CR2_STOP1_BITS*/ | USART_CR2_LINEN, 0,
	/*timeout_cb*/tsRxIRQIdleHandler, /*rxhalf_cb*/tsRxIRQHalfHandler
};
#else
static SerialConfig tsSerialConfig = { 0, 0, USART_CR2_STOP1_BITS | USART_CR2_LINEN, 0 };
#endif /* TS_UART_DMA_MODE */
#endif /* EFI_PROD_CODE */


void startTsPort(ts_channel_s *tsChannel) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
#if EFI_USB_SERIAL || defined(__DOXYGEN__)
	if (isCommandLineConsoleOverTTL()) {
		print("TunerStudio over USB serial");
		/**
		 * This method contains a long delay, that's the reason why this is not done on the main thread
		 */
		usb_serial_start();
		// if console uses UART then TS uses USB
		tsChannel->channel = (BaseChannel *) CONSOLE_USB_DEVICE;
	} else
#endif
	{
		if (boardConfiguration->useSerialPort) {

			print("TunerStudio over USART");
			efiSetPadMode("tunerstudio rx", engineConfiguration->binarySerialRxPin, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
			efiSetPadMode("tunerstudio tx", engineConfiguration->binarySerialTxPin, PAL_MODE_ALTERNATE(TS_SERIAL_AF));

#if TS_UART_DMA_MODE
			print("Using UART-DMA mode");
			// init FIFO queue
			iqObjectInit(&tsUartDma.fifoRxQueue, tsUartDma.buffer, sizeof(tsUartDma.buffer), NULL, NULL);
			
			// start DMA driver
			tsDmaUartConfig.speed = boardConfiguration->tunerStudioSerialSpeed;
			uartStart(TS_DMA_UART_DEVICE, &tsDmaUartConfig);

			// start continuous DMA transfer using our circular buffer
			tsUartDma.readPos = 0;
			uartStartReceive(TS_DMA_UART_DEVICE, sizeof(tsUartDma.dmaBuffer), tsUartDma.dmaBuffer);
#else
			print("Using Serial mode");
			tsSerialConfig.speed = boardConfiguration->tunerStudioSerialSpeed;

			sdStart(TS_SERIAL_UART_DEVICE, &tsSerialConfig);
			
			tsChannel->channel = (BaseChannel *) TS_SERIAL_UART_DEVICE;
#endif /* TS_UART_DMA_MODE */
		} else
			tsChannel->channel = (BaseChannel *) NULL;	// actually not used
	}
#else  /* EFI_PROD_CODE */
	tsChannel->channel = (BaseChannel *) TS_SIMULATOR_PORT;
#endif /* EFI_PROD_CODE */
}

bool stopTsPort(ts_channel_s *tsChannel) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
#if EFI_USB_SERIAL || defined(__DOXYGEN__)
	if (isCommandLineConsoleOverTTL()) {
#if 0
		usb_serial_stop();
#endif
		// don't stop USB!
		return false;
	} else
#endif
	{
		if (boardConfiguration->useSerialPort) {
			// todo: disable Rx/Tx pads?
#if TS_UART_DMA_MODE
			uartStop(TS_DMA_UART_DEVICE);
#else
			sdStop(TS_SERIAL_UART_DEVICE);
#endif /* TS_UART_DMA_MODE */
		}
	}

	tsChannel->channel = (BaseChannel *) NULL;
	return true;
#else  /* EFI_PROD_CODE */
	// don't stop simulator!
	return false;
#endif /* EFI_PROD_CODE */
}

void sr5WriteData(ts_channel_s *tsChannel, const uint8_t * buffer, int size) {
        efiAssertVoid(getRemainingStack(chThdGetSelfX()) > 64, "tunerStudioWriteData");
#if EFI_SIMULATOR || defined(__DOXYGEN__)
			logMsg("chSequentialStreamWrite [%d]\r\n", size);
#endif

#if TS_UART_DMA_MODE && EFI_PROD_CODE
	UNUSED(tsChannel);
	int transferred = size;
	uartSendTimeout(TS_DMA_UART_DEVICE, (size_t *)&transferred, buffer, BINARY_IO_TIMEOUT);
#else
	if (tsChannel->channel == NULL)
		return;

//	int transferred = chnWriteTimeout(tsChannel->channel, buffer, size, BINARY_IO_TIMEOUT);
	// temporary attempt to work around #553
	// instead of one huge packet let's try sending a few smaller packets
	int transferred = 0;
	int stillToTransfer = size;
	while (stillToTransfer > 0) {
		int thisTransferSize = minI(stillToTransfer, 768);
		transferred += chnWriteTimeout(tsChannel->channel, buffer, thisTransferSize, BINARY_IO_TIMEOUT);
		buffer += thisTransferSize;
		stillToTransfer -= thisTransferSize;
	}

#endif

#if EFI_SIMULATOR || defined(__DOXYGEN__)
			logMsg("transferred [%d]\r\n", transferred);
#endif
	if (transferred != size) {
#if EFI_SIMULATOR || defined(__DOXYGEN__)
			logMsg("!!! NOT ACCEPTED %d out of %d !!!", transferred, size);
#endif /* EFI_SIMULATOR */
		scheduleMsg(&tsLogger, "!!! NOT ACCEPTED %d out of %d !!!", transferred, size);
	}
}

int sr5ReadDataTimeout(ts_channel_s *tsChannel, uint8_t * buffer, int size, int timeout) {
#if TS_UART_DMA_MODE || defined(__DOXYGEN__)
	UNUSED(tsChannel);
	return (int)iqReadTimeout(&tsUartDma.fifoRxQueue, (uint8_t * )buffer, (size_t)size, timeout);
#else /* TS_UART_DMA_MODE */
	if (tsChannel->channel == NULL)
		return 0;
	return chnReadTimeout(tsChannel->channel, (uint8_t * )buffer, size, timeout);
#endif /* TS_UART_DMA_MODE */
}

int sr5ReadData(ts_channel_s *tsChannel, uint8_t * buffer, int size) {
	return sr5ReadDataTimeout(tsChannel, buffer, size, SR5_READ_TIMEOUT);
}


/**
 * Adds size to the beginning of a packet and a crc32 at the end. Then send the packet.
 */
void sr5WriteCrcPacket(ts_channel_s *tsChannel, const uint8_t responseCode, const void *buf, const uint16_t size) {
	uint8_t *writeBuffer = tsChannel->writeBuffer;
	uint8_t *crcBuffer = &tsChannel->writeBuffer[3];

	*(uint16_t *) writeBuffer = SWAP_UINT16(size + 1);   // packet size including command
	*(uint8_t *) (writeBuffer + 2) = responseCode;

	// CRC on whole packet
	uint32_t crc = crc32((void *) (writeBuffer + 2), 1); // command part of CRC
	crc = crc32inc((void *) buf, crc, (uint32_t) (size)); // combined with packet CRC

	*(uint32_t *) (crcBuffer) = SWAP_UINT32(crc);

	sr5WriteData(tsChannel, writeBuffer, 3);      // header
	if (size > 0) {
		sr5WriteData(tsChannel, (const uint8_t*)buf, size);      // body
	}
	sr5WriteData(tsChannel, crcBuffer, 4);      // CRC footer
}

void sr5SendResponse(ts_channel_s *tsChannel, ts_response_format_e mode, const uint8_t * buffer, int size) {
	if (mode == TS_CRC) {
		sr5WriteCrcPacket(tsChannel, TS_RESPONSE_OK, buffer, size);
	} else {
		if (size > 0)
			sr5WriteData(tsChannel, buffer, size);
	}
}

bool sr5IsReady(bool isConsoleRedirect) {
#if EFI_PROD_CODE || defined(__DOXYGEN__)
	if (isCommandLineConsoleOverTTL() ^ isConsoleRedirect) {
		// TS uses USB when console uses serial
		return is_usb_serial_ready();
	} else {
		// TS uses serial when console uses USB
		return true;
	}
#else
	return true;
#endif
}

