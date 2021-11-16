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

// for US_TO_NT_MULTIPLIER
#include "mpu_util.h"

// this is about MISRA not liking 'time.h'. todo: figure out something
#if defined __GNUC__
// GCC
#include <sys/types.h>
#else
// IAR
typedef unsigned int time_t;
#endif

#ifdef __cplusplus
#include "eficonsole.h"
#include <ch.hpp>
#endif /* __cplusplus */


/* definition to expand macro then apply to pragma message */
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

#define CORE_CLOCK STM32_SYSCLK
//#pragma message(VAR_NAME_VALUE(CORE_CLOCK))


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

#define getCurrentRemainingStack() getRemainingStack(chThdGetSelfX())

#define EFI_ERROR_CODE 0xffffffff

/**
 * rusEfi is placing some of data structures into CCM memory simply
 * in order to use that memory - no magic about which RAM is faster etc.
 * That said, CCM/TCM could be faster as there will be less bus contention
 * with DMA.
 *
 * Please note that DMA does not work with CCM memory
 */
#if defined(STM32F4XX)
// CCM memory is 64k
#define CCM_OPTIONAL __attribute__((section(".ram4")))
#define NO_CACHE	// F4 has no cache, do nothing
#elif defined(STM32F7XX)
// DTCM memory is 128k
#define CCM_OPTIONAL __attribute__((section(".ram3")))
// SRAM2 is 16k and set to disable dcache
#define NO_CACHE __attribute__((section(".ram2")))
#elif defined(STM32H7XX)
// DTCM memory is 128k
#define CCM_OPTIONAL __attribute__((section(".ram5")))
// SRAM3 is 32k and set to disable dcache
#define NO_CACHE __attribute__((section(".ram3")))
#else /* this MCU doesn't need these */
#define CCM_OPTIONAL
#define NO_CACHE
#endif

#define UNIT_TEST_BUSY_WAIT_CALLBACK() {}
