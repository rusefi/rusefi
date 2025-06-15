/**
 * @file efitime.h
 *
 * By the way, there are 86400000 milliseconds in a day
 *
 * @date Apr 14, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "rusefi_types.h"
#include "error_handling.h"
#include <rusefi/rusefi_time_math.h>


#if EFI_PROD_CODE
// for US_TO_NT_MULTIPLIER which is port-specific
#include "port_mpu_util.h"
#endif

inline int time2print(int64_t time) {
  return static_cast<int>(time);
}

// For the sole purpose of satisfying weird Mac OS + some Win compilers' stdlibs impl...
constexpr bool constexpr_isfinite(float f) {
#if __cplusplus >= 202302L
  // In C++23, std::isfinite is officially constexpr
  return std::isfinite(f);
#elif defined(_WIN32) || defined(__APPLE__) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
  // On Windows and macOS, std::isfinite might not be constexpr pre-C++23
  return true;
#else
  // This is meant to correspond to normal/target platforms
  return std::isfinite(f);
#endif
}

constexpr bool _assertFloatFitsInto32BitsAndCast(float value) {
  constexpr auto FirstUnrepresentableBigFloat = static_cast<float>(INT32_MAX);
  constexpr auto kInt32MinF = static_cast<float>(INT32_MIN);

  // 32bit float has 24bit precision that lead to fiasco like:
  // 2147483648.0 > 2147483647 == false
  // Also at 2147483648.0 we need 2^7=128 step to go to next value of 2147483904.0f
  // And 2147483648.0f is already non-representable in 32 bit int
  // So if someone using corner value of max int for smth like invalid float
  // we might accidentally give green light if we are not strictly under 2147483648.0
  // i.e. do not change this check to implicit convertion int->float with non strict condition!
  return constexpr_isfinite(value) && value >= kInt32MinF && value < FirstUnrepresentableBigFloat;
}

static_assert(_assertFloatFitsInto32BitsAndCast(INT32_MAX) == false);
static_assert(_assertFloatFitsInto32BitsAndCast(static_cast<float>(INT32_MAX)) == false);

// 64 for int because during conversion it is rounding to nearest
// i.e. -64 to +63 = 128 or 2^7 because 2^31 (int) / 2^24 (float)
static_assert(_assertFloatFitsInto32BitsAndCast(INT32_MAX - 1) == false);
static_assert(_assertFloatFitsInto32BitsAndCast(INT32_MAX - 63) == false);
static_assert(_assertFloatFitsInto32BitsAndCast(INT32_MAX - 64) == true);
static_assert(_assertFloatFitsInto32BitsAndCast(static_cast<float>(INT32_MAX - 1)) == false);
static_assert(_assertFloatFitsInto32BitsAndCast(static_cast<float>(INT32_MAX - 63)) == false);
static_assert(_assertFloatFitsInto32BitsAndCast(static_cast<float>(INT32_MAX - 64)) == true);

// For INT32_MIN cases
static_assert(_assertFloatFitsInto32BitsAndCast(INT32_MIN) == true);
static_assert(_assertFloatFitsInto32BitsAndCast(static_cast<float>(INT32_MIN)) == true);
static_assert(_assertFloatFitsInto32BitsAndCast(static_cast<float>(INT32_MIN) - 0.1f) == true);
static_assert(_assertFloatFitsInto32BitsAndCast(static_cast<float>(static_cast<int64_t>(INT32_MIN))) == true);
static_assert(_assertFloatFitsInto32BitsAndCast(static_cast<float>(static_cast<int64_t>(INT32_MIN) - 1)) == true);
static_assert(_assertFloatFitsInto32BitsAndCast(static_cast<float>(static_cast<int64_t>(INT32_MIN) - 128)) == true);
static_assert(_assertFloatFitsInto32BitsAndCast(static_cast<float>(static_cast<int64_t>(INT32_MIN) - 129)) == false);

// Extreme negative float (definitely invalid)
static_assert(_assertFloatFitsInto32BitsAndCast(-1e38f) == false);
// NaN
static_assert(!_assertFloatFitsInto32BitsAndCast(NAN));
// -Infinity
static_assert(!_assertFloatFitsInto32BitsAndCast(-INFINITY));

constexpr int32_t assertFloatFitsInto32BitsAndCast(const char *msg, float value) {
  if (!_assertFloatFitsInto32BitsAndCast(value)) {
    criticalError("%s value is not representable in 32bit int: %f", msg, value);
  }

  return static_cast<int32_t>(value);
}

inline /*64bit*/ efitick_t sumTickAndFloat(/*64bit*/efitick_t ticks, /*32bit*/float extra) {
  // we have a peculiar case of type compiler uses to evaluate expression vs precision here
  // we need 64 bit precision not (lower) float precision
  // 32 bits is 11 or 23 seconds if US_TO_NT_MULTIPLIER = 168 like on kinetis/cypress
  // 32 bits is 500 or 1000 seconds if US_TO_NT_MULTIPLIER = 4 like on stm32
  // 'extra' is below 10 seconds here so we use 32 bit type for performance reasons
  return ticks + (int32_t) extra;
}

// microseconds to ticks
// since only about 20 seconds of ticks fit in 32 bits this macro is casting parameter into 64 bits 'efitick_t' type
// please note that int64 <-> float is a heavy operation thus we have 'USF2NT' below
#define US2NT(us) (((efitick_t)(us)) * US_TO_NT_MULTIPLIER)

// milliseconds to ticks
#define MS2NT(msTime) US2NT(MS2US(msTime))
// See USF2NT above for when to use MSF2NT. ***WARNING*** please be aware of sumTickAndFloat
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
