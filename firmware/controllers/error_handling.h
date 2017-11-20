/**
 * @file	error_handling.h
 *
 * @date Mar 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ERROR_HANDLING_H_
#define ERROR_HANDLING_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "global.h"
#include "obd_error_codes.h"
#include "efifeatures.h"
#include "stdbool.h"
#include "rusefi_types.h"

void addWarningCode(obd_code_e code);

/**
 * Something is wrong, but we can live with it: some minor sensor is disconnected
 * or something like that
 *
 * see also firmwareError()
 */
bool warning(obd_code_e code, const char *fmt, ...);

typedef uint8_t fatal_msg_t[200];
/**
 * Something really bad had happened - firmware cannot function, we cannot run the engine
 *
 * see also warning()
 */
void firmwareError(obd_code_e code, const char *fmt, ...);

#define hasFirmwareError() hasFirmwareErrorFlag

bool isWarningNow(efitimesec_t now, bool forIndicator);
// todo: rename to getFatalErrorMessage
char *getFirmwareError(void);

void initErrorHandling(void);
// todo: rename to getWarningMessage?
char *getWarning(void);

// todo: better place for this shared declaration?
int getRusEfiVersion(void);

/**
 * @deprecated Global panic is inconvenient because it's hard to deliver the error message while whole instance
 * is stopped. Please use firmwareWarning() instead
 */
#if EFI_ENABLE_ASSERTS
  #define efiAssert(condition, message, result) { if (!(condition)) { firmwareError(CUSTOM_ERR_ASSERT, message); return result; } }
  #define efiAssertVoid(condition, message) { if (!(condition)) { firmwareError(CUSTOM_ERR_ASSERT_VOID, message); return; } }
#else /* EFI_ENABLE_ASSERTS */
  #define efiAssert(condition, message, result) { }
  #define efiAssertVoid(condition, message) { }
#endif /* EFI_ENABLE_ASSERTS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ERROR_HANDLING_H_ */
