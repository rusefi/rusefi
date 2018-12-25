/*
 * @file global.h
 *
 * Global header file for win32 or posix simulator
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <ch.h>
#include <hal.h>

#include <stdlib.h>
#include <time.h>

#include "common_headers.h"
#include "global_shared.h"

#include "boards.h"

#ifdef __cplusplus
#include "chprintf.h"
#include "cli_registry.h"
#include "eficonsole.h"
#endif /* __cplusplus */

#define efiSetPadMode(msg, brainPin, mode) {}

#define EFI_UNIT_TEST FALSE

#define hasFatalError() (FALSE)

#define US_TO_NT_MULTIPLIER 100

#define ALWAYS_INLINE

#define US2NT(x) (US_TO_NT_MULTIPLIER * (x))

#define NT2US(x) ((x) / US_TO_NT_MULTIPLIER)

// need to fight 32bit int overflow

#define MY_US2ST(x) ((x) / 10)

#define EFI_ERROR_CODE 0xffffffff

#define DL_OUTPUT_BUFFER 9000

#define CCM_OPTIONAL

#define EFI_CUSTOM_PANIC_METHOD 1

// project-wide default thread stack size
#define UTILITY_THREAD_STACK_SIZE 1384

/**
 * @brief   @p Win32TestStream virtual methods table.
 */
struct Win32TestStreamVMT {
	_base_channel_methods
};

typedef struct {
  const struct Win32TestStreamVMT *vmt;
} TestStream;

extern TestStream testStream;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void printToWin32Console(char *p);

int getRemainingStack(thread_t *otp);

// todo: move somewhere else?
bool lockAnyContext(void);
void unlockAnyContext(void);
void applyNewConfiguration(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */


/**
 * number of SysClock ticks in one ms
 */
#define TICKS_IN_MS  (CH_FREQUENCY / 1000)

#define hal_lld_get_counter_value() 0

#define EXTERN_ENGINE \
		COMMON_EXTERN_ENGINE
#define ENGINE(x) engine->x

#endif /* GLOBAL_H_ */


