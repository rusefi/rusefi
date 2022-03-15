#include "pch.h"

#include "tunerstudio_impl.h"
#include "tunerstudio.h"
#include "tunerstudio_io.h"

#include "status_loop.h"

#if EFI_TUNER_STUDIO && (EFI_PROD_CODE || EFI_SIMULATOR)

/**
 * @brief 'Output' command sends out a snapshot of current values
 * Gauges refresh
 */
void TunerStudio::cmdOutputChannels(TsChannelBase* tsChannel, uint16_t offset, uint16_t count) {
	if (offset + count > sizeof(TunerStudioOutputChannels)) {
		efiPrintf("TS: Version Mismatch? Too much outputs requested %d/%d/%d", offset, count,
				sizeof(TunerStudioOutputChannels));
		sendErrorCode(tsChannel, TS_RESPONSE_OUT_OF_RANGE);
		return;
	}

	tsState.outputChannelsCommandCounter++;
	updateTunerStudioState();
	// this method is invoked too often to print any debug information
	tsChannel->writeCrcPacket(TS_RESPONSE_OK, reinterpret_cast<const uint8_t*>(&engine->outputChannels) + offset, count);
}

#endif // EFI_TUNER_STUDIO
