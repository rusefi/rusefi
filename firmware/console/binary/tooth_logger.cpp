/*
 * @file tooth_logger.cpp
 *
 * logic analyzer, not to be confused with "trigger oscilloscope"
 * also not to be confused with engine_sniffer.cpp - do we have some overlap between generations of tools?

 // timestamps: any event adds an event / line to CSV
 // TDC: toggled on 1st cylinder TDC
 // sync:
 // coils: bitfield of first 8 cylinders
 // Injectors: same as above

 at the moment CSV is tightly coupled with TS composite & primary tooth loggers

 * this file also appends to unit test JSON, what a mess!

 *
 * At least some of the code here is related to xxx.teeth files
 *
 * .teeth file lifecycle (see firmware/hw_layer/mmc_card.cpp sdLoggerTooth()):
 *   - A .teeth file is written instead of the regular .mlg log only while
 *     engineConfiguration->sdTriggerLog is enabled (see prepareLogFileName()).
 *   - No file exists until tooth data is actually available: sdLoggerTooth()
 *     waits until ToothLoggerHasData() is true, then opens a brand new file.
 *   - Each new file therefore starts fresh with the current/default logger
 *     settings - the composite logger state (see 'cur' below) begins empty and
 *     the header is written at offset zero, no state carries over from the
 *     previous file.
 *   - A file is closed (and the next tooth event begins yet another new file)
 *     when either a write error occurs or the logger goes idle: ToothLoggerWriter()
 *     blocks up to 3 seconds waiting for a tooth event (filledBuffers.fetch with
 *     TIME_MS2I(3000)); on timeout it flushes the partially-filled buffer and
 *     signals a new file. Note there is NO 32MB size cap on .teeth files - the
 *     LOGGER_MAX_FILE_SIZE check in mmc_card.cpp applies only to regular .mlg logs.
 *
 * TODO: remove legacy 'binary' format since we have CSV now?
 * See also misc\tooth_log_converter\log_convert.cpp
 *
 * @date Jul 7, 2019
 * @author Matthew Kennedy
 */

#include "pch.h"

#if EFI_TOOTH_LOGGER
#if !EFI_SHAFT_POSITION_INPUT
	fail("EFI_SHAFT_POSITION_INPUT required to have EFI_TOOTH_LOGGER")
#endif

#include "tooth_logger_buffer.h"

/**
 * Engine idles around 20Hz and revs up to 140Hz, at 60/2 and 8 cylinders we have about 20Khz events
 * If we can read buffer at 50Hz we want buffer to be about 400 elements.
 */

static_assert(sizeof(composite_logger_s) == COMPOSITE_PACKET_SIZE, "composite packet size");

static volatile bool ToothLoggerEnabled = false;
static TLmode ToothLoggerMode = TLmode::Full;

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
	event.sync = cur.sync;
	event.coil = cur.coil;
	event.injector = cur.injector;

	events.push_back(event);
}

bool EnableToothLogger(TLmode mode) {
	ToothLoggerEnabled = true;
	events.clear();

	return ToothLoggerEnabled;
}

void DisableToothLogger() {
	ToothLoggerEnabled = false;
}

#else // not EFI_UNIT_TEST

// The buffer lifecycle itself (free/filled queues, current buffer, 5 second
// staleness flush) lives in ToothLoggerBufferPool - see tooth_logger_buffer.h.
// This file owns the enabled flag, the current flag state 'cur', and the
// TS-visible ready indication.

static void setToothLogReady(bool value) {
#if EFI_TUNER_STUDIO && (EFI_PROD_CODE || EFI_SIMULATOR)
	engine->outputChannels.toothLogReady = value;
#endif // EFI_TUNER_STUDIO
}

static ToothLoggerBufferPool toothBuffers{setToothLogReady};

bool EnableToothLogger(TLmode mode) {
	chibios_rt::CriticalSectionLocker csl;

	if (!toothBuffers.startI()) {
		return false;
	}

	// Enable logging of edges as they come
	ToothLoggerEnabled = true;
	ToothLoggerMode = mode;

	setToothLogReady(false);

	return true;
}

void DisableToothLogger() {
	chibios_rt::CriticalSectionLocker csl;

	toothBuffers.stopI();

	ToothLoggerEnabled = false;
	setToothLogReady(false);
}

CompositeBuffer* GetToothLoggerBufferNonblocking() {
	return toothBuffers.getFilled(TIME_IMMEDIATE);
}

void ReturnToothLoggerBuffer(CompositeBuffer* buffer) {
	chibios_rt::CriticalSectionLocker csl;

	toothBuffers.returnBufferI(buffer);
}

static void SetNextCompositeEntry(efitick_t timestamp) {
	// This is called from multiple interrupts/threads, so we need a lock.
	chibios_rt::CriticalSectionLocker csl;

	toothBuffers.appendI(cur, timestamp);
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

	// in tooth mode we are interested in rising edges of primary only
	if ((ToothLoggerMode == TLmode::PrimaryTooth) &&
		(state == false)) {
		return;
	}

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

		// in tooth mode we are interested in rising edges of primary only
		if (ToothLoggerMode == TLmode::PrimaryTooth) {
			return;
		}
		LogTriggerTooth(timestamp);
	}

#if EFI_UNIT_TEST
	jsonTraceEntry("cam", JSON_CAM_PID + camIndex, /*isEnter*/state, timestamp);
#endif
}

void LogTriggerTopDeadCenter(efitick_t timestamp) {
	// in tooth mode we are interested in rising edges of primary only
	if (ToothLoggerMode == TLmode::PrimaryTooth) {
		return;
	}

	// just toggle TDC flag, this looks good on graph
	cur.tdc = !cur.tdc;
	LogTriggerTooth(timestamp);
}

void LogTriggerSync(efitick_t timestamp, bool isSync) {
	cur.sync = isSync;
	LogTriggerTooth(timestamp);;

#if EFI_UNIT_TEST
	jsonTraceEntry("sync", 3, /*isEnter*/isSync, timestamp);
#endif
}

void LogTriggerCoilState(efitick_t timestamp, size_t index, bool state) {
	if (index < 8) {
		if (state) {
			cur.coil |= (1 << index);
		} else {
			cur.coil &= ~(1 << index);
		}

		// in tooth mode we are interested in rising edges of primary only
		if (ToothLoggerMode == TLmode::PrimaryTooth) {
			return;
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

		// in tooth mode we are interested in rising edges of primary only
		if (ToothLoggerMode == TLmode::PrimaryTooth) {
			return;
		}

		LogTriggerTooth(timestamp);
	}

#if EFI_UNIT_TEST
	jsonTraceEntry("inj", 30 + index, state, timestamp);
#endif // EFI_UNIT_TEST
}

void LogTriggerAcrState(efitick_t timestamp, bool state) {
	if (cur.acr == state) {
		return;
	}
	cur.acr = state;

	// in tooth mode we are interested in rising edges of primary only
	if (ToothLoggerMode == TLmode::PrimaryTooth) {
		return;
	}

	LogTriggerTooth(timestamp);

#if EFI_UNIT_TEST
	jsonTraceEntry("acr", 40, state, timestamp);
#endif // EFI_UNIT_TEST
}

bool EnableToothLoggerIfNotEnabled(TLmode mode) {
	if (!ToothLoggerEnabled) {
		ToothLoggerEnabled = EnableToothLogger(mode);
	}

	return ToothLoggerEnabled;
}

bool IsToothLoggerEnabled() {
	return ToothLoggerEnabled;
}

#if EFI_FILE_LOGGING


static int ToothLoggerWriteBin(Writer &writer, CompositeBuffer* buffer) {
	int size = buffer->nextIdx * sizeof(composite_logger_s);

	writer.write(reinterpret_cast<const char*>(buffer->buffer), size);

	return size;
}

bool ToothLoggerHasData() {
	chibios_rt::CriticalSectionLocker csl;

	return toothBuffers.hasDataI();
}

// binary vs CSV output format, latched at file creation - see ToothLoggerWriter()
static bool sdTriggerLogCsv = 0;

/**
 * One iteration of SD card .teeth file writing, called from the SD thread
 * (sdLoggerTooth() in mmc_card.cpp): waits up to 3 seconds for a filled buffer
 * and appends it to the file, as raw binary records or CSV per sdTriggerLogCsv
 * (decided once per file, on-the-fly format change is not supported).
 *
 * @return positive number of bytes written; 0 to request the caller close the
 * file and start a new one on the next tooth event (3 second idle timeout, the
 * partially-filled current buffer is flushed first); negative on error
 */
int ToothLoggerWriter(FileBufferedWriter &writer) {
	int ret = 0;
	CompositeBuffer* buffer = nullptr;
	bool startNewFile = false;

	// manualy pick buffer, do not use getFilled() as it changes TS buffer ready flag
	msg_t msg = toothBuffers.fetchFilled(&buffer, TIME_MS2I(3000));
	if ((msg != MSG_OK) && (msg != MSG_TIMEOUT)) {
		// error?
		return -1;
	}
	if (msg == MSG_TIMEOUT) {
		chibios_rt::CriticalSectionLocker csl;
		// if we did not get any event within 3 seconds - finish current file and wait for new event.
		startNewFile = true;

		// flush data from currently writing buffer!
		buffer = toothBuffers.flushCurrentI();
	}

	// can return nullptr
	if (buffer) {
		// on-fly format change is not supported
		if (writer.size() == 0) {
			sdTriggerLogCsv = engineConfiguration->sdTriggerLogCsv;
		}
		if (sdTriggerLogCsv) {
			if (writer.size() == 0) {
				ToothLoggerWriteCsvHeader(writer);
			}
			ret = ToothLoggerWriteCsv(writer, buffer);
		} else {
			ret = ToothLoggerWriteBin(writer, buffer);
		}

		ReturnToothLoggerBuffer(buffer);
	}

	return startNewFile ? 0 : ret;
}

#endif /* EFI_FILE_LOGGING */

#if EFI_FILE_LOGGING || EFI_UNIT_TEST

int ToothLoggerWriteCsvHeader(Writer &writer) {
	// keep in sync with composite_logger_s
	// drop trigger - purpose not clear
	const char header[] = "Time[s], Primary, Cam 1, Cam 2, Cam 3, Cam 4, Sync, TDC, Coils, Injectors, ACR, VBatt, ET, InstantMAP, TPS\r\n";

	// no tailing '\0'
	writer.write(header, sizeof(header) - 1);

	return 0;
}

int ToothLoggerWriteCsv(Writer &writer, CompositeBuffer* buffer) {
	size_t total = 0;
	char tmp[128];

	for (size_t i = 0; i < buffer->nextIdx; i++) {
		// Swap back
		composite_logger_s c;
		c.x = SWAP_UINT64(buffer->buffer[i].x);

		// recover timestamp
		efitick_t raw_time = buffer->startTime.get() + USF2NT(c.timestamp);
		efitick_t time_us = NT2US(raw_time);
		uint32_t sec = time_us / 1000000;
		uint32_t usec = time_us % 1000000;

		// todo: take these data points from structure, not current values. Kind of works for slow sensors, but still!
		float vbatt = Sensor::get(SensorType::BatteryVoltage).value_or(0);
		float et = Sensor::get(SensorType::Clt).value_or(0);
		float instantMap = engine->outputChannels.instantMAPValue;
		float tps = Sensor::get(SensorType::Tps1).value_or(0);

		// it is cheaper to write all data, even we have 1 cylinder engine with single crank sensor
		int ret = chsnprintf(tmp, sizeof(tmp), "%d.%06d, "
					"%d, %d, %d, %d, %d, "
					"%d, %d, "
					"%d, %d, %d, %.2f, %.2f, %.2f, %.2f\r\n",	// TODO: convert to bitwise?
				sec, usec,
				c.priLevel, c.cam1, c.cam2, c.cam3, c.cam4,
				c.sync, c.tdc,
				c.coil, c.injector, c.acr, vbatt, et, instantMap, tps);

		if ((ret < 0) || (ret >= (int)sizeof(tmp))) {
			return -1;
		}

		ret = writer.write(tmp, ret);

		total += ret;
	}

	return total;
}

#endif /* EFI_FILE_LOGGING || EFI_UNIT_TEST */

#endif /* EFI_TOOTH_LOGGER */
