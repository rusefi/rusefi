/**
 * @file	tunerstudio_io.cpp
 *
 * @date Mar 8, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "tunerstudio_io.h"

#if EFI_SIMULATOR
#include "rusEfiFunctionalTest.h"
#endif // EFI_SIMULATOR

#if EFI_PROD_CODE || EFI_SIMULATOR
size_t TsChannelBase::read(uint8_t* buffer, size_t size) {
	return readTimeout(buffer, size, SR5_READ_TIMEOUT);
}
#endif

bool TsChannelBase::isBigPacket(size_t size) {
	return ((TS_PACKET_HEADER_SIZE + size + TS_PACKET_TAIL_SIZE) > sizeof(scratchBuffer));
}

void TsChannelBase::copyAndWriteSmallCrcPacket(uint8_t responseCode, const uint8_t* buf, size_t size) {
	// don't transmit too large a buffer
	criticalAssertVoid(!isBigPacket(size), "copyAndWriteSmallCrcPacket tried to transmit too large a packet")

	// If transmitting data, copy it in to place in the scratch buffer
	// We want to prevent the data changing itself (higher priority threads could write
	// tsOutputChannels) during the CRC computation.  Instead compute the CRC on our
	// local buffer that nobody else will write.
	if (size) {
		memcpy(scratchBuffer + TS_PACKET_HEADER_SIZE, buf, size);
	}

	crcAndWriteBuffer(responseCode, size);
}

void TsChannelBase::crcAndWriteBuffer(uint8_t responseCode, size_t size) {
	criticalAssertVoid(!isBigPacket(size), "crcAndWriteBuffer tried to transmit too large a packet")

	// Index 0/1 = packet size (big endian)
	*(uint16_t*)scratchBuffer = SWAP_UINT16(size + 1);
	// Index 2 = response code
	scratchBuffer[2] = responseCode;

	// CRC is computed on the responseCode and payload but not length
	uint32_t crc = crc32(&scratchBuffer[2], size + 1); // command part of CRC

	// Place the CRC at the end
	crc = SWAP_UINT32(crc);
	memcpy(scratchBuffer + size + TS_PACKET_HEADER_SIZE, &crc, sizeof(crc));

	// Write to the underlying stream
	write(reinterpret_cast<uint8_t*>(scratchBuffer), size + 7, true);
	flush();
}

uint32_t TsChannelBase::writePacketHeader(const uint8_t responseCode, const size_t size) {
	uint8_t headerBuffer[3];
	*(uint16_t*)headerBuffer = SWAP_UINT16(size + 1);
	*(uint8_t*)(headerBuffer + 2) = responseCode;
	// Write header
	write(headerBuffer, sizeof(headerBuffer), /*isEndOfPacket*/false);

	 // Command part of CRC
	return crc32((void*)(headerBuffer + 2), 1);
}

void TsChannelBase::writeCrcPacketLarge(const uint8_t responseCode, const uint8_t* buf, const size_t size) {
	uint8_t crcBuffer[4];

	// Command part of CRC
	uint32_t crc = writePacketHeader(responseCode, size);
	// Data part of CRC
	crc = crc32inc((void*)buf, crc, size);
	*(uint32_t*)crcBuffer = SWAP_UINT32(crc);

	// If data, write that
	if (size) {
		write(buf, size, /*isEndOfPacket*/false);
	}

	// Lastly the CRC footer
	write(crcBuffer, sizeof(crcBuffer), /*isEndOfPacket*/true);
	flush();
}

uint32_t TsChannelBase::writePacketBody(const uint8_t* buf, const size_t size, uint32_t crc) {
	if ((size) && (buf)) {
		write(buf, size, /*isEndOfPacket*/false);
		crc = crc32inc((void *)buf, crc, size);
	}

	// return updated CRC
	return crc;
}

void TsChannelBase::writeCrcPacketTail(uint32_t crc) {
	uint8_t crcBuffer[4];

	*(uint32_t *)crcBuffer = SWAP_UINT32(crc);

	write(crcBuffer, sizeof(crcBuffer), /*isEndOfPacket*/true);
	flush();
}

TsChannelBase::TsChannelBase(const char *p_name) {
	this->name = p_name;
}

void TsChannelBase::assertPacketSize(size_t size, bool allowLongPackets) {
	if (isBigPacket(size) && !allowLongPackets) {
		criticalError("[USE PROPER CONSOLE VERSION ] disallowed long packet of size %d", size);
	}
}

/**
 * Adds size to the beginning of a packet and a crc32 at the end. Then send the packet.
 */
void TsChannelBase::writeCrcPacket(uint8_t responseCode, const uint8_t* buf, size_t size, bool allowLongPackets) {
	// don't transmit a null buffer...
	if (!buf) {
		size = 0;
	}

	assertPacketSize(size, allowLongPackets);

	if (isBigPacket(size)) {
		// for larger packets we do not use a buffer for CRC calculation meaning data is now allowed to modify while pending
		writeCrcPacketLarge(responseCode, buf, size);
	} else {
		// for small packets we use a buffer for CRC calculation
		copyAndWriteSmallCrcPacket(responseCode, buf, size);
	}
}

void TsChannelBase::sendResponse(ts_response_format_e mode, const uint8_t * buffer, int size, bool allowLongPackets /* = false */) {
	if (mode == TS_CRC) {
		writeCrcPacket(TS_RESPONSE_OK, buffer, size, allowLongPackets);
	} else {
		if (size > 0) {
			write(buffer, size, true);
			flush();
		}
	}
}
