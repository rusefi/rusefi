/*
 * @file tooth_logger.h
 *
 * @date Jul 7, 2019
 * @author Matthew Kennedy
 */

#pragma once

#include "rusefi_enums.h"
#include <rusefi/expected.h>
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

struct CompositeBuffer {
	composite_logger_s buffer[toothLoggerEntriesPerBuffer];
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
int ToothLoggerWriteCsv(Writer &writer, CompositeBuffer* buffer);

#endif
