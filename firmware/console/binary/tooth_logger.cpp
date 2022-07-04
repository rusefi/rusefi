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

static constexpr size_t bufferCount = 2;

struct CompositeBuffer {
	composite_logger_s buffer[bufferCount / 2];
	size_t nextIdx = 0;
};

static CompositeBuffer buffers[bufferCount] CCM_OPTIONAL;
static chibios_rt::Mailbox<CompositeBuffer*, bufferCount> freeBuffers CCM_OPTIONAL;
static chibios_rt::Mailbox<CompositeBuffer*, bufferCount> filledBuffers CCM_OPTIONAL;

static CompositeBuffer* currentBuffer = nullptr;

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
#if EFI_TUNER_STUDIO && (EFI_PROD_CODE || EFI_SIMULATOR)
	engine->outputChannels.toothLogReady = value;
#endif // EFI_TUNER_STUDIO
}

CompositeBuffer* findBuffer() {
	CompositeBuffer* buffer;

	if (!currentBuffer) {
		chibios_rt::CriticalSectionLocker csl;

		msg_t res = freeBuffers.fetchI(&buffer);

		if (res != MSG_OK) {
			return nullptr;
		}

		currentBuffer = buffer;
	}

	return buffer;
}

static void SetNextCompositeEntry(efitick_t timestamp) {
	CompositeBuffer* buffer = findBuffer();

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

	if (buffer->nextIdx >= efi::size(buffer->buffer)) {
		chibios_rt::CriticalSectionLocker csl;
		// Buffer is full!
		
		// Post to the output queue
		filledBuffers.postI(buffer);

		// Reset next idx
		buffer->nextIdx = 0;

		// Null the current buffer so we get a new one next time
		currentBuffer = nullptr;

		// Flag that we are ready
		setToothLogReady(true);
	}
}

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

void EnableToothLogger() {
	// Reset all buffers
	for (size_t i = 0; i < efi::size(buffers); i++) {
		buffers[i].nextIdx = 0;
	}

	// Reset state
	currentBuffer = nullptr;

	// Empty the filled buffer list
	CompositeBuffer* dummy;
	while (MSG_TIMEOUT != filledBuffers.fetch(&dummy, TIME_IMMEDIATE)) ;

	// Put all buffers in the free list
	for (size_t i = 0; i < efi::size(buffers); i++) {
		freeBuffers.post(&buffers[i], TIME_IMMEDIATE);
	}

	// Reset the last edge to now - this prevents the first edge logged from being bogus
	lastEdgeTimestamp = getTimeNowUs();

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
	CompositeBuffer* buffer;
	msg_t msg = filledBuffers.fetch(&buffer, TIME_IMMEDIATE);

	if (msg == MSG_TIMEOUT) {
		// Buffer is empty, what do we do here?
		return {};
	}

	if (msg != MSG_OK) {
		// What even happened if we didn't get timeout, but also didn't get OK?
	}

	// Return this buffer to the free list
	msg = freeBuffers.post(buffer, TIME_IMMEDIATE);
	efiAssert(OBD_PCM_Processor_Fault, msg == MSG_OK, "Composite logger post to free buffer fail", {});

	return { reinterpret_cast<uint8_t*>(buffer->buffer), sizeof(buffer->buffer)};
}

#endif /* EFI_TOOTH_LOGGER */
