/**
 * @file	error_handling.h
 *
 * @date Mar 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "obd_error_codes.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * Something is wrong, but we can live with it: some minor sensor is disconnected
 * or something like that
 *
 * see also firmwareError()
 */
bool warning(obd_code_e code, const char *fmt, ...);

typedef char critical_msg_t[ERROR_BUFFER_SIZE];

/**
 * Something really bad had happened - firmware cannot function, we cannot run the engine
 * We definitely use this critical error approach in case of invalid configuration. If user sets a self-contradicting
 * configuration we have to just put a hard stop on this.
 *
 * see also warning()
 */
void firmwareError(obd_code_e code, const char *fmt, ...);

extern bool hasFirmwareErrorFlag;

#define hasFirmwareError() hasFirmwareErrorFlag

// todo: rename to getCriticalErrorMessage
const char* getFirmwareError(void);
const char* getWarningMessage(void);

// todo: better place for this shared declaration?
int getRusEfiVersion(void);

/**
 * @deprecated Global panic is inconvenient because it's hard to deliver the error message while whole instance
 * is stopped. Please use firmwareWarning() instead
 */
#if EFI_ENABLE_ASSERTS
  #define efiAssert(code, condition, message, result) { if (!(condition)) { firmwareError(code, message); return result; } }
  #define efiAssertVoid(code, condition, message) { if (!(condition)) { firmwareError(code, message); return; } }
#else /* EFI_ENABLE_ASSERTS */
  #define efiAssert(code, condition, message, result) { }
  #define efiAssertVoid(code, condition, message) { }
#endif /* EFI_ENABLE_ASSERTS */

#ifdef __cplusplus
}
#endif /* __cplusplus */
