/*
 * @file tooth_logger.cpp
 *
 * @date Jul 7, 2019
 * @author Matthew Kennedy
 */

#include "tooth_logger.h"

#include "global.h"

#if EFI_TOOTH_LOGGER

#include <cstddef>
#include "efitime.h"
#include "efilib.h"
#include "tunerstudio_configuration.h"

typedef struct __attribute__ ((packed)) {
	// the whole order of all packet bytes is reversed, not just the 'endian-swap' integers
	uint32_t timestamp;
	// unfortunately all these fields are required by TS...
	bool priLevel : 1;
	bool secLevel : 1;
	bool trigger : 1;
	bool sync : 1;
} composite_logger_s;



static composite_logger_s buffer[COMPOSITE_PACKET_COUNT] CCM_OPTIONAL;
static size_t NextIdx = 0;
static volatile bool ToothLoggerEnabled = false;

static uint32_t lastEdgeTimestamp = 0;

static bool trigger1 = false;
static bool trigger2 = false;

//char (*__kaboom)[sizeof( composite_logger_s )] = 1;
//char (*__kaboom)[sizeof( buffer )] = 1;

static void SetNextEntry(uint32_t nowUs, bool trigger1, bool trigger2) {
	// TS uses big endian, grumble
	buffer[NextIdx].timestamp = SWAP_UINT32(nowUs);
	buffer[NextIdx].priLevel = trigger1;
	buffer[NextIdx].secLevel = trigger2;
	// todo:
	//buffer[NextIdx].sync = isSynced;
	//buffer[NextIdx].trigger = wtfIsTriggerIdk;

	NextIdx++;

	static_assert(sizeof(composite_logger_s) == COMPOSITE_PACKET_SIZE, "composite packet size");

	// If we hit the end, loop
	if (NextIdx >= sizeof(buffer) / sizeof(buffer[0])) {
		NextIdx = 0;
	}
}

void LogTriggerTooth(trigger_event_e tooth, efitick_t timestamp) {
	// bail if we aren't enabled
	if (!ToothLoggerEnabled) {
		return;
	}

	switch (tooth) {
	case SHAFT_PRIMARY_FALLING:
		trigger1 = false;
		break;
	case SHAFT_PRIMARY_RISING:
		trigger1 = true;
		break;
	case SHAFT_SECONDARY_FALLING:
		trigger2 = false;
		break;
	case SHAFT_SECONDARY_RISING:
		trigger2 = true;
		break;
	default:
		break;
	}

	uint32_t nowUs = NT2US(timestamp);
	SetNextEntry(nowUs, trigger1, trigger2);
}

void EnableToothLogger() {
	// Clear the buffer
	memset(buffer, 0, sizeof(buffer));

	// Reset the last edge to now - this prevents the first edge logged from being bogus
	lastEdgeTimestamp = getTimeNowUs();

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

#endif /* EFI_TOOTH_LOGGER */
