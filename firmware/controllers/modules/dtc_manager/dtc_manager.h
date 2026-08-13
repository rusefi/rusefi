/*
 * @file dtc_manager.h
 *
 * @date Jul 22, 2026
 * @author Andrey Gusakov
 */

#pragma once

#include "engine_module.h"
#include "tooth_logger_buffer.h"
#include "thread_controller.h"

#if EFI_UNIT_TEST || EFI_SIMULATOR || !EFI_FILE_LOGGING

class DtcManagerModule : public EngineModule {
};

#else

#if (EFI_TOOTH_LOGGER_STATICBUFFER_COUNT == 0) && MODULE_DTC_MANAGER
	#error "MODULE_DTC_MANAGER needs EFI_TOOTH_LOGGER_STATICBUFFER_COUNT > 0"
#endif

/**
 * Empty adapter class
 *
 * TODO:
 * Incapsulating whole DtcManager into Engine through Engine module cause out of RAM issue on some F4 boards
 * where engine is located in CCM memory.
 * Also DTC manager does not utilize any of EngineModule interfaces/callbacks so we just keep this empty
 * module for compatibility.
 */
class DtcManagerModule : public EngineModule {
};

int DtcManagerStart(FIL *p_fd, FileBufferedWriter *p_writer);
int DtcManagerStop(void);

#endif

// Triggers dump of the circular buffer to a file; a no-op stub in builds
// without circular background logging
// TODO: better name?
int DtcTriggerEvent(const char *reason, ObdCode code = ObdCode::None, efitick_t timestamp = getTimeNowNt());

int DtcToothLoggerFilled(efitick_t timestamp = getTimeNowNt());
