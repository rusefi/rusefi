/**
 * @file efitime.h
 *
 * By the way, there are 86400000 milliseconds in a day
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef EFITIME_H_
#define EFITIME_H_

#include <stdint.h>
#include "efifeatures.h"

/**
 * integer time in milliseconds
 * 32 bit 4B / 1000 = 4M seconds = 1111.11 hours = 46 days.
 * Please restart your ECU every 46 days? :)
 */
typedef uint32_t efitimems_t;

/**
 * numeric value from 0 to 100
 */
typedef float percent_t;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#define US_PER_SECOND 1000000
#define US_PER_SECOND_LL 1000000LL

#define MS2US(MS_TIME) ((MS_TIME) * 1000)

#define US_TO_TI_TEMP 10

// todo: implement a function to work with times considering counter overflow
#define overflowDiff(now, time) ((now) - (time))

/**
 * 64-bit counter of microseconds (1/1 000 000 of a second) since MCU reset
 *
 * By using 64 bit, we can achieve a very precise timestamp which does not overflow.
 * The primary implementation counts the number of CPU cycles from MCU reset.
 *
 * WARNING: you should use getTimeNowNt where possible for performance reasons.
 * The heaviest part is '__aeabi_ildivmod' - non-native 64 bit division
 */
uint64_t getTimeNowUs(void);

/**
 * 64-bit counter CPU cycles since MCU reset
 */
uint64_t getTimeNowNt(void);

uint64_t getHalTimer(void);

/**
 * @brief   Returns the number of milliseconds since the board initialization.
 */
efitimems_t currentTimeMillis(void);

/**
 * @brief   Current system time in seconds.
 */
int getTimeNowSeconds(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EFITIME_H_ */
