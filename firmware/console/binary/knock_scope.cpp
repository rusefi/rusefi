/**
 * @file knock_scope.cpp
 * @brief Ring-buffer transport for software_knock raw windows to the host.
 *
 * `software_knock` appends frames after each DMA window. The host drains all pending
 * frames per READ (batch header v2 + concatenated v1 frames). On overflow the oldest
 * frames are dropped and counted in the batch header.
 */

#include "pch.h"

#ifdef KNOCK_SCOPE

#include "knock_scope.h"
#include "tunerstudio_io.h"

struct KnockScopeFrameHeader {
	uint8_t version;
	uint8_t cylinderNumber;
	uint8_t channelNumber;
	uint8_t reserved;
	uint16_t sampleCount;
	uint16_t reserved2;
};

struct KnockScopeBatchHeader {
	uint8_t version;
	uint8_t reserved;
	uint16_t frameCount;
	uint16_t droppedSinceLastRead;
	uint32_t totalFramesWritten;
};

static_assert((sizeof(KnockScopeFrameHeader) % sizeof(adcsample_t)) == 0);
static_assert(sizeof(KnockScopeBatchHeader) == 12);

static BigBufferHandle buffer;
static bool isRunning = false;

static size_t readPos = 0;
static size_t writePos = 0;
// occupiedBytes includes the batch pinned during TX, pendingBytes does not.
static size_t occupiedBytes = 0;
static size_t pendingBytes = 0;
static bool sendInProgress = false;
static bool disablePending = false;
static uint32_t totalFramesWritten = 0;
static uint16_t droppedSinceLastRead = 0;

static constexpr uint8_t kKnockScopeFrameVersion = 1;
static constexpr uint8_t kKnockScopeBatchVersion = 2;
static constexpr size_t kKnockScopeBatchHeaderSize = sizeof(KnockScopeBatchHeader);

static size_t ringCapacity() {
	return buffer ? buffer.size() : 0;
}

static size_t ringUsedUnlocked() {
	return occupiedBytes;
}

static size_t ringPendingUnlocked() {
	return pendingBytes;
}

static size_t ringFreeUnlocked() {
	const size_t cap = ringCapacity();
	if (cap == 0) {
		return 0;
	}
	return cap - ringUsedUnlocked();
}

static void ringCopyFrom(size_t pos, void* dst, size_t len) {
	auto* ring = buffer.get<uint8_t>();
	const size_t cap = ringCapacity();
	pos %= cap;
	if (pos + len <= cap) {
		memcpy(dst, ring + pos, len);
	} else {
		const size_t first = cap - pos;
		memcpy(dst, ring + pos, first);
		memcpy(reinterpret_cast<uint8_t*>(dst) + first, ring, len - first);
	}
}

static void ringCopyTo(size_t pos, const void* src, size_t len) {
	auto* ring = buffer.get<uint8_t>();
	const size_t cap = ringCapacity();
	pos %= cap;
	if (pos + len <= cap) {
		memcpy(ring + pos, src, len);
	} else {
		const size_t first = cap - pos;
		memcpy(ring + pos, src, first);
		memcpy(ring, reinterpret_cast<const uint8_t*>(src) + first, len - first);
	}
}

static void ringAdvanceRead(size_t len) {
	const size_t cap = ringCapacity();
	readPos = (readPos + len) % cap;
}

static void ringAdvanceWrite(size_t len) {
	const size_t cap = ringCapacity();
	writePos = (writePos + len) % cap;
}

static bool ringPeekFrameHeader(KnockScopeFrameHeader* header) {
	if (ringPendingUnlocked() < sizeof(KnockScopeFrameHeader)) {
		return false;
	}
	ringCopyFrom(readPos, header, sizeof(*header));
	return header->version == kKnockScopeFrameVersion && header->sampleCount > 0;
}

static size_t ringFrameBytes(const KnockScopeFrameHeader& header) {
	return sizeof(header) + static_cast<size_t>(header.sampleCount) * sizeof(adcsample_t);
}

static void ringDropOldestFrameUnlocked() {
	KnockScopeFrameHeader header;
	if (!ringPeekFrameHeader(&header)) {
		// Corrupt or partial data — drop everything.
		readPos = writePos;
		occupiedBytes = 0;
		pendingBytes = 0;
		return;
	}

	const size_t frameBytes = ringFrameBytes(header);
	if (frameBytes > ringPendingUnlocked()) {
		readPos = writePos;
		occupiedBytes = 0;
		pendingBytes = 0;
		return;
	}

	ringAdvanceRead(frameBytes);
	occupiedBytes -= frameBytes;
	pendingBytes -= frameBytes;
	droppedSinceLastRead++;
}

static void updateReadyFlagUnlocked() {
	engine->outputChannels.knockScopeReady = pendingBytes != 0;
}

static size_t countPendingFramesAndBytes(uint16_t* outFrameCount) {
	size_t pos = readPos;
	size_t remainingBytes = pendingBytes;
	size_t framesBytes = 0;
	uint16_t count = 0;
	const size_t cap = ringCapacity();

	while (remainingBytes != 0) {
		if (remainingBytes < sizeof(KnockScopeFrameHeader)) {
			break;
		}

		KnockScopeFrameHeader header;
		ringCopyFrom(pos, &header, sizeof(header));
		if (header.version != kKnockScopeFrameVersion || header.sampleCount == 0) {
			break;
		}

		const size_t frameBytes = ringFrameBytes(header);
		if (frameBytes > remainingBytes) {
			break;
		}

		framesBytes += frameBytes;
		remainingBytes -= frameBytes;
		count++;
		pos = (pos + frameBytes) % cap;
	}

	if (outFrameCount) {
		*outFrameCount = count;
	}
	return framesBytes;
}

bool isKnockScopeActive() {
	return isRunning;
}

void knockScopeEnable() {
	if (!engineConfiguration->enableKnockScope) {
		return;
	}

	chibios_rt::CriticalSectionLocker csl;

	if (isRunning || sendInProgress || disablePending) {
		return;
	}

	buffer = getBigBuffer(BigBufferUser::KnockSpectrogram);
	if (!buffer) {
		return;
	}

	readPos = 0;
	writePos = 0;
	occupiedBytes = 0;
	pendingBytes = 0;
	sendInProgress = false;
	disablePending = false;
	totalFramesWritten = 0;
	droppedSinceLastRead = 0;
	memset(buffer.get<uint8_t>(), 0, buffer.size());

	isRunning = true;
	updateReadyFlagUnlocked();
}

void knockScopeDisable() {
	chibios_rt::CriticalSectionLocker csl;

	isRunning = false;
	engine->outputChannels.knockScopeReady = false;

	if (sendInProgress) {
		disablePending = true;
		return;
	}

	buffer = {};
	readPos = 0;
	writePos = 0;
	occupiedBytes = 0;
	pendingBytes = 0;
	sendInProgress = false;
	disablePending = false;
	totalFramesWritten = 0;
	droppedSinceLastRead = 0;
}

void knockScopePublishWindow(const adcsample_t* samples, size_t count, uint8_t cylinderNumber, uint8_t channelNumber) {
	if (!samples || count == 0) {
		return;
	}

	{
		chibios_rt::CriticalSectionLocker csl;

		if (!isRunning || !buffer) {
			return;
		}

		const size_t maxSamplesInBuffer = (buffer.size() - sizeof(KnockScopeFrameHeader)) / sizeof(adcsample_t);
		const size_t copiedCount = std::min(count, maxSamplesInBuffer);
		const size_t frameBytes = sizeof(KnockScopeFrameHeader) + copiedCount * sizeof(adcsample_t);
		KnockScopeFrameHeader header = {
			.version = kKnockScopeFrameVersion,
			.cylinderNumber = cylinderNumber,
			.channelNumber = channelNumber,
			.reserved = 0,
			.sampleCount = static_cast<uint16_t>(copiedCount),
			.reserved2 = 0
		};

		while (ringFreeUnlocked() < frameBytes) {
			if (sendInProgress) {
				droppedSinceLastRead++;
				return;
			}
			ringDropOldestFrameUnlocked();
		}

		ringCopyTo(writePos, &header, sizeof(header));
		ringAdvanceWrite(sizeof(header));
		ringCopyTo(writePos, samples, copiedCount * sizeof(adcsample_t));
		ringAdvanceWrite(copiedCount * sizeof(adcsample_t));
		occupiedBytes += frameBytes;
		pendingBytes += frameBytes;

		totalFramesWritten++;
		updateReadyFlagUnlocked();
	}
}

bool knockScopeSendPending(TsChannelBase* tsChannel) {
	if (!tsChannel) {
		return false;
	}

	uint16_t frameCount = 0;
	size_t framesBytes = 0;
	size_t sendPos = 0;
	uint16_t dropped = 0;
	uint32_t totalWritten = 0;
	KnockScopeBatchHeader batch;

	{
		chibios_rt::CriticalSectionLocker csl;

		if (!isRunning || !buffer || sendInProgress || pendingBytes == 0) {
			return false;
		}

		framesBytes = countPendingFramesAndBytes(&frameCount);
		if (frameCount == 0 || framesBytes == 0) {
			readPos = writePos;
			occupiedBytes = 0;
			pendingBytes = 0;
			updateReadyFlagUnlocked();
			return false;
		}

		dropped = droppedSinceLastRead;
		droppedSinceLastRead = 0;
		totalWritten = totalFramesWritten;
		batch = {
			.version = kKnockScopeBatchVersion,
			.reserved = 0,
			.frameCount = frameCount,
			.droppedSinceLastRead = dropped,
			.totalFramesWritten = totalWritten,
		};

		sendPos = readPos;
		ringAdvanceRead(framesBytes);
		pendingBytes -= framesBytes;
		sendInProgress = true;
		updateReadyFlagUnlocked();
	}

	const size_t payloadLen = kKnockScopeBatchHeaderSize + framesBytes;
	uint32_t crc = tsChannel->writePacketHeader(TS_RESPONSE_OK, payloadLen);
	crc = tsChannel->writePacketBody(reinterpret_cast<const uint8_t*>(&batch), sizeof(batch), crc);

	const auto* ring = buffer.get<uint8_t>();
	const size_t firstBytes = std::min(framesBytes, ringCapacity() - sendPos);
	crc = tsChannel->writePacketBody(ring + sendPos, firstBytes, crc);
	crc = tsChannel->writePacketBody(ring, framesBytes - firstBytes, crc);

	tsChannel->writeCrcPacketTail(crc);
	tsChannel->flush();

	{
		chibios_rt::CriticalSectionLocker csl;
		sendInProgress = false;

		if (disablePending) {
			buffer = {};
			readPos = 0;
			writePos = 0;
			occupiedBytes = 0;
			pendingBytes = 0;
			disablePending = false;
			totalFramesWritten = 0;
			droppedSinceLastRead = 0;
			engine->outputChannels.knockScopeReady = false;
		} else {
			occupiedBytes -= framesBytes;
			updateReadyFlagUnlocked();
		}
	}

	return true;
}

void initKnockScope() {
}

#endif // KNOCK_SCOPE
