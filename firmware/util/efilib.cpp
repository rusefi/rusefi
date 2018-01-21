/**
 * @file	efilib.cpp
 *
 * We cannot use stdlib because we do not have malloc - so, we have to implement these functions
 *
 * @date Feb 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include <string.h>
#include <math.h>
#include "efilib.h"
#include "datalogging.h"
#include "histogram.h"
#include "error_handling.h"

const char * boolToString(bool value) {
	return value ? "Yes" : "No";
}

int minI(int i1, int i2) {
	return i1 < i2 ? i1 : i2;
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
	efiAssert(precision != 0, "zero precision", NAN);
	float a = rintf (value / precision);
	return a * precision;
}

float absF(float value) {
	return value > 0 ? value : -value;
}

int absI(int32_t value) {
	return value >= 0 ? value : -value;
}

int maxI(int i1, int i2) {
	return i1 > i2 ? i1 : i2;
}

float maxF(float i1, float i2) {
	return i1 > i2 ? i1 : i2;
}

float minF(float i1, float i2) {
	return i1 < i2 ? i1 : i2;
}

uint32_t efiStrlen(const char *param) {
	register const char *s;
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

int indexOf(const char *string, char ch) {
	// todo: there should be a standard function for this
	// todo: on the other hand MISRA wants us not to use standard headers
	int len = efiStrlen(string);
	for (int i = 0; i < len; i++) {
		if (string[i] == ch) {
			return i;
		}
	}
	return -1;
}

// string to integer
int atoi(const char *string) {
	// todo: use stdlib '#include <stdlib.h> '
	int len = strlen(string);
	if (len == 0) {
		return -ERROR_CODE;
	}
	if (string[0] == '-') {
		return -atoi(string + 1);
	}
	int result = 0;

	for (int i = 0; i < len; i++) {
		char ch = string[i];
		if (ch < '0' || ch > '9') {
			return ERROR_CODE;
		}
		int c = ch - '0';
		result = result * 10 + c;
	}

	return result;
}

static char todofixthismesswithcopy[100];

/**
 * WARNING: due to implementation details specifid buffer should be at least size of '_MAX_FILLER'
 */
static char *ltoa_internal(char *p, long num, unsigned radix) {
	int i;
	char *q;

	q = p + _MAX_FILLER;
	do {
		i = (int) (num % radix);
		i += '0';
		if (i > '9')
			i += 'A' - '0' - 10;
		*--q = i;
	} while ((num /= radix) != 0);

	i = (int) (p + _MAX_FILLER - q);
	do {
		*p++ = *q++;
	} while (--i);

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
// todo: unit test
	return itoa_signed(p, num, 10);
}

#define EPS 0.0001

bool isSameF(float v1, float v2) {
	return absF(v1 - v2) < EPS;
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
 * string to float. NaN input is not supported
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

	// todo: is there a standard function?
	// todo: create a unit test
	int dotIndex = indexOf(string, '.');
	if (dotIndex == -1) {
		// just an integer
		int result = atoi(string);
		if (absI(result) == ERROR_CODE)
			return (float) NAN;
		return (float) result;
	}
	// todo: this needs to be fixed
	string[dotIndex] = 0;
	int integerPart = atoi(string);
	if (absI(integerPart) == ERROR_CODE)
		return (float) NAN;
	string += (dotIndex + 1);
	int decimalLen = strlen(string);
	int decimal = atoi(string);
	if (absI(decimal) == ERROR_CODE)
		return (float) NAN;
	float divider = 1.0;
	// todo: reuse 'pow10' function which we have anyway
	for (int i = 0; i < decimalLen; i++) {
		divider = divider * 10.0;
	}
	return integerPart + decimal / divider;
}

#define TO_LOWER(x) (((x)>='A' && (x)<='Z') ? (x) - 'A' + 'a' : (x))

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

/**
 * @brief This function knows how to print a histogram_s summary
 */
void printHistogram(Logging *logging, histogram_s *histogram) {
#if EFI_HISTOGRAMS && ! EFI_UNIT_TEST
	int report[5];
	int len = hsReport(histogram, report);

	resetLogging(logging);
	appendMsgPrefix(logging);
	appendPrintf(logging, "histogram %s *", histogram->name);
	for (int i = 0; i < len; i++)
	appendPrintf(logging, "%d ", report[i]);
	appendPrintf(logging, "*");
	appendMsgPostfix(logging);
	scheduleLogging(logging);
#else
	UNUSED(logging);
	UNUSED(histogram);
	
#endif /* EFI_HISTOGRAMS */
}

