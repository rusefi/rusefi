/*
 * @file global.h
 *
 * Global header file for win32 or posix simulator
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <hal.h>

#include <stdlib.h>
#include <time.h>

#include "common_headers.h"

#include "boards.h"

#ifdef __cplusplus
#include "chprintf.h"
#include "cli_registry.h"
#include "eficonsole.h"
#endif /* __cplusplus */

#define hasOsPanicError() (FALSE)

#define US_TO_NT_MULTIPLIER 100

// need to fight 32bit int overflow

#define MY_US2ST(x) ((x) / 10)

#define EFI_ERROR_CODE 0xffffffff

#define DL_OUTPUT_BUFFER 9000

#define CCM_OPTIONAL
#define NO_CACHE

#define EFI_CUSTOM_PANIC_METHOD 1

// project-wide default thread stack size
#define UTILITY_THREAD_STACK_SIZE 1384

#define UNIT_TEST_BUSY_WAIT_CALLBACK() {}

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void printToConsole(char *p);

#define getCurrentRemainingStack() getRemainingStack(chThdGetSelfX())

int getRemainingStack(thread_t *otp);

void applyNewConfiguration(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#define hal_lld_get_counter_value() 0

