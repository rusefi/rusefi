/**
 * @file	efilib.h
 *
 * @date Feb 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "unused.h"
#include "efi_quote.h"
#include <stdint.h>

#include <rusefi/arrays.h>

int djb2lowerCase(const char *str);

#define _MAX_FILLER 11

// http://en.wikipedia.org/wiki/Endianness

inline uint16_t SWAP_UINT16(uint16_t x)
{
	return ((x << 8) | (x >> 8));
}

inline uint32_t SWAP_UINT32(uint32_t x)
{
	return (((x >> 24) & 0x000000ff) | ((x <<  8) & 0x00ff0000) |
			((x >>  8) & 0x0000ff00) | ((x << 24) & 0xff000000));
}

#define BIT(n) (UINT32_C(1) << (n))

// also known as 'HUMAN_INDEX'
#define HUMAN_OFFSET 1

// human-readable IDs start from 1 while computer-readable indices start from 0
#define ID2INDEX(id) ((id) - HUMAN_OFFSET)

// number of milliseconds in one period of given frequency (per second)
#define frequency2periodMs(freq) ((1000.0f) / (freq))

// number of microseconds in one period of given frequency (per second)
#define frequency2periodUs(freq) ((1000000.0f) / (freq))

const char * boolToString(bool value);

char * efiTrim(char *param);
int efiPow10(int param);

/**
 * Rounds value to specified precision.
 * @param precision some pow of 10 value - for example, 100 for two digit precision
 */
float efiRound(float value, float precision);

// sometimes known as 'itoa'
char* itoa10(char *p, int num);

/**
 * clamps value into the [0, 100] range
 */
#define clampPercentValue(x) (clampF(0, x, 100))

// Currently used by air-interp. tCharge mode (see EngineState::updateTChargeK()).
float limitRateOfChange(float newValue, float oldValue, float incrLimitPerSec, float decrLimitPerSec, float secsPassed);

bool isPhaseInRange(float test, float current, float next);

#include <cstddef>
#include <cstring>

#define IS_NEGATIVE_ZERO(value) (__builtin_signbit(value) && value==0)
#define fixNegativeZero(value) (IS_NEGATIVE_ZERO(value) ? 0 : value)

#define assertIsInBounds(length, array, msg) criticalAssertVoid(std::is_unsigned_v<decltype(length)> && (length) < efi::size(array), msg)

#define assertIsInBoundsWithResult(length, array, msg, failedResult) efiAssert(ObdCode::OBD_PCM_Processor_Fault, std::is_unsigned_v<decltype(length)> && (length) < efi::size(array), msg, failedResult)

template <typename T>
bool isInRange(T min, T val, T max) {
	return val >= min && val <= max;
}

inline constexpr size_t operator-(Gpio a, Gpio b) {
	return (size_t)a - (size_t)b;
}

inline constexpr Gpio operator-(Gpio a, size_t b) {
	return (Gpio)((size_t)a - b);
}

inline constexpr Gpio operator+(Gpio a, size_t b) {
	return (Gpio)((size_t)a + b);
}

inline constexpr Gpio operator+(size_t a, Gpio b) {
	// addition is commutative, just use the other operator
	return b + a;
}

namespace efi
{
template <class _Ty>
struct remove_reference {
    using type = _Ty;
};

template <class _Ty>
struct remove_reference<_Ty&> {
    using type = _Ty;
};

template <class _Ty>
struct remove_reference<_Ty&&> {
    using type = _Ty;
};

template <class _Ty>
using remove_reference_t = typename remove_reference<_Ty>::type;

// FUNCTION TEMPLATE move
template <class _Ty>
constexpr remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept {
    return static_cast<remove_reference_t<_Ty>&&>(_Arg);
}
}

int getBitRangeLsb(const uint8_t data[], int bitIndex, int bitWidth);
/**
 for instance DBC 8|16@0
 */
int getBitRangeMsb(const uint8_t data[], int bitIndex, int bitWidth);
void setBitRangeMsb(uint8_t data[], int totalBitIndex, int bitWidth, int value);

int motorolaMagicFromDbc(int b, int length);
int getBitRangeMoto(const uint8_t data[], int bitIndex, int bitWidth);
void setBitRangeMoto(uint8_t data[], int totalBitIndex, int bitWidth, int value);
