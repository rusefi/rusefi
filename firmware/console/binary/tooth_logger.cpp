/*
 * @file tooth_logger.cpp
 *
 * @date Jul 7, 2019
 * @author Matthew Kennedy
 */

#include "pch.h"

#include "tooth_logger.h"

#if EFI_TOOTH_LOGGER

typedef struct __attribute__ ((packed)) {
    uint16_t timestamp;
} tooth_logger_s;

typedef struct __attribute__ ((packed)) {
	// the whole order of all packet bytes is reversed, not just the 'endian-swap' integers
	uint32_t timestamp;
	// unfortunately all these fields are required by TS...
	bool priLevel : 1;
	bool secLevel : 1;
	bool trigger : 1;
	bool sync : 1;
	bool coil : 1;
	bool injector : 1;
} composite_logger_s;

/**
 * Engine idles around 20Hz and revs up to 140Hz, at 60/2 and 8 cylinders we have about 20Khz events
 * If we can read buffer at 50Hz we want buffer to be about 400 elements.
 */
static composite_logger_s buffer[COMPOSITE_PACKET_COUNT] CCM_OPTIONAL;
static composite_logger_s *ptr_buffer_first = &buffer[0];
static composite_logger_s *ptr_buffer_second = &buffer[(COMPOSITE_PACKET_COUNT/2)-1];
static size_t NextIdx = 0;
static volatile bool ToothLoggerEnabled = false;
static volatile bool firstBuffer = true;
static uint32_t lastEdgeTimestamp = 0;

static bool currentTrigger1 = false;
static bool currentTrigger2 = false;
static bool currentTdc = false;
// any coil, all coils thrown together
static bool currentCoilState = false;
// same about injectors
static bool currentInjectorState = false;

int getCompositeRecordCount() {
	return NextIdx;
}


#if EFI_UNIT_TEST
#include "logicdata.h"
int copyCompositeEvents(CompositeEvent *events) {
	for (size_t i = 0; i < NextIdx; i++) {
		CompositeEvent *event = &events[i];
		event->timestamp = SWAP_UINT32(buffer[i].timestamp);
		event->primaryTrigger = buffer[i].priLevel;
		event->secondaryTrigger = buffer[i].secLevel;
		event->isTDC = buffer[i].trigger;
		event->sync = buffer[i].sync;
		event->coil = buffer[i].coil;
		event->injector = buffer[i].injector;
	}
	return NextIdx;
}

#endif // EFI_UNIT_TEST

static void setToothLogReady(bool value) {
#if EFI_TUNER_STUDIO
	tsOutputChannels.toothLogReady = value;
#endif // EFI_TUNER_STUDIO
}

static void SetNextCompositeEntry(efitick_t timestamp) {
	uint32_t nowUs = NT2US(timestamp);
	
	// TS uses big endian, grumble
	buffer[NextIdx].timestamp = SWAP_UINT32(nowUs);
	buffer[NextIdx].priLevel = currentTrigger1;
	buffer[NextIdx].secLevel = currentTrigger2;
	buffer[NextIdx].trigger = currentTdc;
	buffer[NextIdx].sync = engine->triggerCentral.triggerState.getShaftSynchronized();
	buffer[NextIdx].coil = currentCoilState;
	buffer[NextIdx].injector = currentInjectorState;

	NextIdx++;

	static_assert(sizeof(composite_logger_s) == COMPOSITE_PACKET_SIZE, "composite packet size");

	//If we hit the end, loop
	if ((firstBuffer) && (NextIdx >= (COMPOSITE_PACKET_COUNT/2))) {
		/* first half is full */
		setToothLogReady(true);
		firstBuffer = false;
	}
	if ((!firstBuffer) && (NextIdx >= sizeof(buffer) / sizeof(buffer[0]))) {
		setToothLogReady(true);
		NextIdx = 0;
		firstBuffer = true;
	}

}

void LogTriggerTooth(trigger_event_e tooth, efitick_t timestamp) {
	// bail if we aren't enabled
	if (!ToothLoggerEnabled) {
		return;
	}

	// Don't log at significant engine speed
	if (engine->rpmCalculator.getRpm() > CONFIG(engineSnifferRpmThreshold)) {
		return;
	}

	ScopePerf perf(PE::LogTriggerTooth);

/*
		// We currently only support the primary trigger falling edge
    	// (this is the edge that VR sensors are accurate on)
    	// Since VR sensors are the most useful case here, this is okay for now.
    	if (tooth != SHAFT_PRIMARY_FALLING) {
    		return;
    	}

    	uint32_t nowUs = NT2US(timestamp);
    	// 10us per LSB - this gives plenty of accuracy, yet fits 655.35 ms in to a uint16
    	uint16_t delta = static_cast<uint16_t>((nowUs - lastEdgeTimestamp) / 10);
    	lastEdgeTimestamp = nowUs;

    	SetNextEntry(delta);
*/

	switch (tooth) {
	case SHAFT_PRIMARY_FALLING:
		currentTrigger1 = false;
		break;
	case SHAFT_PRIMARY_RISING:
		currentTrigger1 = true;
		break;
	case SHAFT_SECONDARY_FALLING:
		currentTrigger2 = false;
		break;
	case SHAFT_SECONDARY_RISING:
		currentTrigger2 = true;
		break;
// major hack to get most value of limited logic data write
#if EFI_UNIT_TEST
	case SHAFT_3RD_FALLING:
		currentCoilState = false;
		break;
	case SHAFT_3RD_RISING:
		currentCoilState = true;
		break;
#endif
	default:
		break;
	}

	SetNextCompositeEntry(timestamp);
}

void LogTriggerTopDeadCenter(efitick_t timestamp) {
	// bail if we aren't enabled
	if (!ToothLoggerEnabled) {
		return;
	}
	currentTdc = true;
	SetNextCompositeEntry(timestamp);
	currentTdc = false;
	SetNextCompositeEntry(timestamp + 10);
}

void LogTriggerCoilState(efitick_t timestamp, bool state) {
	if (!ToothLoggerEnabled) {
		return;
	}
	currentCoilState = state;
	UNUSED(timestamp);
	//SetNextCompositeEntry(timestamp, trigger1, trigger2, trigger);
}

void LogTriggerInjectorState(efitick_t timestamp, bool state) {
	if (!ToothLoggerEnabled) {
		return;
	}
	currentInjectorState = state;
	UNUSED(timestamp);
	//SetNextCompositeEntry(timestamp, trigger1, trigger2, trigger);
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


	setToothLogReady(false);
}

void EnableToothLoggerIfNotEnabled() {
	if (!ToothLoggerEnabled) {
		EnableToothLogger();
	}
}

void DisableToothLogger() {
	ToothLoggerEnabled = false;
	setToothLogReady(false);
}

ToothLoggerBuffer GetToothLoggerBuffer() {
	// tell TS that we do not have data until we have data again
	setToothLogReady(false);
	if (firstBuffer) {
		return { reinterpret_cast<uint8_t*>(ptr_buffer_second), (sizeof(buffer)/2) };
	} else {
		return { reinterpret_cast<uint8_t*>(ptr_buffer_first), (sizeof(buffer)/2) };
	}
}


#endif /* EFI_TOOTH_LOGGER */
