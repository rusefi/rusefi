#include "tooth_logger.h"

#include <cstddef>
#include "efitime.h"
#include "global.h"
#include "efilib.h"
#include "tunerstudio_configuration.h"

extern TunerStudioOutputChannels tsOutputChannels;

static uint16_t buffer[1000] CCM_OPTIONAL;
static size_t NextIdx = 0;
static volatile bool ToothLoggerEnabled = false;

static uint32_t lastEdgeTimestamp = 0;

void SetNextEntry(uint16_t entry) {
	buffer[NextIdx] = SWAP_UINT16(entry);
	NextIdx++;

	// If we hit the end, loop
	if (NextIdx >= sizeof(buffer) / sizeof(buffer[0])) {
		NextIdx = 0;
	}
}

void LogTriggerTooth(trigger_event_e tooth) {
	// bail if we aren't enabled
	if (!ToothLoggerEnabled) return;

	// We currently only support the primary trigger falling edge
	// (this is the edge that VR sensors are accurate on)
	// Since VR senors are the most useful case here, this is okay for now.
	if (tooth != SHAFT_PRIMARY_FALLING) return;

	efitick_t timestamp = getTimeNowNt();

	uint32_t nowUs = NT2US(timestamp);
	// 10us per LSB - this gives plenty of accuracy, yet fits 655.35 ms in to a uint16
	uint16_t delta = static_cast<uint16_t>((nowUs - lastEdgeTimestamp) / 10);
	lastEdgeTimestamp = nowUs;

	SetNextEntry(delta);
}

void EnableToothLogger() {
	// Clear the buffer
	memset(buffer, 0, sizeof(buffer));

	// Reset the last edge to now - this prevents the first edge logged from being bogus
	lastEdgeTimestamp = NT2US(getTimeNowNt());

	// Reset write index
	NextIdx = 0;

	// Enable logging of edges as they come
	ToothLoggerEnabled = true;

	// Tell TS that we're ready for it to read out the log
	// nb: this is a lie, as we may not have written anything
	// yet.  However, we can let it continuously read out the buffer
	// as we update it, which looks pretty nice.
	tsOutputChannels.toothLogReady = true;
}

void DisableToothLogger() {
	ToothLoggerEnabled = false;
	tsOutputChannels.toothLogReady = false;
}

ToothLoggerBuffer GetToothLoggerBuffer() {
	return { reinterpret_cast<uint8_t*>(buffer), sizeof(buffer) };
}
