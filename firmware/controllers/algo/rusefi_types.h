/**
 * @file	rusefi_types.h
 *
 * @date Jan 12, 2015
 * @author Andrey Belomutskiy, (c) 2012-2014
 */
#ifndef CONTROLLERS_ALGO_RUSEFI_TYPES_H_
#define CONTROLLERS_ALGO_RUSEFI_TYPES_H_

#include <stdint.h>

/**
 * integer time in milliseconds
 * 32 bit 4B / 1000 = 4M seconds = 1111.11 hours = 46 days.
 * Please restart your ECU every 46 days? :)
 */
typedef uint32_t efitimems_t;

/**
 * 64 bit time in microseconds, since boot
 */
typedef uint64_t efitimeus_t;

/**
 * platform-dependent tick since boot
 * in case of stm32f4 that's a CPU tick
 */
typedef uint64_t efitick_t;

typedef float angle_t;

/**
 * numeric value from 0 to 100
 */
typedef float percent_t;

#endif /* CONTROLLERS_ALGO_RUSEFI_TYPES_H_ */
