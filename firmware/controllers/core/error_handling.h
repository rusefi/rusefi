/**
 * @file	error_handling.h
 *
 * @date Mar 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "obd_error_codes.h"
#include "generated_lookup_meta.h"
#include <cstdint>

/**
 * Something is wrong, but we can live with it: some minor sensor is disconnected
 * or something like that
 *
 * see also firmwareError()
 */
bool warning(ObdCode code, const char *fmt, ...);

/**
 * Same as above, but also report to user by pop-up window in TunerStudio
 */
bool warningTsReport(ObdCode code, const char *fmt, ...);

using critical_msg_t = char[CRITICAL_BUFFER_SIZE];

#define criticalShutdown() \
    TURN_FATAL_LED(); \
    turnAllPinsOff();

/**
 * Something really bad had happened - firmware cannot function, we cannot run the engine
 * We definitely use this critical error approach in case of invalid configuration. If user sets a self-contradicting
 * configuration we have to just put a hard stop on this.
 *
 * see also warning()
 */
void firmwareError(ObdCode code, const char *fmt, ...);

#define criticalError(...) firmwareError(ObdCode::OBD_PCM_Processor_Fault, __VA_ARGS__)

extern bool hasCriticalFirmwareErrorFlag;

#define hasFirmwareError() hasCriticalFirmwareErrorFlag

const char* getCriticalErrorMessage();

// report recoverable configuration error
void configError(const char *fmt, ...);
void clearConfigErrorMessage();
const char* getConfigErrorMessage();
bool hasConfigError();
bool hasErrorReportFile();

// todo: better place for this shared declaration?
int getRusEfiVersion();

#if EFI_ENABLE_ASSERTS
  #define efiAssert(code, condition, message, result) { if (!(condition)) { firmwareError(code, message); return result; } }
  #define efiAssertVoid(code, condition, message) { if (!(condition)) { firmwareError(code, message); return; } }
#else /* EFI_ENABLE_ASSERTS */
  #define efiAssert(code, condition, message, result) { UNUSED(code);UNUSED(condition);UNUSED(message);UNUSED(result); }
  #define efiAssertVoid(code, condition, message) { UNUSED(condition);UNUSED(message);}
#endif /* EFI_ENABLE_ASSERTS */

#define criticalAssert(condition, message, result) efiAssert(ObdCode::OBD_PCM_Processor_Fault, condition, message, result)
#define criticalAssertVoid(condition, message) efiAssertVoid(ObdCode::OBD_PCM_Processor_Fault, condition, message)

//todo?#define GET_MACRO(_0, _1, _2, NAME, ...) NAME
//todo?  #define FOO(...) GET_MACRO(_0, ##__VA_ARGS__, FOO2, FOO1, FOO0)(__VA_ARGS__)

#define assertNotNullVoid(pointer) efiAssertVoid(ObdCode::OBD_PCM_Processor_Fault, pointer != nullptr, "unexpected nullptr")
#define assertNotNull(pointer, result) efiAssert(ObdCode::OBD_PCM_Processor_Fault, pointer != nullptr, "unexpected nullptr", result)

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#if EFI_PROD_CODE

// for port_extctx
#include "ch.h"

// These use very specific values to avoid interpreting random garbage memory as a real value
enum class ErrorCookie : uint32_t {
    None = 0,
    FirmwareError = 0xcafebabe,
    HardFault = 0xdeadbeef,
    ChibiOsPanic = 0xdeadfa11,
};

const char *errorCookieToName(ErrorCookie cookie);

// Error handling/recovery/reporting information

#define ERROR_STACK_DEPTH   96

typedef struct {
    ErrorCookie Cookie;

    critical_msg_t msg;
    critical_msg_t file;
    int line;
    port_extctx FaultCtx;
    uint32_t FaultType;
    uint32_t FaultAddress;
    uint32_t Csfr;
    uint32_t sp;
    uint32_t stack[ERROR_STACK_DEPTH];
} backupErrorState;

// reads backup ram and checks for any error report
void errorHandlerInit();
// true if we just started from some crash
bool errorHandlerIsStartFromError();
// If there was an error on the last boot, print out information about it now and reset state.
void errorHandlerShowBootReasonAndErrors();

//void errorHandlerWriteReportFile(FIL *fd);

#endif // EFI_PROD_CODE

#ifdef __cplusplus
}
#endif /* __cplusplus */

// todo log red LED reason on SD card #7290 bool isRuntimeCritical(ObdCode error);
