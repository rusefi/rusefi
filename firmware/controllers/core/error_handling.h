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
 * rusEFI distinguishes three kinds of errors, in order of increasing severity:
 *
 *  1) warning(...)         - Recoverable runtime condition. The engine keeps running.
 *                            Example: a minor/optional sensor is disconnected, a
 *                            transient out-of-range reading, a non-fatal CAN timeout.
 *                            Warnings are rate-limited and surfaced in the TS warning
 *                            channel. Use warningTsReport() to also pop up a TS dialog.
 *
 *  2) configError(...)     - Recoverable *configuration* problem detected at startup
 *                            or while applying a new tune. The firmware keeps running
 *                            (so the user can fix the tune over TS), but the offending
 *                            subsystem should refuse to operate. The message is kept
 *                            and exposed via getConfigErrorMessage() / hasConfigError()
 *                            so TunerStudio can show it to the user.
 *
 *  3) firmwareError(...) / criticalError(...)
 *                          - Non-recoverable: firmware cannot safely run the engine.
 *                            Sets hasCriticalFirmwareErrorFlag, latches a critical
 *                            message (getCriticalErrorMessage()), turns the fatal LED
 *                            on and shuts outputs off. Use this for self-contradicting
 *                            configuration that cannot be ignored, failed asserts, or
 *                            any condition where continuing to drive outputs would be
 *                            unsafe. criticalError() is just firmwareError() with the
 *                            OBD_PCM_Processor_Fault code pre-filled.
 *
 * Rule of thumb:
 *   - Sensor/runtime hiccup the engine can survive          -> warning()
 *   - Bad tune / setup the user must fix in TunerStudio     -> configError()
 *   - We must not keep firing injectors/coils as configured -> firmwareError()/criticalError()
 *
 * See also docs/AI/ files referenced from CLAUDE.md.
 */

/**
 * Something is wrong, but we can live with it: some minor sensor is disconnected
 * or something like that. Rate-limited; engine continues to run.
 *
 * see also configError() for bad-tune problems and firmwareError() for fatal ones.
 */
bool warning(ObdCode code, const char *fmt, ...);

/**
 * Same as warning(), but also report to the user via a pop-up window in TunerStudio.
 */
bool warningTsReport(ObdCode code, const char *fmt, ...);

using critical_msg_t = char[CRITICAL_BUFFER_SIZE];

#define criticalShutdown() \
    TURN_FATAL_LED(); \
    turnAllPinsOff();

/**
 * Something really bad has happened - firmware cannot function, we cannot run the engine.
 * Latches a critical error: sets hasCriticalFirmwareErrorFlag, stores the message
 * (retrievable via getCriticalErrorMessage()), and triggers criticalShutdown() (fatal
 * LED on, all output pins off). Used for self-contradicting configuration that we
 * refuse to run with, failed asserts, and other unrecoverable conditions.
 *
 * see also warning() (recoverable runtime) and configError() (recoverable bad tune).
 */
void firmwareError(ObdCode code, const char *fmt, ...);

// Shortcut: firmwareError() with OBD_PCM_Processor_Fault as the OBD code.
#define criticalError(...) firmwareError(ObdCode::OBD_PCM_Processor_Fault, __VA_ARGS__)

extern bool hasCriticalFirmwareErrorFlag;

#define hasFirmwareError() hasCriticalFirmwareErrorFlag

const char* getCriticalErrorMessage();

/**
 * Report a recoverable configuration error (bad/inconsistent tune).
 *
 * Surfacing: status_loop.cpp ORs hasConfigError() into
 * tsOutputChannels->hasCriticalError, and TunerStudio reads the message
 * via getConfigErrorMessage(). The flag stays set until something calls
 * clearConfigErrorMessage() (who does that when?!).
 */
void configError(const char *fmt, ...);
void clearConfigErrorMessage();
const char* getConfigErrorMessage();
bool hasConfigError();
bool hasErrorReportFile();

void errorHandlerResetCounters();

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
