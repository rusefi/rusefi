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

// current state
static composite_logger_s cur;

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
	event.primaryTrigger = cur.priLevel;
	event.secondaryTrigger = cur.cam1;
	event.isTDC = cur.tdc;
	event.sync = engine->triggerCentral.triggerState.getShaftSynchronized();
	event.coil = cur.coil;
	event.injector = cur.injector;

	events.push_back(event);
}

bool EnableToothLogger() {
	ToothLoggerEnabled = true;
	events.clear();

	return ToothLoggerEnabled;
}

void DisableToothLogger() {
	ToothLoggerEnabled = false;
}

#else // not EFI_UNIT_TEST

static constexpr size_t bufferCount = BIG_BUFFER_SIZE / sizeof(CompositeBuffer);
static_assert(bufferCount >= 2);

static CompositeBuffer* buffers = nullptr;
static chibios_rt::Mailbox<CompositeBuffer*, bufferCount> freeBuffers;
static chibios_rt::Mailbox<CompositeBuffer*, bufferCount> filledBuffers;

static CompositeBuffer* currentBuffer = nullptr;

static void setToothLogReady(bool value) {
#if EFI_TUNER_STUDIO && (EFI_PROD_CODE || EFI_SIMULATOR)
	engine->outputChannels.toothLogReady = value;
#endif // EFI_TUNER_STUDIO
}

static BigBufferHandle bufferHandle;

bool EnableToothLogger() {
	chibios_rt::CriticalSectionLocker csl;

	bufferHandle = getBigBuffer(BigBufferUser::ToothLogger);
	if (!bufferHandle) {
		return false;
	}

	buffers = bufferHandle.get<CompositeBuffer>();

	// Reset all buffers
	for (size_t i = 0; i < bufferCount; i++) {
		buffers[i].nextIdx = 0;
	}

	// Reset state
	currentBuffer = nullptr;

	// Empty the filled buffer list
	CompositeBuffer* dummy;
	while (MSG_TIMEOUT != filledBuffers.fetchI(&dummy))
		;

	// Put all buffers in the free list
	for (size_t i = 0; i < bufferCount; i++) {
		freeBuffers.postI(&buffers[i]);
	}

	// Enable logging of edges as they come
	ToothLoggerEnabled = true;

	setToothLogReady(false);

	return true;
}

void DisableToothLogger() {
	chibios_rt::CriticalSectionLocker csl;

	// Release the big buffer for another user
	// C++ magic: here we are calling BigBufferHandle::operator=() with empty instance
	bufferHandle = {};
	buffers = nullptr;

	ToothLoggerEnabled = false;
	setToothLogReady(false);
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
	efiAssertVoid(ObdCode::OBD_PCM_Processor_Fault, msg == MSG_OK, "Composite logger post to free buffer fail");

	// If the used list is empty, clear the ready flag
	if (filledBuffers.getUsedCountI() == 0) {
		setToothLogReady(false);
	}
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

		entry->x = cur.x;
		// TODO: why?
		entry->sync = engine->triggerCentral.triggerState.getShaftSynchronized();
		entry->timestamp = NT2US(timestamp);

		// TS uses big endian, grumble
		// the whole order of all packet bytes is reversed, not just the 'endian-swap' integers
		// swap whole record byteorder
		entry->x = SWAP_UINT64(entry->x);
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

static void LogTriggerTooth(efitick_t timestamp) {
	// bail if we aren't enabled
	if (!ToothLoggerEnabled) {
		return;
	}

	// Don't log at significant engine speed
	if (!getTriggerCentral()->isEngineSnifferEnabled) {
		return;
	}

	ScopePerf perf(PE::LogTriggerTooth);

	SetNextCompositeEntry(timestamp);
}

void LogPrimaryTriggerTooth(efitick_t timestamp, bool state) {
	cur.trigger = false;
	cur.priLevel = state;

	LogTriggerTooth(timestamp);

#if EFI_UNIT_TEST
	jsonTraceEntry("trg0", JSON_TRG_PID, /*isEnter*/state, timestamp);
#endif // EFI_UNIT_TEST
}

void LogCamTriggerTooth(efitick_t timestamp, int camIndex, bool state) {
	if (camIndex < 4) {
		cur.trigger = true;

		if (camIndex == 0) {
			cur.cam1 = state;
		} else if (camIndex == 1) {
			cur.cam2 = state;
		} else if (camIndex == 2) {
			cur.cam3 = state;
		} else if (camIndex == 3) {
			cur.cam4 = state;
		}

		LogTriggerTooth(timestamp);
	}

#if EFI_UNIT_TEST
	jsonTraceEntry("cam", JSON_CAM_PID + camIndex, /*isEnter*/state, timestamp);
#endif
}

void LogTriggerTopDeadCenter(efitick_t timestamp) {
	cur.tdc = true;
	LogTriggerTooth(timestamp);
	cur.tdc = false;
	LogTriggerTooth(timestamp + 10);
}

void LogTriggerSync(efitick_t timestamp, bool isSync) {
#if EFI_UNIT_TEST
	jsonTraceEntry("sync", 3, /*isEnter*/isSync, timestamp);
#else
	UNUSED(isSync); UNUSED(timestamp);
#endif
}

void LogTriggerCoilState(efitick_t timestamp, size_t index, bool state) {
	if (index < 8) {
		if (state) {
			cur.coil |= (1 << index);
		} else {
			cur.coil &= ~(1 << index);
		}

		LogTriggerTooth(timestamp);
	}

#if EFI_UNIT_TEST
	jsonTraceEntry("coil", 20 + index, state, timestamp);
#endif // EFI_UNIT_TEST
}

void LogTriggerInjectorState(efitick_t timestamp, size_t index, bool state) {
	if (index < 8) {
		if (state) {
			cur.injector |= (1 << index);
		} else {
			cur.injector &= ~(1 << index);
		}

		LogTriggerTooth(timestamp);
	}

#if EFI_UNIT_TEST
	jsonTraceEntry("inj", 30 + index, state, timestamp);
#endif // EFI_UNIT_TEST
}

bool EnableToothLoggerIfNotEnabled() {
	if (!ToothLoggerEnabled) {
		ToothLoggerEnabled = EnableToothLogger();
	}

	return ToothLoggerEnabled;
}

bool IsToothLoggerEnabled() {
	return ToothLoggerEnabled;
}

#endif /* EFI_TOOTH_LOGGER */
