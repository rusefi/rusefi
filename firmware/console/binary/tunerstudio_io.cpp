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

void TsChannelBase::writeCrcPacketSmall(uint8_t responseCode, const uint8_t* buf, size_t size) {
	// don't transmit too large a buffer
	criticalAssertVoid(!isBigPacket(size), "writeCrcPacketSmall tried to transmit too large a packet")

	// Index 0/1 = packet size (big endian)
	*(uint16_t*)scratchBuffer = SWAP_UINT16(size + 1);
	// Index 2 = response code
	scratchBuffer[2] = responseCode;

	// If transmitting data, copy it in to place in the scratch buffer
	// We want to prevent the data changing itself (higher priority threads could write
	// tsOutputChannels) during the CRC computation.  Instead compute the CRC on our
	// local buffer that nobody else will write.
	if (size) {
		memcpy(&scratchBuffer[TS_PACKET_HEADER_SIZE], buf, size);
	}
	// CRC is computed on the responseCode and payload but not length
	uint32_t crc = crc32(&scratchBuffer[2], size + 1);

	// Place the CRC at the end
	crc = SWAP_UINT32(crc);
	memcpy(&scratchBuffer[TS_PACKET_HEADER_SIZE + size], &crc, sizeof(crc));

	// Write to the underlying stream
	write(reinterpret_cast<uint8_t*>(scratchBuffer), TS_PACKET_HEADER_SIZE + size + TS_PACKET_TAIL_SIZE, true);
	flush();
}

uint32_t TsChannelBase::writePacketHeader(const uint8_t responseCode, const size_t size) {
	uint8_t headerBuffer[3];
	*(uint16_t*)headerBuffer = SWAP_UINT16(size + 1);
	*(uint8_t*)(headerBuffer + 2) = responseCode;
	// Write header
	write(headerBuffer, sizeof(headerBuffer), /*isEndOfPacket*/false);

	// CRC is computed on the responseCode and payload but not length
	return crc32((void*)(&headerBuffer[2]), 1);
}

uint32_t TsChannelBase::writePacketBody(const uint8_t* buf, const size_t size, uint32_t crc) {
	if ((size) && (buf)) {
		write(buf, size, /*isEndOfPacket*/false);

		return crc32inc((void *)buf, crc, size);
	}

	return crc;
}

void TsChannelBase::writeCrcPacketTail(uint32_t crc) {
	uint8_t crcBuffer[4];

	/* TODO: rework */
	*(uint32_t *)crcBuffer = SWAP_UINT32(crc);

	write(crcBuffer, sizeof(crcBuffer), true);
	flush();
}

void TsChannelBase::writeCrcPacketLarge(const uint8_t responseCode, const uint8_t* buf, size_t size) {
	// Command part
	size_t offset = 0;
	uint32_t crc = writePacketHeader(responseCode, size);

	// Data part
	while (size) {
		uint16_t chunk = minI(size, sizeof(scratchBuffer));

		// copy data to local buffer so it will not be changed between crc calculation and sent
		memcpy(scratchBuffer, buf + offset, chunk);
		crc = writePacketBody((uint8_t *)scratchBuffer, chunk, crc);

		size -= chunk;
		offset += chunk;
	}

	// Lastly the CRC footer
	writeCrcPacketTail(crc);
}

TsChannelBase::TsChannelBase(const char *p_name) {
	this->name = p_name;
}

/**
 * Adds size to the beginning of a packet and a crc32 at the end. Then send the packet.
 */
void TsChannelBase::writeCrcPacket(uint8_t responseCode, const uint8_t* buf, size_t size) {
	// don't transmit a null buffer...
	if (!buf) {
		size = 0;
	}

	if (isBigPacket(size)) {
		// For larger packets we also use a buffer for CRC calculation
		// but we do it in buffer sized blocks. Therefore, the overall CRC will
		// be valid, but the data may be partially updated during the sending process
		writeCrcPacketLarge(responseCode, buf, size);
	} else {
		// for small packets we use a buffer for CRC calculation
		writeCrcPacketSmall(responseCode, buf, size);
	}
}

void TsChannelBase::sendResponse(ts_response_format_e mode, const uint8_t * buffer, int size) {
	if (mode == TS_CRC) {
		writeCrcPacket(TS_RESPONSE_OK, buffer, size);
	} else {
		if (size > 0) {
			write(buffer, size, true);
			flush();
		}
	}
}
