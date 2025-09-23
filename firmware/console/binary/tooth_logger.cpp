/*
 * @file tooth_logger.cpp
 *
 * At least some of the code here is related to xxx.teeth files
 * See also misc\tooth_log_converter\log_convert.cpp
 *
 * @date Jul 7, 2019
 * @author Matthew Kennedy
 */

#include "pch.h"

#if EFI_TOOTH_LOGGER
#if !EFI_SHAFT_POSITION_INPUT
	fail("EFI_SHAFT_POSITION_INPUT required to have EFI_EMULATE_POSITION_SENSORS")
#endif

/**
 * Engine idles around 20Hz and revs up to 140Hz, at 60/2 and 8 cylinders we have about 20Khz events
 * If we can read buffer at 50Hz we want buffer to be about 400 elements.
 */

static_assert(sizeof(composite_logger_s) == COMPOSITE_PACKET_SIZE, "composite packet size");

static volatile bool ToothLoggerEnabled = false;
//static uint32_t lastEdgeTimestamp = 0;

static bool currentTrigger1 = false;
static bool currentTrigger2 = false;
static bool currentTdc = false;
// any coil, all coils thrown together
static bool currentCoilState = false;
// same about injectors
static bool currentInjectorState = false;

#if EFI_UNIT_TEST

void jsonTraceEntry(const char* name, int pid, bool isEnter, efitick_t timestamp) {
extern FILE *jsonTrace;
  if (jsonTrace != nullptr) {
    fprintf(jsonTrace, ",\n");
    fprintf(jsonTrace, "{\"name\":\"%s\",\"ph\":\"%s\",\"tid\":0,\"pid\":%d,\"ts\":%f}",
      name,
      isEnter ? "B" : "E",
      pid,
    timestamp / 1000.0);
  }
}

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

static constexpr size_t BUFFER_COUNT = BIG_BUFFER_SIZE / sizeof(CompositeBuffer);
static_assert(BUFFER_COUNT >= 2);

static CompositeBuffer* buffers = nullptr;
static chibios_rt::Mailbox<CompositeBuffer*, BUFFER_COUNT> freeBuffers CCM_OPTIONAL;
static chibios_rt::Mailbox<CompositeBuffer*, BUFFER_COUNT> filledBuffers CCM_OPTIONAL;

static CompositeBuffer* currentBuffer = nullptr;

static void setToothLogReady(bool value) {
#if EFI_TUNER_STUDIO && (EFI_PROD_CODE || EFI_SIMULATOR)
	engine->outputChannels.toothLogReady = value;
#endif // EFI_TUNER_STUDIO
}

static BigBufferHandle bufferHandle;

void EnableToothLogger() {
	chibios_rt::CriticalSectionLocker csl;

	bufferHandle = getBigBuffer(BigBufferUser::ToothLogger);
	if (!bufferHandle) {
		return;
	}

	buffers = bufferHandle.get<CompositeBuffer>();

	// Reset all buffers
	for (size_t i = 0; i < BUFFER_COUNT; i++) {
		buffers[i].nextIdx = 0;
	}

	// Reset state
	currentBuffer = nullptr;

	// Empty the filled buffer list
	CompositeBuffer* dummy;
	while (MSG_TIMEOUT != filledBuffers.fetchI(&dummy)) ;

	// Put all buffers in the free list
	for (size_t i = 0; i < BUFFER_COUNT; i++) {
		freeBuffers.postI(&buffers[i]);
	}

	// Reset the last edge to now - this prevents the first edge logged from being bogus
	//lastEdgeTimestamp = getTimeNowUs();

	// Enable logging of edges as they come
	ToothLoggerEnabled = true;

	setToothLogReady(false);
}

void DisableToothLogger() {
	chibios_rt::CriticalSectionLocker csl;

	ToothLoggerEnabled = false;
	setToothLogReady(false);

	// Release the big buffer for another user
	// C++ magic: here we are calling BigBufferHandle::operator=() with empty instance
	bufferHandle = {};
	buffers = nullptr;
}

static CompositeBuffer* GetToothLoggerBufferImpl(sysinterval_t timeout) {
	CompositeBuffer* buffer;
	msg_t msg = filledBuffers.fetch(&buffer, timeout);

	if (msg == MSG_TIMEOUT) {
		setToothLogReady(false);
		return nullptr;
	}

	if (msg != MSG_OK) {
		// What even happened if we didn't get timeout, but also didn't get OK?
		return nullptr;
	}

	chibios_rt::CriticalSectionLocker csl;

	// If the used list is empty, clear the ready flag
	if (filledBuffers.getUsedCountI() == 0) {
		setToothLogReady(false);
	}

	return buffer;
}

CompositeBuffer* GetToothLoggerBufferNonblocking() {
	return GetToothLoggerBufferImpl(TIME_IMMEDIATE);
}

CompositeBuffer* GetToothLoggerBufferBlocking() {
	return GetToothLoggerBufferImpl(TIME_INFINITE);
}

void ReturnToothLoggerBuffer(CompositeBuffer* buffer) {
	chibios_rt::CriticalSectionLocker csl;

	msg_t msg = freeBuffers.postI(buffer);
	criticalAssertVoid(msg == MSG_OK, "Composite logger post to free buffer fail");
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
		buffer->nextIdx = 0;

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

	size_t idx = buffer->nextIdx;
	auto nextIdx = idx + 1;
	buffer->nextIdx = nextIdx;

	if (idx < efi::size(buffer->buffer)) {
		composite_logger_s* entry = &buffer->buffer[idx];

		uint32_t nowUs = NT2US(timestamp);

		// TS uses big endian, grumble
		entry->timestamp = SWAP_UINT32(nowUs);
		entry->priLevel = currentTrigger1;
		entry->secLevel = currentTrigger2;
		entry->trigger = currentTdc;
		entry->sync = engine->triggerCentral.triggerState.getShaftSynchronized();
		entry->coil = currentCoilState;
		entry->injector = currentInjectorState;
	}

	// if the buffer is full...
	bool bufferFull = nextIdx >= efi::size(buffer->buffer);
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

#endif // not EFI_UNIT_TEST

#define JSON_TRG_PID 4
#define JSON_CAM_PID 10

void LogTriggerSync(bool isSync, efitick_t timestamp) {
#if EFI_UNIT_TEST
	jsonTraceEntry("sync", 3, /*isEnter*/isSync, timestamp);
#else
	UNUSED(isSync); UNUSED(timestamp);
#endif
}

void LogTriggerCamTooth(bool isRising, efitick_t timestamp, int index) {
#if EFI_UNIT_TEST
	jsonTraceEntry("cam", JSON_CAM_PID + index, /*isEnter*/isRising, timestamp);
#else
	UNUSED(isRising); UNUSED(timestamp); UNUSED(index);
#endif
}

void LogTriggerTooth(trigger_event_e tooth, efitick_t timestamp) {
#if EFI_UNIT_TEST
	if (tooth == SHAFT_PRIMARY_RISING) {
		jsonTraceEntry("trg0", JSON_TRG_PID, /*isEnter*/true, timestamp);
	} else if (tooth == SHAFT_PRIMARY_FALLING) {
		jsonTraceEntry("trg0", JSON_TRG_PID, /*isEnter*/false, timestamp);
	}
#endif // EFI_UNIT_TEST

    efiAssertVoid(ObdCode::CUSTOM_ERR_6650, hasLotsOfRemainingStack(), "l-t-t");
	// bail if we aren't enabled
	if (!ToothLoggerEnabled) {
		return;
	}

	// Don't log at significant engine speed
	if (!getTriggerCentral()->isEngineSnifferEnabled) {
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

void LogTriggerCoilState(efitick_t timestamp, size_t index, bool state) {
#if EFI_UNIT_TEST
	jsonTraceEntry("coil", 20 + index, state, timestamp);
#endif // EFI_UNIT_TEST
	if (!ToothLoggerEnabled) {
		return;
	}
	currentCoilState = state;
	UNUSED(timestamp); UNUSED(index);
	//SetNextCompositeEntry(timestamp, trigger1, trigger2, trigger);
}

void LogTriggerInjectorState(efitick_t timestamp, size_t index, bool state) {
#if EFI_UNIT_TEST
	jsonTraceEntry("inj", 30 + index, state, timestamp);
#endif // EFI_UNIT_TEST
	if (!ToothLoggerEnabled) {
		return;
	}
	currentInjectorState = state;
	UNUSED(timestamp); UNUSED(index);
	//SetNextCompositeEntry(timestamp, trigger1, trigger2, trigger);
}

void EnableToothLoggerIfNotEnabled() {
	if (!ToothLoggerEnabled) {
		EnableToothLogger();
	}
}

bool IsToothLoggerEnabled() {
	return ToothLoggerEnabled;
}

#endif /* EFI_TOOTH_LOGGER */
