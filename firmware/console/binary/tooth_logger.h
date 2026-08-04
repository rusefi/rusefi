/*
 * @file tooth_logger.h
 *
 * @date Jul 7, 2019
 * @author Matthew Kennedy
 */

#pragma once

#include "rusefi_enums.h"
#include <rusefi/expected.h>
#include <rusefi/scaled_channel.h>
#include "trigger_structure.h"

#if EFI_UNIT_TEST
#include "logicdata.h"
const std::vector<CompositeEvent>& getCompositeEvents();
void jsonTraceEntry(const char* name, int pid, bool isEnter, efitick_t timestamp);
#endif // EFI_UNIT_TEST

enum class TLmode : uint8_t {
	Full,
	PrimaryTooth
};

bool EnableToothLoggerIfNotEnabled(TLmode mode = TLmode::Full);

// Enable the tooth logger - this clears the buffer starts logging
bool EnableToothLogger(TLmode mode = TLmode::Full);

// Stop logging - leave buffer intact
void DisableToothLogger();

bool IsToothLoggerEnabled();

// A new tooth has arrived! Log to the buffer if enabled.
void LogPrimaryTriggerTooth(efitick_t timestamp, bool state);
void LogCamTriggerTooth(efitick_t timestamp, int camIndex, bool state);

void LogTriggerTopDeadCenter(efitick_t timestamp);
void LogTriggerSync(efitick_t timestamp, bool isSync);

void LogTriggerCoilState(efitick_t timestamp, size_t index, bool state);
void LogTriggerInjectorState(efitick_t timestamp, size_t index, bool state);
void LogTriggerAcrState(efitick_t timestamp, bool state);

typedef union __attribute__((packed)) {
	// the whole order of all packet bytes is reversed, not just the 'endian-swap' integers
	struct {
		uint32_t timestamp;
		// unfortunately all these fields are required by TS...
		bool priLevel : 1;
		bool cam1 : 1;
		bool trigger : 1;
		bool sync : 1;
		bool tdc : 1;
		bool cam2 : 1;
		bool cam3 : 1;
		bool cam4 : 1;
		uint8_t coil;
		uint8_t injector;
		bool acr : 1;
	};
	uint64_t x;
} composite_logger_s;

static constexpr size_t toothLoggerEntriesPerBuffer = 250;

// Per-event snapshot of the analog values rendered as the VBatt/ET/InstantMAP/
// TPS .teeth CSV columns. Sampled at append time (interrupt context) alongside
// the composite_logger_s entry so those columns reflect the event moment - a
// buffer can sit up to 5 seconds before it is flushed, so flush-time sampling
// skews them. The composite_logger_s entry itself is a TunerStudio wire-format
// contract and cannot grow, so the snapshot lives in a parallel array inside
// CompositeBuffer, present only in builds that can write CSV. Stored as scaled
// integers, not floats: the array widens CompositeBuffer, and at float width
// fewer than two buffers would fit in the BigBuffer region.
struct composite_sensor_snapshot_s {
	scaled_channel<int16_t, 100> vbatt;			// Volts
	scaled_channel<int16_t, 100> et;			// engine (coolant) temperature, deg C
	scaled_channel<uint16_t, 100> instantMap;	// kPa
	scaled_channel<int16_t, 100> tps;			// percent
};
static_assert(sizeof(composite_sensor_snapshot_s) == 8);

// Optional board-defined per-event payload, sampled at append time (interrupt
// context) by custom_board_toothLogSample and rendered into extra .teeth CSV
// columns by custom_board_toothLogCsvLine - see board_overrides.h. The
// composite_logger_s entry itself is a TunerStudio wire-format contract and
// cannot grow, so the payload lives in a parallel array inside the same
// CompositeBuffer. A board opts in by adding
// -DTOOTH_LOG_BOARD_PAYLOAD_SIZE=<bytes per event, multiple of 4> to DDEFS of
// BOTH its firmware and unit-test builds, and maps its own struct onto the
// void* payload pointers (static_assert the struct size matches). The default
// of 0 compiles the array out entirely. Note: the payload widens
// CompositeBuffer and therefore reduces how many buffers fit in the shared
// BigBuffer region (BIG_BUFFER_SIZE, overridable) - check the resulting
// buffer count covers your SD-card stall tolerance, keeping in mind that
// sensorSnapshot below already costs 8 bytes per event in EFI_FILE_LOGGING
// builds. A board combining a payload with file logging likely needs to raise
// BIG_BUFFER_SIZE to keep the bufferCount >= 2 static_assert happy.
#ifndef TOOTH_LOG_BOARD_PAYLOAD_SIZE
#define TOOTH_LOG_BOARD_PAYLOAD_SIZE 0
#endif
static_assert(TOOTH_LOG_BOARD_PAYLOAD_SIZE % 4 == 0,
	"payload must keep CompositeBuffer 4-byte aligned");

struct CompositeBuffer {
	composite_logger_s buffer[toothLoggerEntriesPerBuffer];
#if EFI_FILE_LOGGING || EFI_UNIT_TEST
	composite_sensor_snapshot_s sensorSnapshot[toothLoggerEntriesPerBuffer];
#endif
#if TOOTH_LOG_BOARD_PAYLOAD_SIZE > 0
	uint8_t boardPayload[toothLoggerEntriesPerBuffer][TOOTH_LOG_BOARD_PAYLOAD_SIZE];
#endif
	size_t nextIdx;
	Timer startTime;
};

// Require that the composite buffer be a multiple of 4 bytes long.
// The bigBuffer is guaranteed to be aligned on 4 bytes, but we need all elements to be aligned too.
// If misaligned, Timer will crash.
static_assert(sizeof(CompositeBuffer) % 4 == 0);

// Get a reference to the buffer
// Returns nullptr if no buffer is available
CompositeBuffer* GetToothLoggerBufferNonblocking();

// Return a buffer to the pool once its contents have been read
void ReturnToothLoggerBuffer(CompositeBuffer*);


#if EFI_FILE_LOGGING

#include "file_writer.h"

bool ToothLoggerHasData();
int ToothLoggerWriter(FileBufferedWriter &writer);

#endif

#if EFI_FILE_LOGGING || EFI_UNIT_TEST

#include "writer.h"

int ToothLoggerWriteCsvHeader(Writer &writer);
int ToothLoggerWriteBufferCsv(Writer &writer, CompositeBuffer* buffer, bool tail = false);

#endif
