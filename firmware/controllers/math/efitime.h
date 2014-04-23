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


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

// todo: implement a function to work with times considering counter overflow
#define overflowDiff(now, time) ((now) - (time))

/**
 * 64-bit counter of microseconds (1/1 000 000 of a second) since MCU reset
 *
 * By using 64 bit, we can achive a very precise timestamp which does not overflow.
 * The primary implementation counts the number of CPU cycles from MCU reset.
 */
uint64_t getTimeNowUs(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EFITIME_H_ */
