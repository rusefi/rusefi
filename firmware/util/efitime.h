/**
 * @file efitime.h
 *
 * By the way, there are 86400000 milliseconds in a day
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "efifeatures.h"
#include "rusefi_types.h"
#include "global.h"
#include <rusefi/rusefi_time_math.h>

#if EFI_PROD_CODE
// for US_TO_NT_MULTIPLIER which is port-specific
#include "port_mpu_util.h"
#endif

// microseconds to ticks
// since only about 20 seconds of ticks fit in 32 bits this macro is casting parameter into 64 bits 'efitick_t' type
// please note that int64 <-> float is a heavy operation thus we have 'USF2NT' below
#define US2NT(us) (((efitick_t)(us)) * US_TO_NT_MULTIPLIER)

// milliseconds to ticks
#define MS2NT(msTime) US2NT(MS2US(msTime))
// See USF2NT above for when to use MSF2NT
#define MSF2NT(msTimeFloat) USF2NT(MS2US(msTimeFloat))

/**
 * Get a monotonically increasing (but wrapping) 32-bit timer value
 * Implemented at port level, based on timer or CPU tick counter
 * Main source of EFI clock, SW-extended to 64bits
 *
 * 2147483648 / ~168MHz = ~12 seconds to overflow
 *
 */
uint32_t getTimeNowLowerNt();

/**
 * @brief   Returns the 32 bit number of milliseconds since the board initialization.
 */
efitimems_t getTimeNowMs();

/**
 * @brief   Current system time in seconds (32 bits)
 */
efitimesec_t getTimeNowS();

#if EFI_UNIT_TEST
void setTimeNowUs(int us);
void advanceTimeUs(int us);
#endif
