/**
 * @file	efilib.h
 *
 * @date Feb 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include <stdint.h>

#define _MAX_FILLER 11

// http://en.wikipedia.org/wiki/Endianness

static inline uint16_t SWAP_UINT16(uint16_t x)
{
	return ((x << 8) | (x >> 8));
}

static inline uint32_t SWAP_UINT32(uint32_t x)
{
	return (((x >> 24) & 0x000000ff) | ((x <<  8) & 0x00ff0000) |
			((x >>  8) & 0x0000ff00) | ((x << 24) & 0xff000000));
}

#define BIT(n) (UINT32_C(1) << (n))

// we also have efi::size which probably does not work for C code
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define HUMAN_OFFSET 1

// human-readable IDs start from 1 while computer-readable indices start from 0
#define ID2INDEX(id) ((id) - HUMAN_OFFSET)

// number of milliseconds in one period of given frequency (per second)
#define frequency2periodMs(freq) ((1000.0f) / (freq))

// number of microseconds in one period of given frequency (per second)
#define frequency2periodUs(freq) ((1000000.0f) / (freq))

#define ERROR_CODE 311223344

#define Q(x) #x
#define QUOTE(x) Q(x)

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

const char * boolToString(bool value);

char * efiTrim(char *param);
int mytolower(const char c);
uint32_t efiStrlen(const char *param);
int efiPow10(int param);
bool startsWith(const char *line, const char *prefix);
int indexOf(const char *string, char ch);
float atoff(const char *string);
int atoi(const char *string);

#define UNUSED(x) (void)(x)

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

bool strEqualCaseInsensitive(const char *str1, const char *str2);
bool strEqual(const char *str1, const char *str2);

// Currently used by air-interp. tCharge mode (see EngineState::updateTChargeK()).
float limitRateOfChange(float newValue, float oldValue, float incrLimitPerSec, float decrLimitPerSec, float secsPassed);

#ifdef __cplusplus
}

#include <cstddef>
#include <cstring>

#define IS_NEGATIVE_ZERO(value) (__builtin_signbit(value) && value==0)
#define fixNegativeZero(value) (IS_NEGATIVE_ZERO(value) ? 0 : value)

#define assertIsInBounds(length, array, msg) efiAssertVoid(OBD_PCM_Processor_Fault, std::is_unsigned_v<decltype(length)> && (length) < efi::size(array), msg)

#define assertIsInBoundsWithResult(length, array, msg, failedResult) efiAssert(OBD_PCM_Processor_Fault, std::is_unsigned_v<decltype(length)> && (length) < efi::size(array), msg, failedResult)

template <typename T>
bool isInRange(T min, T val, T max) {
	return val >= min && val <= max;
}

static constexpr size_t operator-(Gpio a, Gpio b) {
	return (size_t)a - (size_t)b;
}

static constexpr Gpio operator-(Gpio a, size_t b) {
	return (Gpio)((size_t)a - b);
}

static constexpr Gpio operator+(Gpio a, size_t b) {
	return (Gpio)((size_t)a + b);
}

static constexpr Gpio operator+(size_t a, Gpio b) {
	// addition is commutative, just use the other operator
	return b + a;
}

#endif /* __cplusplus */
