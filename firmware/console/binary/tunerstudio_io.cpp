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

#if HAL_USE_SERIAL_USB
// Assert that the USB tx/rx buffers are large enough to fit one full packet
static_assert(SERIAL_USB_BUFFERS_SIZE >= BLOCKING_FACTOR + 10);
#define SERIAL_USB_DRIVER SerialUSBDriver
#define TS_USB_DEVICE EFI_CONSOLE_USB_DEVICE // SDU1
#endif /* HAL_USE_SERIAL_USB */

#ifdef TS_USB_DEVICE
extern SERIAL_USB_DRIVER TS_USB_DEVICE;
#endif /* TS_USB_DEVICE */

#ifdef TS_CAN_DEVICE
#include "serial_can.h"
#endif /* TS_CAN_DEVICE */


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
#elif defined(TS_CAN_DEVICE)
static CANConfig tsCanConfig = { CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP, CAN_BTR_500 };
#endif /* TS_UART_DMA_MODE */
#endif /* EFI_PROD_CODE */


void startTsPort(ts_channel_s *tsChannel) {

	#if EFI_PROD_CODE
	tsChannel->channel = (BaseChannel *) NULL;
		#if defined(TS_USB_DEVICE)
#if defined(TS_UART_DEVICE)
#error 	"cannot have TS_UART_DEVICE and TS_USB_DEVICE"
#endif
			print("TunerStudio over USB serial");
			/**
			 * This method contains a long delay, that's the reason why this is not done on the main thread
			 * TODO: actually now with some refactoring this IS on the main thread :(
			 */
			usb_serial_start();
			// if console uses UART then TS uses USB
			tsChannel->channel = (BaseChannel *) &TS_USB_DEVICE;
			return;
		#endif /* TS_USB_DEVICE */
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
		#if defined(TS_CAN_DEVICE)
			/*if (CONFIG(useCanForTs))*/ {
				print("TunerStudio over CAN");

				efiSetPadMode("ts can rx", GPIOG_13/*CONFIG(canRxPin)*/, PAL_MODE_ALTERNATE(TS_CAN_AF)); // CAN2_RX2_0
				efiSetPadMode("ts can tx", GPIOG_14/*CONFIG(canTxPin)*/, PAL_MODE_ALTERNATE(TS_CAN_AF)); // CAN2_TX2_0

				canStart(&TS_CAN_DEVICE, &tsCanConfig);
				canInit(&TS_CAN_DEVICE);

				//tsChannel->channel = (BaseChannel *) &TS_CAN_DEVICE;
			}
		#endif /* TS_CAN_DEVICE */
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
		#if defined(TS_CAN_DEVICE)
		/*if (CONFIG(useCanForTs))*/ {
			canStop(&TS_CAN_DEVICE);
		}
		#endif /* TS_CAN_DEVICE */
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

void sr5WriteData(ts_channel_s *tsChannel, const uint8_t * buffer, int size) {
	memcpy(&st5TestBuffer[sr5TestWriteDataIndex], buffer, size);
	sr5TestWriteDataIndex += size;
}
#endif // EFI_UNIT_TEST

#if EFI_PROD_CODE || EFI_SIMULATOR
void sr5WriteData(ts_channel_s *tsChannel, const uint8_t * buffer, int size) {
        efiAssertVoid(CUSTOM_ERR_6570, getCurrentRemainingStack() > 64, "tunerStudioWriteData");
#if EFI_SIMULATOR
			logMsg("chSequentialStreamWrite [%d]\r\n", size);
#endif

#if (PRIMARY_UART_DMA_MODE || TS_UART_DMA_MODE || TS_UART_MODE) && EFI_PROD_CODE
	if (tsChannel->uartp != nullptr) {
	    int transferred = size;
	    uartSendTimeout(tsChannel->uartp, (size_t *)&transferred, buffer, BINARY_IO_TIMEOUT);
        return;
	}
#elif defined(TS_CAN_DEVICE)
	UNUSED(tsChannel);
	int transferred = size;
	canAddToTxStreamTimeout(&TS_CAN_DEVICE, (size_t *)&transferred, buffer, BINARY_IO_TIMEOUT);
#endif
	if (tsChannel->channel == nullptr)
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

int sr5ReadDataTimeout(ts_channel_s *tsChannel, uint8_t * buffer, int size, int timeout) {
#if TS_UART_DMA_MODE || PRIMARY_UART_DMA_MODE
	if (tsChannel->uartp!= NULL) {
		extern uart_dma_s tsUartDma;
		return (int)iqReadTimeout(&tsUartDma.fifoRxQueue, (uint8_t * )buffer, (size_t)size, timeout);
	}
#endif

#if TS_UART_DMA_MODE
#elif TS_UART_MODE
	UNUSED(tsChannel);
	size_t received = (size_t)size;
	uartReceiveTimeout(TS_UART_DEVICE, &received, buffer, timeout);
	return (int)received;
#elif defined(TS_CAN_DEVICE)
	UNUSED(tsChannel);
	size_t received = (size_t)size;
	canStreamReceiveTimeout(&TS_CAN_DEVICE, &received, buffer, timeout);
	return (int)received;
#else /* TS_UART_DMA_MODE */
	if (tsChannel->channel == nullptr)
		return 0;
	return chnReadTimeout(tsChannel->channel, (uint8_t * )buffer, size, timeout);
#endif /* TS_UART_DMA_MODE */
	firmwareError(CUSTOM_ERR_6126, "Unexpected channel situation");
	return 0;
}

int sr5ReadData(ts_channel_s *tsChannel, uint8_t * buffer, int size) {
	return sr5ReadDataTimeout(tsChannel, buffer, size, SR5_READ_TIMEOUT);
}
#endif // EFI_PROD_CODE || EFI_SIMULATOR

void sr5WriteCrcPacketSmall(ts_channel_s* tsChannel, uint8_t responseCode, const uint8_t* buf, size_t size) {
	auto scratchBuffer = tsChannel->scratchBuffer;

	// don't transmit too large a buffer
	efiAssertVoid(OBD_PCM_Processor_Fault, size <= BLOCKING_FACTOR + 7, "sr5WriteCrcPacket tried to transmit too large a packet")

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
	sr5WriteData(tsChannel, reinterpret_cast<uint8_t*>(scratchBuffer), size + 7);
}

void sr5WriteCrcPacketLarge(ts_channel_s* tsChannel, uint8_t responseCode, const uint8_t* buf, size_t size) {
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
	sr5WriteData(tsChannel, headerBuffer, sizeof(headerBuffer));

	// If data, write that
	if (size) {
		sr5WriteData(tsChannel, buf, size);
	}

	// Lastly the CRC footer
	sr5WriteData(tsChannel, crcBuffer, sizeof(crcBuffer));
}

/**
 * Adds size to the beginning of a packet and a crc32 at the end. Then send the packet.
 */
void sr5WriteCrcPacket(ts_channel_s *tsChannel, uint8_t responseCode, const uint8_t* buf, size_t size) {
	// don't transmit a null buffer...
	if (!buf) {
		size = 0;
	}

#if defined(TS_CAN_DEVICE) && defined(TS_CAN_DEVICE_SHORT_PACKETS_IN_ONE_FRAME)
	// a special case for short packets: we can sent them in 1 frame, without CRC & size,
	// because the CAN protocol is already protected by its own checksum.
	if ((size + 1) <= 7) {
		sr5WriteData(tsChannel, &responseCode, 1);      // header without size
		if (size > 0) {
			sr5WriteData(tsChannel, (const uint8_t*)buf, size);      // body
		}
		sr5FlushData(tsChannel);
		return;
	}
#endif /* TS_CAN_DEVICE */
	if (size <= BLOCKING_FACTOR + 7) {
		// small packets use small packet optimization
		sr5WriteCrcPacketSmall(tsChannel, responseCode, buf, size);
	} else {
		sr5WriteCrcPacketLarge(tsChannel, responseCode, buf, size);
	}

	sr5FlushData(tsChannel);
}

void sr5SendResponse(ts_channel_s *tsChannel, ts_response_format_e mode, const uint8_t * buffer, int size) {
	if (mode == TS_CRC) {
		sr5WriteCrcPacket(tsChannel, TS_RESPONSE_OK, buffer, size);
	} else {
		if (size > 0) {
			sr5WriteData(tsChannel, buffer, size);
			sr5FlushData(tsChannel);
		}
	}
}

bool sr5IsReady(ts_channel_s *tsChannel) {
#if EFI_USB_SERIAL
	if (isUsbSerial(tsChannel->channel)) {
		// TS uses USB when console uses serial
		return is_usb_serial_ready();
	}
#endif /* EFI_USB_SERIAL */
	return true;
}

void sr5FlushData(ts_channel_s *tsChannel) {
#if defined(TS_CAN_DEVICE)
	UNUSED(tsChannel);
	canFlushTxStream(&TS_CAN_DEVICE);
#else
	UNUSED(tsChannel);
#endif /* TS_CAN_DEVICE */
}

