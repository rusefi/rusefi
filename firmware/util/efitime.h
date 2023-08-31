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

#ifdef __cplusplus
/**
 * Provide a 62-bit counter from a 32-bit counter source that wraps around.
 *
 * If you'd like it use it with a 16-bit counter, shift the source by 16 before passing it here.
 * This class is thread/interrupt-safe.
 */
struct WrapAround62 {
	uint64_t update(uint32_t source) {
		// Shift cannot be 31, as we wouldn't be able to tell if time is moving forward or
		// backward relative to m_upper.  We do need to handle both directions as our
		// "thread" can be racing with other "threads" in sampling stamp and updating
		// m_upper.
		constexpr unsigned shift = 30;

		uint32_t upper = m_upper;
		uint32_t relative_unsigned = source - (upper << shift);
		upper += int32_t(relative_unsigned) >> shift;
		m_upper = upper;

		// Yes we could just do upper<<shift, but then the result would span both halves of
		// the 64-bit result.  Doing it this way means we only operate on one half at a
		// time.  Source will supply those bits anyways, so we don't need them from
		// upper...
		return (efitick_t(upper >> (32 - shift)) << 32) | source;
	}

private:
	volatile uint32_t m_upper = 0;
};

/**
 * Get a monotonically increasing (but wrapping) 32-bit timer value
 * Implemented at port level, based on timer or CPU tick counter
 * Main source of EFI clock, SW-extended to 64bits
 */
uint32_t getTimeNowLowerNt();

/**
 * @brief   Returns the number of milliseconds since the board initialization.
 */
efitimems_t getTimeNowMs();

/**
 * @brief   Current system time in seconds.
 */
efitimesec_t getTimeNowS();

#endif /* __cplusplus */
