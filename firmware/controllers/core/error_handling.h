/**
 * @file	error_handling.h
 *
 * @date Mar 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "obd_error_codes.h"
#include <cstdint>

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

using critical_msg_t = char[ERROR_BUFFER_SIZE];

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

const char* getCriticalErrorMessage(void);
const char* getWarningMessage(void);

// todo: better place for this shared declaration?
int getRusEfiVersion(void);

#if EFI_ENABLE_ASSERTS
  #define efiAssert(code, condition, message, result) { if (!(condition)) { firmwareError(code, message); return result; } }
  #define efiAssertVoid(code, condition, message) { if (!(condition)) { firmwareError(code, message); return; } }
#else /* EFI_ENABLE_ASSERTS */
  #define efiAssert(code, condition, message, result) { }
  #define efiAssertVoid(code, condition, message) { }
#endif /* EFI_ENABLE_ASSERTS */

#if EFI_PROD_CODE
#include <hal.h>

// If there was an error on the last boot, print out information about it now and reset state.
void checkLastBootError();
void logHardFault(uint32_t type, uintptr_t faultAddress, port_extctx* ctx, uint32_t csfr);
#endif // EFI_PROD_CODE

#ifdef __cplusplus
}
#endif /* __cplusplus */
