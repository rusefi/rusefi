/**
 * @file	efilib.c
 *
 * We cannot use stdlib because we do not have malloc - so, we have to implement these functions
 *
 * @date Feb 21, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include <string.h>
#include <math.h>
#include "efilib.h"

#define _MAX_FILLER 11

/**
 * there is some BS related to isnan in MinGW, so let's have all the issues in one place
 */
bool cisnan(float f) {
	return *(((int*) (&f))) == 0x7FC00000;
}

const char * boolToString(bool value) {
	return value ? "Yes" : "No";
}

int minI(int i1, int i2) {
	return i1 < i2 ? i1 : i2;
}

float efiRound(float value, float precision) {
	int a = (int)(value / precision);
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

int efiStrlen(const char *param) {
	return strlen(param);
}

int indexOf(const char *string, char ch) {
	// todo: there should be a standard function for this
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
 */
char* itoa10(char *p, int num) {
// todo: unit test
	return itoa_signed(p, num, 10);
}

#define EPS 0.0001

bool isSameF(float v1, float v2) {
	return absF(v1 - v2) < EPS;
}

// string to float
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
		return (float) result;
	}
	// todo: this needs to be fixed
	string[dotIndex] = 0;
	int integerPart = atoi(string);
	string += (dotIndex + 1);
	int decimalLen = strlen(string);
	int decimal = atoi(string);
	float divider = 1.0;
	// todo: reuse 'pow10' function which we have anyway
	for (int i = 0; i < decimalLen; i++) {
		divider = divider * 10.0;
	}
	return integerPart + decimal / divider;
}
