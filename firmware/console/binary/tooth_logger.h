/*
 * @file tooth_logger.h
 *
 * @date Jul 7, 2019
 * @author Matthew Kennedy
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include "efitime.h"
#include "rusefi_enums.h"
#include "engine_ptr.h"

#if EFI_UNIT_TEST
#include "logicdata.h"
int copyCompositeEvents(CompositeEvent *events);
#endif // EFI_UNIT_TEST

int getCompositeRecordCount();

void EnableToothLoggerIfNotEnabled();

// Enable the tooth logger - this clears the buffer starts logging
void EnableToothLogger();

// Stop logging - leave buffer intact
void DisableToothLogger();

// A new tooth has arrived! Log to the buffer if enabled.
void LogTriggerTooth(trigger_event_e tooth, efitick_t timestamp DECLARE_ENGINE_PARAMETER_SUFFIX);

void LogTriggerTopDeadCenter(efitick_t timestamp DECLARE_ENGINE_PARAMETER_SUFFIX);

void LogTriggerCoilState(efitick_t timestamp, bool state DECLARE_ENGINE_PARAMETER_SUFFIX);

void LogTriggerInjectorState(efitick_t timestamp, bool state DECLARE_ENGINE_PARAMETER_SUFFIX);

struct ToothLoggerBuffer
{
	const uint8_t* const Buffer;
	const size_t Length;
};

// Get a reference to the buffer
ToothLoggerBuffer GetToothLoggerBuffer();
