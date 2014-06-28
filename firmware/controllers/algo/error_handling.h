/**
 * @file	error_handling.h
 *
 * @date Mar 6, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef ERROR_HANDLING_H_
#define ERROR_HANDLING_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "obd_error_codes.h"
#include "stdbool.h"

/**
 * Something is wrong, but we can live with it: some minor sensor is disconnected
 * or something like that
 *
 */
int warning(obd_code_e code, const char *fmt, ...);
/**
 * Something really bad had happened - firmware cannot function
 *
 * todo: better method name?
 */
void firmwareError(const char *fmt, ...);
bool hasFirmwareError(void);

/**
 * declared as a macro so that this code does not use stack
 * so that it would not crash the error handler in case of stack issues
 */
#if CH_DBG_SYSTEM_STATE_CHECK
#define hasFatalError() (dbg_panic_msg != NULL)
#else
#define hasFatalError() (FALSE)
#endif

void chDbgPanic3(const char *msg, const char * file, int line);

void initErrorHandling(void);

// todo: better place for this shared declaration?
int getRusEfiVersion(void);

/**
 * @deprecated Global panic is inconvenient because it's hard to deliver the error message while whole instance
 * is stopped. Please use firmwareWarning() instead
 */
#define efiAssert(condition, message, result) { if (!(condition)) { firmwareError(message); return result; } }

#define efiAssertVoid(condition, message) { if (!(condition)) { firmwareError(message); return; } }


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* ERROR_HANDLING_H_ */
