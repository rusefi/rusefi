#include "pch.h"

#include "tunerstudio_impl.h"
#include "tunerstudio.h"
#include "tunerstudio_io.h"

#include "live_data.h"

#include "status_loop.h"

#if EFI_TUNER_STUDIO

static constexpr size_t getTunerStudioPageSize() {
	return TOTAL_CONFIG_SIZE;
}

// Validate whether the specified offset and count would cause an overrun in the tune.
// Returns true if an overrun would occur.
bool validateOffsetCount(size_t offset, size_t count, TsChannelBase* tsChannel) {
	size_t allowedSize = getTunerStudioPageSize();
	if (offset + count > allowedSize) {
		efiPrintf("TS: Project mismatch? Too much configuration requested %d+%d>%d", offset, count, allowedSize);
		tunerStudioError(tsChannel, "ERROR: out of range");
		sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE, "bad_offset");
		return true;
	}

	return false;
}

static Timer channelsRequestTimer;

int getSecondsSinceChannelsRequest() {
    return channelsRequestTimer.getElapsedSeconds();
}

/**
 * @brief 'Output' command sends out a snapshot of current values
 * Gauges refresh
 */
void TunerStudio::cmdOutputChannels(TsChannelBase* tsChannel, uint16_t offset, uint16_t count) {
	if (offset + count > TS_TOTAL_OUTPUT_SIZE) {
		efiPrintf("TS: Version Mismatch? Too much outputs requested offset=%d + count=%d/total=%d", offset, count,
				TS_TOTAL_OUTPUT_SIZE);
		sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE, "cmd_size");
		return;
	}

	if (offset < BLOCKING_FACTOR) {
		engine->outputChannels.outputRequestPeriod = channelsRequestTimer.getElapsedUs();
		channelsRequestTimer.reset();
	}

	tsState.outputChannelsCommandCounter++;
	updateTunerStudioState();
	tsChannel->assertPacketSize(count, false);
	// this method is invoked too often to print any debug information
	uint8_t * scratchBuffer = (uint8_t *)tsChannel->scratchBuffer;
	/**
	 * collect data from all models
	 */
	copyRange(scratchBuffer + 3, getLiveDataFragments(), offset, count);

	tsChannel->crcAndWriteBuffer(TS_RESPONSE_OK, count);
}

#endif // EFI_TUNER_STUDIO
