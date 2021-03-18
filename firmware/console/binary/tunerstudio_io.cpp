/**
 * @file	tunerstudio_io.cpp
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine.h"
#include "os_access.h"
#include "tunerstudio_io.h"
#include "console_io.h"
#include "connector_uart_dma.h"

#if EFI_SIMULATOR
#include "rusEfiFunctionalTest.h"
#endif // EFI_SIMULATOR

EXTERN_ENGINE;

extern LoggingWithStorage tsLogger;

#if EFI_PROD_CODE
#include "pin_repository.h"

#if TS_UART_DMA_MODE
#elif TS_UART_MODE
/* Note: This structure is modified from the default ChibiOS layout! */
static UARTConfig tsUartConfig = { 
	.txend1_cb 		= NULL,
	.txend2_cb 		= NULL,
	.rxend_cb 		= NULL,
	.rxchar_cb		= NULL,
	.rxerr_cb		= NULL,
	.timeout_cb		= NULL,
	.speed 			= 0,
	.cr1 			= 0,
	.cr2 			= 0/*USART_CR2_STOP1_BITS*/ | USART_CR2_LINEN,
	.cr3 			= 0,
	.rxhalf_cb		= NULL
};
#elif defined(TS_SERIAL_DEVICE)
static SerialConfig tsSerialConfig = { .speed = 0, .cr1 = 0, .cr2 = USART_CR2_STOP1_BITS | USART_CR2_LINEN, .cr3 = 0 };
#endif /* TS_UART_DMA_MODE */
#endif /* EFI_PROD_CODE */


void startTsPort(ts_channel_s *tsChannel) {
	#if EFI_PROD_CODE
	tsChannel->channel = (BaseChannel *) NULL;
		#if defined(TS_UART_DEVICE) || defined(TS_SERIAL_DEVICE)
			if (CONFIG(useSerialPort)) {

				print("TunerStudio over USART");
				/**
				 * We have hard-coded USB serial console so that it would be clear how to connect to each specific board,
				 * but for UART serial we allow users to change settings.
				 */
				efiSetPadMode("tunerstudio rx", engineConfiguration->binarySerialRxPin, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
				efiSetPadMode("tunerstudio tx", engineConfiguration->binarySerialTxPin, PAL_MODE_ALTERNATE(TS_SERIAL_AF));

				#if TS_UART_DMA_MODE
					tsChannel->uartp = TS_UART_DEVICE;
					startUartDmaConnector(tsChannel->uartp PASS_CONFIG_PARAMETER_SUFFIX);
				#elif TS_UART_MODE
					print("Using UART mode");
					// start DMA driver
					tsUartConfig.speed = CONFIG(tunerStudioSerialSpeed);
					uartStart(TS_UART_DEVICE, &tsUartConfig);
				#elif defined(TS_SERIAL_DEVICE)
					print("Using Serial mode");
					tsSerialConfig.speed = CONFIG(tunerStudioSerialSpeed);

					sdStart(TS_SERIAL_DEVICE, &tsSerialConfig);

					tsChannel->channel = (BaseChannel *) TS_SERIAL_DEVICE;
				#endif
			}
		#endif /* TS_UART_DMA_MODE || TS_UART_MODE */
	#elif EFI_SIMULATOR /* EFI_PROD_CODE */
		tsChannel->channel = (BaseChannel *) TS_SIMULATOR_PORT;
	#endif /* EFI_PROD_CODE */
}

bool stopTsPort(ts_channel_s *tsChannel) {
	#if EFI_PROD_CODE
		#if EFI_USB_SERIAL
			// don't stop USB!
			//usb_serial_stop();
			return false;
		#endif
		if (CONFIG(useSerialPort)) {
			// todo: disable Rx/Tx pads?
			#if (TS_UART_DMA_MODE || TS_UART_MODE)
				uartStop(TS_UART_DEVICE);
			#endif /* TS_UART_DMA_MODE || TS_UART_MODE */
			#ifdef TS_SERIAL_DEVICE
				sdStop(TS_SERIAL_DEVICE);
			#endif /* TS_SERIAL_DEVICE */
		}
		tsChannel->channel = (BaseChannel *) NULL;
		return true;
	#else  /* EFI_PROD_CODE */
		// don't stop simulator!
		return false;
	#endif /* EFI_PROD_CODE */
}

#if EFI_UNIT_TEST
int sr5TestWriteDataIndex = 0;
uint8_t st5TestBuffer[16000];

size_t ts_channel_s::readTimeout(uint8_t* buffer, size_t size, int timeout) {
	// unit test, nothing to do here
	return size;
}

void ts_channel_s::write(const uint8_t* buffer, size_t size) {
	memcpy(&st5TestBuffer[sr5TestWriteDataIndex], buffer, size);
	sr5TestWriteDataIndex += size;
}
#endif // EFI_UNIT_TEST

#if EFI_PROD_CODE || EFI_SIMULATOR
void ts_channel_s::write(const uint8_t* buffer, size_t size) {
        efiAssertVoid(CUSTOM_ERR_6570, getCurrentRemainingStack() > 64, "tunerStudioWriteData");
#if EFI_SIMULATOR
			logMsg("chSequentialStreamWrite [%d]\r\n", size);
#endif

#if (PRIMARY_UART_DMA_MODE || TS_UART_DMA_MODE || TS_UART_MODE) && EFI_PROD_CODE
	if (uartp) {
		uartSendTimeout(uartp, &size, buffer, BINARY_IO_TIMEOUT);
		return;
	}
#endif
	if (!channel) {
		return;
	}

//	int transferred = chnWriteTimeout(tsChannel->channel, buffer, size, BINARY_IO_TIMEOUT);
	// temporary attempt to work around #553
	// instead of one huge packet let's try sending a few smaller packets
	size_t transferred = 0;
	size_t stillToTransfer = size;
	while (stillToTransfer > 0) {
		int thisTransferSize = minI(stillToTransfer, 768);
		transferred += chnWriteTimeout(channel, buffer, thisTransferSize, BINARY_IO_TIMEOUT);
		buffer += thisTransferSize;
		stillToTransfer -= thisTransferSize;
	}

#if EFI_SIMULATOR
			logMsg("transferred [%d]\r\n", transferred);
#endif
	if (transferred != size) {
#if EFI_SIMULATOR
			logMsg("!!! NOT ACCEPTED %d out of %d !!!", transferred, size);
#endif /* EFI_SIMULATOR */
		scheduleMsg(&tsLogger, "!!! NOT ACCEPTED %d out of %d !!!", transferred, size);
	}
}

size_t ts_channel_s::readTimeout(uint8_t* buffer, size_t size, int timeout) {
#if TS_UART_DMA_MODE || PRIMARY_UART_DMA_MODE
	if (uartp) {
		extern uart_dma_s tsUartDma;
		return iqReadTimeout(&tsUartDma.fifoRxQueue, buffer, size, timeout);
	}
#endif

#if TS_UART_DMA_MODE
#elif TS_UART_MODE
	uartReceiveTimeout(TS_UART_DEVICE, &size, buffer, timeout);
	return size;
#else /* TS_UART_DMA_MODE */
	if (channel == nullptr)
		return 0;
	return chnReadTimeout(channel, buffer, size, timeout);
#endif /* TS_UART_DMA_MODE */
	firmwareError(CUSTOM_ERR_6126, "Unexpected channel situation");
	return 0;
}

size_t TsChannelBase::read(uint8_t* buffer, size_t size) {
	return readTimeout(buffer, size, SR5_READ_TIMEOUT);
}
#endif // EFI_PROD_CODE || EFI_SIMULATOR

void TsChannelBase::writeCrcPacketSmall(uint8_t responseCode, const uint8_t* buf, size_t size) {
	auto scratchBuffer = this->scratchBuffer;

	// don't transmit too large a buffer
	efiAssertVoid(OBD_PCM_Processor_Fault, size <= BLOCKING_FACTOR + 7, "writeCrcPacketSmall tried to transmit too large a packet")

	// If transmitting data, copy it in to place in the scratch buffer
	// We want to prevent the data changing itself (higher priority threads could write
	// tsOutputChannels) during the CRC computation.  Instead compute the CRC on our
	// local buffer that nobody else will write.
	if (size) {
		memcpy(scratchBuffer + 3, buf, size);
	}

	// Index 0/1 = packet size (big endian)
	*(uint16_t*)scratchBuffer = SWAP_UINT16(size + 1);
	// Index 2 = response code
	scratchBuffer[2] = responseCode;

	// CRC is computed on the responseCode and payload but not length
	uint32_t crc = crc32(&scratchBuffer[2], size + 1); // command part of CRC

	// Place the CRC at the end
	*reinterpret_cast<uint32_t*>(&scratchBuffer[size + 3]) = SWAP_UINT32(crc);

	// Write to the underlying stream
	write(reinterpret_cast<uint8_t*>(scratchBuffer), size + 7);
}

void TsChannelBase::writeCrcPacketLarge(uint8_t responseCode, const uint8_t* buf, size_t size) {
	uint8_t headerBuffer[3];
	uint8_t crcBuffer[4];

	*(uint16_t*)headerBuffer = SWAP_UINT16(size + 1);
	*(uint8_t*)(headerBuffer + 2) = responseCode;

	// Command part of CRC
	uint32_t crc = crc32((void*)(headerBuffer + 2), 1);
	// Data part of CRC
	crc = crc32inc((void*)buf, crc, size);
	*(uint32_t*)crcBuffer = SWAP_UINT32(crc);

	// Write header
	write(headerBuffer, sizeof(headerBuffer));

	// If data, write that
	if (size) {
		write(buf, size);
	}

	// Lastly the CRC footer
	write(crcBuffer, sizeof(crcBuffer));
}

/**
 * Adds size to the beginning of a packet and a crc32 at the end. Then send the packet.
 */
void TsChannelBase::writeCrcPacket(uint8_t responseCode, const uint8_t* buf, size_t size) {
	// don't transmit a null buffer...
	if (!buf) {
		size = 0;
	}

	if (size <= BLOCKING_FACTOR + 7) {
		// small packets use small packet optimization
		writeCrcPacketSmall(responseCode, buf, size);
	} else {
		writeCrcPacketLarge(responseCode, buf, size);
	}

	flush();
}

void TsChannelBase::sendResponse(ts_response_format_e mode, const uint8_t * buffer, int size) {
	if (mode == TS_CRC) {
		writeCrcPacket(TS_RESPONSE_OK, buffer, size);
	} else {
		if (size > 0) {
			write(buffer, size);
			flush();
		}
	}
}

bool ts_channel_s::isConfigured() const {
	return
#if TS_UART_DMA_MODE || PRIMARY_UART_DMA_MODE || TS_UART_MODE
		this->uartp ||
#endif
		this->channel;
}

bool ts_channel_s::isReady() const {
#if EFI_USB_SERIAL
	if (isUsbSerial(this->channel)) {
		// TS uses USB when console uses serial
		return is_usb_serial_ready();
	}
#endif /* EFI_USB_SERIAL */
	return true;
}

#if EFI_PROD_CODE || EFI_SIMULATOR
void BaseChannelTsChannel::write(const uint8_t* buffer, size_t size) {
	chnWriteTimeout(m_channel, buffer, size, BINARY_IO_TIMEOUT);
}

size_t BaseChannelTsChannel::readTimeout(uint8_t* buffer, size_t size, int timeout) {
	return chnReadTimeout(m_channel, buffer, size, timeout);
}

void BaseChannelTsChannel::flush() {
	// nop for this channel, writes automatically flush
}

bool BaseChannelTsChannel::isReady() const {
#if EFI_USB_SERIAL
if (isUsbSerial(m_channel)) {
		// TS uses USB when console uses serial
		return is_usb_serial_ready();
	}
#endif /* EFI_USB_SERIAL */
	return true;
}
#endif // EFI_PROD_CODE || EFI_SIMULATOR
