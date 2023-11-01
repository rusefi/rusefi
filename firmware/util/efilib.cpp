/**
 * @file	efilib.cpp
 *
 * We cannot use stdlib because we do not have malloc - so, we have to implement these functions
 *
 * @date Feb 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include <string.h>
#include <math.h>
#include "datalogging.h"

// also known as bool2string and boolean2string
const char * boolToString(bool value) {
	return value ? "Yes" : "No";
}

/*
float efiFloor(float value, float precision) {
	int a = (int) (value / precision);
	return a * precision;
}
*/

/**
 *
 * @param precision for example '0.1' for one digit fractional part
 */
float efiRound(float value, float precision) {
	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, precision != 0, "zero precision", NAN);
	float a = round(value / precision);
	return fixNegativeZero(a * precision);
}

uint32_t efiStrlen(const char *param) {
	const char *s;
	for (s = param; *s; ++s)
		;
	return (s - param);
}

char * efiTrim(char *param) {
	while (param[0] == ' ') {
		param++; // that would skip leading spaces
	}
	int len = efiStrlen(param);
	while (len > 0 && param[len - 1] == ' ') {
		param[len - 1] = 0;
		len--;
	}
	return param;
}

bool startsWith(const char *line, const char *prefix) {
	uint32_t len = efiStrlen(prefix);
	if (efiStrlen(line) < len) {
		return false;
	}
	for (uint32_t i = 0; i < len; i++) {
		if (line[i] != prefix[i]) {
			return false;
		}
	}
	return true;
}

int indexOf(const char *string, char c) {
	// a standard function for this is strnchr?
	// todo: on the other hand MISRA wants us not to use standard headers
	int len = efiStrlen(string);
	for (int i = 0; i < len; i++) {
		if (string[i] == c) {
			return i;
		}
	}
	return -1;
}

// string to integer
int atoi(const char *string) {
	int len = strlen(string);
	if (len == 0) {
		return -ATOI_ERROR_CODE;
	}
	if (string[0] == '-') {
		return -atoi(string + 1);
	}
	int result = 0;

	for (int i = 0; i < len; i++) {
		char c = string[i];
		if (c < '0' || c > '9') {
			if (i > 0) {
				break;
			} else {
				return ATOI_ERROR_CODE;
			}
		}
		int num = c - '0';
		result = result * 10 + num;
	}

	return result;
}

static char todofixthismesswithcopy[100];

static char *ltoa_internal(char *p, uint32_t num, unsigned radix) {
	constexpr int bufferLength = 10;
	
	char buffer[bufferLength];

	size_t idx = bufferLength - 1;

	// First, we write from right-to-left so that we don't have to compute
	// log(num)/log(radix)
	do
	{
		auto digit = num % radix;

		// Digits 0-9 -> '0'-'9'
		// Digits 10-15 -> 'a'-'f'
		char c = digit < 10
			? digit + '0'
			: digit + 'a' - 10;

		// Write this digit in to the buffer
		buffer[idx] = c;
		idx--;
	} while ((num /= radix) != 0);
	
	idx++;

	// Now, we copy characters in to place in the final buffer
	while (idx < bufferLength)
	{
		*p++ = buffer[idx++];
	}

	return p;
}

/**
 * @return pointer at the end zero symbol after the digits
 */
static char* itoa_signed(char *p, int num, unsigned radix) {
	if (num < 0) {
		*p++ = '-';
		char *end = ltoa_internal(p, -num, radix);
		*end = 0;
		return end;
	}
	char *end = ltoa_internal(p, num, radix);
	*end = 0;
	return end;
}

/**
 * Integer to string
 *
 * @return pointer at the end zero symbol after the digits
 */
char* itoa10(char *p, int num) {
	return itoa_signed(p, num, 10);
}

int efiPow10(int param) {
	switch (param) {
	case 0:
		return 1;
	case 1:
		return 10;
	case 2:
		return 100;
	case 3:
		return 1000;
	case 4:
		return 10000;
	case 5:
		return 100000;
	case 6:
		return 1000000;
	case 7:
		return 10000000;
	case 8:
		return 100000000;
	}
	return 10 * efiPow10(10 - 1);
}

/**
 * string to float. NaN input is supported
 *
 * @return NAN in case of invalid string
 * todo: explicit value for error code? probably not, NaN is only returned in case of an error
 */
float atoff(const char *param) {
	uint32_t totallen = strlen(param);
	if (totallen > sizeof(todofixthismesswithcopy) - 1)
		return (float) NAN;
	strcpy(todofixthismesswithcopy, param);
	char *string = todofixthismesswithcopy;
	if (indexOf(string, 'n') != -1 || indexOf(string, 'N') != -1) {
#if ! EFI_SIMULATOR
		efiPrintf("NAN from [%s]", string);
#endif
		return (float) NAN;
	}

	// todo: is there a standard function?
	// unit-tested by 'testMisc()'
	int dotIndex = indexOf(string, '.');
	if (dotIndex == -1) {
		// just an integer
		int result = atoi(string);
		if (absI(result) == ATOI_ERROR_CODE)
			return (float) NAN;
		return (float) result;
	}
	// todo: this needs to be fixed
	string[dotIndex] = 0;
	int integerPart = atoi(string);
	if (absI(integerPart) == ATOI_ERROR_CODE)
		return (float) NAN;
	string += (dotIndex + 1);
	int decimalLen = strlen(string);
	int decimal = atoi(string);
	if (absI(decimal) == ATOI_ERROR_CODE)
		return (float) NAN;
	float divider = 1.0;
	// todo: reuse 'pow10' function which we have anyway
	for (int i = 0; i < decimalLen; i++) {
		divider = divider * 10.0;
	}
	return integerPart + decimal / divider;
}

bool strEqualCaseInsensitive(const char *str1, const char *str2) {
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	if (len1 != len2) {
		return false;
	}
	for (int i = 0; i < len1; i++)
		if (TO_LOWER(str1[i]) != TO_LOWER(str2[i]))
			return false;
	return true;
}

/*
** return lower-case of c if upper-case, else c
*/
int mytolower(const char c) {
  return TO_LOWER(c);
}


int djb2lowerCase(const char *str) {
	unsigned long hash = 5381;
	int c;

	while ( (c = *str++) ) {
		c = TO_LOWER(c);
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}

	return hash;
}

bool strEqual(const char *str1, const char *str2) {
	// todo: there must be a standard function?!
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	if (len1 != len2) {
		return false;
	}
	for (int i = 0; i < len1; i++)
		if (str1[i] != str2[i])
			return false;
	return true;
}

float limitRateOfChange(float newValue, float oldValue, float incrLimitPerSec, float decrLimitPerSec, float secsPassed) {
	if (newValue >= oldValue)
		return (incrLimitPerSec <= 0.0f) ? newValue : oldValue + minF(newValue - oldValue, incrLimitPerSec * secsPassed);
	return (decrLimitPerSec <= 0.0f) ? newValue : oldValue - minF(oldValue - newValue, decrLimitPerSec * secsPassed);
}

bool isPhaseInRange(float test, float current, float next) {
	bool afterCurrent = test >= current;
	bool beforeNext = test < next;

	if (next > current) {
		// we're not near the end of the cycle, comparison is simple
		// 0            |------------------------|       720
		//            next                    current
		return afterCurrent && beforeNext;
	} else {
		// we're near the end of the cycle so we have to check the wraparound
		// 0 -----------|                        |------ 720
		//            next                    current
		// Check whether test is after current (ie, between current tooth and end of cycle)
		// or if test if before next (ie, between start of cycle and next tooth)
		return afterCurrent || beforeNext;
	}
}
