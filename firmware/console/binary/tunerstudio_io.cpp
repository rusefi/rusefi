/**
 * @file	tunerstudio_io.cpp
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "os_access.h"
#include "tunerstudio_io.h"

#if EFI_SIMULATOR
#include "rusEfiFunctionalTest.h"
#endif // EFI_SIMULATOR

#if EFI_PROD_CODE || EFI_SIMULATOR
size_t TsChannelBase::read(uint8_t* buffer, size_t size) {
	return readTimeout(buffer, size, SR5_READ_TIMEOUT);
}
#endif

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
