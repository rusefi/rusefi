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

static_assert(sizeof(composite_logger_s) == COMPOSITE_PACKET_SIZE, "composite packet size");

static volatile bool ToothLoggerEnabled = false;
static uint32_t lastEdgeTimestamp = 0;

static bool currentTrigger1 = false;
static bool currentTrigger2 = false;
static bool currentTdc = false;
// any coil, all coils thrown together
static bool currentCoilState = false;
// same about injectors
static bool currentInjectorState = false;

#if EFI_UNIT_TEST
#include "logicdata.h"

static std::vector<CompositeEvent> events;

const std::vector<CompositeEvent>& getCompositeEvents() {
	return events;
}

void SetNextCompositeEntry(efitick_t timestamp) {
	CompositeEvent event;

	event.timestamp = timestamp;
	event.primaryTrigger = currentTrigger1;
	event.secondaryTrigger = currentTrigger2;
	event.isTDC = currentTdc;
	event.sync = engine->triggerCentral.triggerState.getShaftSynchronized();
	event.coil = currentCoilState;
	event.injector = currentInjectorState;

	events.push_back(event);
}

void EnableToothLogger() {
	ToothLoggerEnabled = true;
	events.clear();
}

void DisableToothLogger() {
	ToothLoggerEnabled = false;
}

#else // not EFI_UNIT_TEST

static constexpr size_t bufferCount = 4;
static constexpr size_t entriesPerBuffer = COMPOSITE_PACKET_COUNT / bufferCount;

struct CompositeBuffer {
	composite_logger_s buffer[entriesPerBuffer];
	size_t nextIdx;
	Timer startTime;
};

static CompositeBuffer buffers[bufferCount] CCM_OPTIONAL;
static chibios_rt::Mailbox<CompositeBuffer*, bufferCount> freeBuffers CCM_OPTIONAL;
static chibios_rt::Mailbox<CompositeBuffer*, bufferCount> filledBuffers CCM_OPTIONAL;

static CompositeBuffer* currentBuffer = nullptr;

static void setToothLogReady(bool value) {
#if EFI_TUNER_STUDIO && (EFI_PROD_CODE || EFI_SIMULATOR)
	engine->outputChannels.toothLogReady = value;
#endif // EFI_TUNER_STUDIO
}

void EnableToothLogger() {
	chibios_rt::CriticalSectionLocker csl;

	// Reset all buffers
	for (size_t i = 0; i < efi::size(buffers); i++) {
		buffers[i].nextIdx = 0;
	}

	// Reset state
	currentBuffer = nullptr;

	// Empty the filled buffer list
	CompositeBuffer* dummy;
	while (MSG_TIMEOUT != filledBuffers.fetchI(&dummy)) ;

	// Put all buffers in the free list
	for (size_t i = 0; i < efi::size(buffers); i++) {
		freeBuffers.postI(&buffers[i]);
	}

	// Reset the last edge to now - this prevents the first edge logged from being bogus
	lastEdgeTimestamp = getTimeNowUs();

	// Enable logging of edges as they come
	ToothLoggerEnabled = true;

	setToothLogReady(false);
}

void DisableToothLogger() {
	ToothLoggerEnabled = false;
	setToothLogReady(false);
}

expected<ToothLoggerBuffer> GetToothLoggerBuffer() {
	chibios_rt::CriticalSectionLocker csl;

	CompositeBuffer* buffer;
	msg_t msg = filledBuffers.fetchI(&buffer);

	if (msg == MSG_TIMEOUT) {
		setToothLogReady(false);
		return unexpected;
	}

	if (msg != MSG_OK) {
		// What even happened if we didn't get timeout, but also didn't get OK?
		return unexpected;
	}

	size_t entryCount = buffer->nextIdx;
	buffer->nextIdx = 0;

	// Return this buffer to the free list
	msg = freeBuffers.postI(buffer);
	efiAssert(OBD_PCM_Processor_Fault, msg == MSG_OK, "Composite logger post to free buffer fail", unexpected);

	// If the used list is empty, clear the ready flag
	if (filledBuffers.getUsedCountI() == 0) {
		setToothLogReady(false);
	}

	return ToothLoggerBuffer{ reinterpret_cast<uint8_t*>(buffer->buffer), entryCount * sizeof(composite_logger_s)};
}

static CompositeBuffer* findBuffer(efitick_t timestamp) {
	CompositeBuffer* buffer;

	if (!currentBuffer) {
		// try and find a buffer, if none available, we can't log
		if (MSG_OK != freeBuffers.fetchI(&buffer)) {
			return nullptr;
		}

		// Record the time of the last buffer swap so we can force a swap after a minimum period of time
		// This ensures the user sees *something* even if they don't have enough trigger events
		// to fill the buffer.
		buffer->startTime.reset(timestamp);

		currentBuffer = buffer;
	}

	return currentBuffer;
}

static void SetNextCompositeEntry(efitick_t timestamp) {
	// This is called from multiple interrupts/threads, so we need a lock.
	chibios_rt::CriticalSectionLocker csl;

	CompositeBuffer* buffer = findBuffer(timestamp);

	if (!buffer) {
		// All buffers are full, nothing to do here.
		return;
	}

	composite_logger_s* entry = &buffer->buffer[buffer->nextIdx];

	uint32_t nowUs = NT2US(timestamp);

	// TS uses big endian, grumble
	entry->timestamp = SWAP_UINT32(nowUs);
	entry->priLevel = currentTrigger1;
	entry->secLevel = currentTrigger2;
	entry->trigger = currentTdc;
	entry->sync = engine->triggerCentral.triggerState.getShaftSynchronized();
	entry->coil = currentCoilState;
	entry->injector = currentInjectorState;

	buffer->nextIdx++;

	// if the buffer is full...
	bool bufferFull = buffer->nextIdx >= efi::size(buffer->buffer);
	// ... or it's been too long since the last flush
	bool bufferTimedOut = buffer->startTime.hasElapsedSec(5);

	// Then cycle buffers and set the ready flag.
	if (bufferFull || bufferTimedOut) {
		// Post to the output queue
		filledBuffers.postI(buffer);

		// Null the current buffer so we get a new one next time
		currentBuffer = nullptr;

		// Flag that we are ready
		setToothLogReady(true);
	}
}

#endif // EFI_UNIT_TEST

void LogTriggerTooth(trigger_event_e tooth, efitick_t timestamp) {
	// bail if we aren't enabled
	if (!ToothLoggerEnabled) {
		return;
	}

	// Don't log at significant engine speed
	if (!engine->isEngineSnifferEnabled) {
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

void EnableToothLoggerIfNotEnabled() {
	if (!ToothLoggerEnabled) {
		EnableToothLogger();
	}
}

#endif /* EFI_TOOTH_LOGGER */
