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
  
int absI(int32_t value);
float absF(float value);
/**
 * Rounds value to specified precision.
 * @param precision some pow of 10 value - for example, 100 for two digit precision
 */
float efiRound(float value, float precision);
int maxI(int i1, int i2);
int minI(int i1, int i2);
float maxF(float i1, float i2);
float minF(float i1, float i2);
char* itoa10(char *p, int num);
bool isSameF(float v1, float v2);

int clampI(int min, int clamp, int max);
float clampF(float min, float clamp, float max);

/**
 * clamps value into the [0, 100] range
 */
#define clampPercentValue(x) (clampF(0, x, 100))

bool strEqualCaseInsensitive(const char *str1, const char *str2);
bool strEqual(const char *str1, const char *str2);

// Currently used by air-interp. tCharge mode (see EngineState::updateTChargeK()).
float limitRateOfChange(float newValue, float oldValue, float incrLimitPerSec, float decrLimitPerSec, float secsPassed);

// @brief Compute e^x using a 4th order taylor expansion centered at x=-1.  Provides
// bogus results outside the range -2 < x < 0.
float expf_taylor(float x);

// @brief Compute tan(theta) using a ratio of the Taylor series for sin and cos
// Valid for the range [0, pi/2 - 0.01]
float tanf_taylor(float theta);

#ifdef __cplusplus
}

#include <cstddef>
#include <cstring>

#define IS_NEGATIVE_ZERO(value) (__builtin_signbit(value) && value==0)
#define fixNegativeZero(value) (IS_NEGATIVE_ZERO(value) ? 0 : value)

// C++ helpers go here
namespace efi
{
template <typename T, size_t N>
constexpr size_t size(const T(&)[N]) {
    return N;
}

// Zero the passed object
template <typename T>
constexpr void clear(T* obj) {
	// The cast to void* is to prevent errors like:
	//    clearing an object of non-trivial type 'struct persistent_config_s'; use assignment or value-initialization instead
	// This is technically wrong, but we know config objects only ever actually
	// contain integral types, though they may be wrapped in a scaled_channel
	memset(reinterpret_cast<void*>(obj), 0, sizeof(T));
}

template <typename T>
constexpr void clear(T& obj) {
	clear(&obj);
}
} // namespace efi

#define assertIsInBounds(length, array, msg) efiAssertVoid(OBD_PCM_Processor_Fault, (length) >= 0 && (length) < efi::size(array), msg)

#define assertIsInBoundsWithResult(length, array, msg, failedResult) efiAssert(OBD_PCM_Processor_Fault, (length) >= 0 && (length) < efi::size(array), msg, failedResult)

/**
 * Copies an array from src to dest.  The lengths of the arrays must match.
 */
template <typename TElement, size_t N>
constexpr void copyArray(TElement (&dest)[N], const TElement (&src)[N]) {
	for (size_t i = 0; i < N; i++) {
		dest[i] = src[i];
	}
}

/**
 * Copies an array from src to the beginning of dst.  If dst is larger
 * than src, then only the elements copied from src will be touched.
 * Any remaining elements at the end will be untouched.
 */
template <typename TElement, size_t NSrc, size_t NDest>
constexpr void copyArrayPartial(TElement (&dest)[NDest], const TElement (&src)[NSrc]) {
	static_assert(NDest >= NSrc, "Source array must be larger than destination.");

	for (size_t i = 0; i < NSrc; i++) {
		dest[i] = src[i];
	}
}

#endif /* __cplusplus */

#if defined(__cplusplus) && defined(__OPTIMIZE__)
#include <type_traits>
// "g++ -O2" version, adds more strict type check and yet no "strict-aliasing" warnings!
#define cisnan(f) ({ \
	static_assert(sizeof(f) == sizeof(int32_t)); \
	union cisnanu_t { std::remove_reference_t<decltype(f)> __f; int32_t __i; } __cisnan_u = { f }; \
	__cisnan_u.__i == 0x7FC00000; \
})
#else
// "g++ -O0" or other C++/C compilers
#define cisnan(f) (*(((int*) (&f))) == 0x7FC00000)
#endif /* __cplusplus && __OPTIMIZE__ */
