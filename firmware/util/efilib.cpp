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

int indexOf(const char *string, char ch) {
	// todo: there should be a standard function for this
	int len = strlen(string);
	for (int i = 0; i < len; i++) {
		if (string[i] == ch)
			return i;
	}
	return -1;
}

// string to integer
int atoi(const char *string) {
	// todo: use stdlib '#include <stdlib.h> '
	int len = strlen(string);
	if (len == 0)
		return -ERROR_CODE;
	if (string[0] == '-')
		return -atoi(string + 1);
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

// string to float
float atoff(const char *param) {
	int totallen = strlen(param);
	if (totallen > sizeof(todofixthismesswithcopy) - 1)
		return (float)NAN;
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
	for (int i = 0; i < decimalLen; i++)
		divider = divider * 10.0;
	return integerPart + decimal / divider;
}
