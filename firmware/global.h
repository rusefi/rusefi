/*
 * @file global.h
 *
 * Global utility header file for firmware
 *
 * Simulator and unit tests have their own version of this header
 *
 * @date May 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

// todo: remove this from here and rely on os_access.h. unfortunately hal.h includes ch.h :(
#include <hal.h>
// *** IMPORTANT *** from painful experience we know that common_headers.h has to be included AFTER hal.h
// *** https://github.com/rusefi/rusefi/issues/1007 ***
#include "common_headers.h"
#include "single_timer_executor.h"

// for US_TO_NT_MULTIPLIER
#include "mpu_util.h"

#include <sys/types.h>

#ifdef __cplusplus
#include "eficonsole.h"
#include <ch.hpp>
#endif /* __cplusplus */


/* definition to expand macro then apply to pragma message */
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

// todo: very odd placement for very weird define, do we even need it? used in SENT
#define CORE_CLOCK STM32_SYSCLK

/**
 * project-wide default thread stack size
 * See also PORT_INT_REQUIRED_STACK
 * See getRemainingStack()
 * See CountFreeStackSpace()
 * See "threadsinfo" command cmd_threads
 */
#ifndef UTILITY_THREAD_STACK_SIZE
#define UTILITY_THREAD_STACK_SIZE 400
#endif /* UTILITY_THREAD_STACK_SIZE */

// CH_DBG_ENABLE_STACK_CHECK
#if CORTEX_MODEL == 7
// F7/H7 use MPU guard pages, so we don't need to check for stack overflow
#define getCurrentRemainingStack() 100000
#else
#define getCurrentRemainingStack() getRemainingStack(chThdGetSelfX())
#endif

#define EFI_ERROR_CODE 0xffffffff

#include "global_port.h"

#define UNIT_TEST_BUSY_WAIT_CALLBACK() {}
