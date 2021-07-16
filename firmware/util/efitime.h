/**
 * @file efitime.h
 *
 * By the way, there are 86400000 milliseconds in a day
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "efifeatures.h"
#include "rusefi_types.h"

#define US_PER_SECOND 1000000
#define US_PER_SECOND_F 1000000.0
#define US_PER_SECOND_LL 1000000LL
#define NT_PER_SECOND (US2NT(US_PER_SECOND_LL))

#define MS2US(MS_TIME) ((MS_TIME) * 1000)

// microseconds to ticks
// since only about 20 seconds of ticks fit in 32 bits this macro is casting parameter into 64 bits 'efitime_t' type
// please note that int64 <-> float is a heavy operation thus we have 'USF2NT' below
#define US2NT(us) (((efitime_t)(us)) * US_TO_NT_MULTIPLIER)

// microseconds to ticks, but floating point
// If converting a floating point time period, use this macro to avoid
// the expensive conversions from int64 <-> float
#define USF2NT(us_float) ((us_float) * US_TO_NT_MULTIPLIER)

// And back
#define NT2US(x) ((x) / US_TO_NT_MULTIPLIER)

// milliseconds to ticks
#define MS2NT(msTime) US2NT(MS2US(msTime))
// See USF2NT above for when to use MSF2NT
#define MSF2NT(msTimeFloat) USF2NT(MS2US(msTimeFloat))

/**
 * We use this 'deep in past, before ECU has ever started' value as a way to unify
 * handling of first ever event and an event which has happened after a large pause in engine activity
 */
#define DEEP_IN_THE_PAST_SECONDS -10

// todo: implement a function to work with times considering counter overflow
#define overflowDiff(now, time) ((now) - (time))

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/**
 * 64-bit counter of microseconds (1/1 000 000 of a second) since MCU reset
 *
 * By using 64 bit, we can achieve a very precise timestamp which does not overflow.
 * The primary implementation counts the number of CPU cycles from MCU reset.
 *
 * WARNING: you should use getTimeNowNt where possible for performance reasons.
 * The heaviest part is '__aeabi_ildivmod' - non-native 64 bit division
 */
efitimeus_t getTimeNowUs(void);

/**
 * 64-bit counter CPU cycles since MCU reset
 *
 * See getTimeNowLowerNt for a quicker version which returns only lower 32 bits
 * Lower 32 bits are enough if all we need is to measure relatively short time durations
 * (BTW 2^32 cpu cycles at 168MHz is 25.59 seconds)
 */
efitick_t getTimeNowNt(void);

efitick_t getHalTimer(void);

/**
 * @brief   Returns the number of milliseconds since the board initialization.
 */
efitimems_t currentTimeMillis(void);

/**
 * @brief   Current system time in seconds.
 */
efitimesec_t getTimeNowSeconds(void);

// Get a monotonically increasing (but wrapping) 32-bit timer value
uint32_t getTimeNowLowerNt(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

