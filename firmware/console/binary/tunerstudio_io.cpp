/**
 * @file	tunerstudio_io.cpp
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine.h"
#include "os_access.h"
#include "tunerstudio.h"
#include "tunerstudio_io.h"
#include "console_io.h"
#include "connector_uart_dma.h"

#if EFI_SIMULATOR
#include "rusEfiFunctionalTest.h"
#endif // EFI_SIMULATOR

EXTERN_ENGINE;

#if EFI_PROD_CODE
#include "pin_repository.h"

#if defined(TS_PRIMARY_UART) || defined(TS_PRIMARY_SERIAL)
#ifdef TS_PRIMARY_UART
	#if EFI_USE_UART_DMA
		UartDmaTsChannel primaryChannel(TS_PRIMARY_UART);
	#else
		UartTsChannel primaryChannel(TS_PRIMARY_UART);
	#endif
#elif defined(TS_PRIMARY_SERIAL)
SerialTsChannel primaryChannel(TS_PRIMARY_SERIAL);
#endif

struct PrimaryChannelThread : public TunerstudioThread {
	PrimaryChannelThread() : TunerstudioThread("Primary TS Channel") { }

	TsChannelBase* setupChannel() {
		efiSetPadMode("Primary Channel RX", EFI_CONSOLE_RX_BRAIN_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));
		efiSetPadMode("Primary Channel TX", EFI_CONSOLE_TX_BRAIN_PIN, PAL_MODE_ALTERNATE(EFI_CONSOLE_AF));

		primaryChannel.start(CONFIG(uartConsoleSerialSpeed));

		return &primaryChannel;
	}
};

static PrimaryChannelThread primaryChannelThread;
#endif // defined(TS_PRIMARY_UART) || defined(TS_PRIMARY_SERIAL)

#if defined(TS_SECONDARY_UART) || defined(TS_SECONDARY_SERIAL)

#ifdef TS_SECONDARY_UART
	#if EFI_USE_UART_DMA
		UartDmaTsChannel secondaryChannel(TS_SECONDARY_UART);
	#else
		UartTsChannel secondaryChannel(TS_SECONDARY_UART);
	#endif
#elif defined(TS_SECONDARY_SERIAL)
SerialTsChannel secondaryChannel(TS_SECONDARY_SERIAL);
#endif

struct SecondaryChannelThread : public TunerstudioThread {
	SecondaryChannelThread() : TunerstudioThread("Secondary TS Channel") { }

	TsChannelBase* setupChannel() {
		efiSetPadMode("Secondary Channel RX", engineConfiguration->binarySerialRxPin, PAL_MODE_ALTERNATE(TS_SERIAL_AF));
		efiSetPadMode("Secondary Channel TX", engineConfiguration->binarySerialTxPin, PAL_MODE_ALTERNATE(TS_SERIAL_AF));

		secondaryChannel.start(CONFIG(uartConsoleSerialSpeed));

		return &secondaryChannel;
	}
};

static SecondaryChannelThread secondaryChannelThread;
#endif // defined(TS_SECONDARY_UART) || defined(TS_SECONDARY_SERIAL)

void startSerialChannels() {
#if defined(TS_PRIMARY_UART) || defined(TS_PRIMARY_SERIAL)
	primaryChannelThread.Start();
#endif

#if defined(TS_SECONDARY_UART) || defined(TS_SECONDARY_SERIAL)
	secondaryChannelThread.Start();
#endif
}

SerialTsChannelBase* getBluetoothChannel() {
#if defined(TS_SECONDARY_UART) || defined(TS_SECONDARY_SERIAL)
	return &secondaryChannel;
#elif defined(TS_PRIMARY_UART) || defined(TS_PRIMARY_SERIAL)
	// Use primary channel for BT if no secondary exists
	return &primaryChannel;
#endif

	// no HW serial channels on this board, fail
	return nullptr;
}

size_t TsChannelBase::read(uint8_t* buffer, size_t size) {
	return readTimeout(buffer, size, SR5_READ_TIMEOUT);
}

#endif // EFI_PROD_CODE

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
