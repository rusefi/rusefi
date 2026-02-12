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

// DBC bit numbers
// byte 0    7  6  5  4  3  2  1  0
// byte 1   15 14 13 12 11 10  9  8
// byte 2   23 22 21 20 19 18 17 16
// byte 3   31 30 29 28 27 26 25 24
// byte 4   39 38 37 36 35 34 33 32
// byte 5   47 46 45 44 43 42 41 40
// byte 6   55 54 53 52 51 50 49 48
// byte 7   63 62 61 60 57 58 57 56

// Intel byte order, bitIndex is the least significant bit of the value
// DBC sample: 0|16@1+
uint32_t getBitRangeLsb(const uint8_t data[], int bitIndex, int bitWidth);
int32_t getBitRangeLsbSigned(const uint8_t data[], int bitIndex, int bitWidth);
void setBitRangeLsb(uint8_t data[], int totalBitIndex, int bitWidth, uint32_t value);

// Motorola byte order, bitIndex is the least significant bit of the value
// not used in DBC
uint32_t getBitRangeMsb(const uint8_t data[], int bitIndex, int bitWidth);
int32_t getBitRangeMsbSigned(const uint8_t data[], int bitIndex, int bitWidth);
void setBitRangeMsb(uint8_t data[], int totalBitIndex, int bitWidth, uint32_t value);

// Motorola byte order, bitIndex is the most significant bit of the value
// DBC sample: 7|16@0+
uint32_t getBitRangeMoto(const uint8_t data[], int bitIndex, int bitWidth);
int32_t getBitRangeMotoSigned(const uint8_t data[], int bitIndex, int bitWidth);
void setBitRangeMoto(uint8_t data[], int totalBitIndex, int bitWidth, uint32_t value);

// convert bitIndex from LSB to MSB style
int motorolaMagicFromDbc(int b, int length);
