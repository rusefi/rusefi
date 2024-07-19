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
#include "histogram.h"

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
	efiAssert(ObdCode::CUSTOM_ERR_ASSERT, precision != 0, "Zero precision is not valid for efiRound maybe you mean '1'?", NAN);
	float a = round(value / precision);
	return fixNegativeZero(a * precision);
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

/**
 * @brief This function knows how to print a histogram_s summary
 */
void printHistogram(Logging *logging, histogram_s *histogram) {
#if EFI_HISTOGRAMS && ! EFI_UNIT_TEST
	int report[5];
	int len = hsReport(histogram, report);

	logging->reset();
	logging.append(PROTOCOL_MSG LOG_DELIMITER);
	logging.appendPrintf("histogram %s *", histogram->name);
	for (int i = 0; i < len; i++)
	logging.appendPrintf("%d ", report[i]);
	logging.appendPrintf("*");
	logging.append(LOG_DELIMITER);
	scheduleLogging(logging);
#else
	UNUSED(logging);
	UNUSED(histogram);

#endif /* EFI_HISTOGRAMS */
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

static int getBitRangeCommon(const uint8_t data[], int bitIndex, int bitWidth, int secondByteOffset) {
	int byteIndex = bitIndex >> 3;
	int shift = bitIndex - byteIndex * 8;
	int value = data[byteIndex];
	if (shift + bitWidth > 8) {
		value = value + data[secondByteOffset + byteIndex] * 256;
	}
	int mask = (1 << bitWidth) - 1;
	return (value >> shift) & mask;
}

// see also getBitRange in lua_lib.h
int getBitRangeLsb(const uint8_t data[], int bitIndex, int bitWidth) {
  return getBitRangeCommon(data, bitIndex, bitWidth, 1);
}

// see also getBitRangeMsb in lua_lib.h
int getBitRangeMsb(const uint8_t data[], int bitIndex, int bitWidth) {
  return getBitRangeCommon(data, bitIndex, bitWidth, -1);
}
