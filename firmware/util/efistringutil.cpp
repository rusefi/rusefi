#include <cstring>
#include <cstdint>
#include <math.h>
#include "efiprintf.h"
#include "efistringutil.h"
#include "efiprintf.h"
#include <rusefi/math.h>

uint32_t efiStrlen(const char *param) {
	const char *s;
	for (s = param; *s; ++s)
		;
	return (s - param);
}

int indexOf(const char *string, char ch) {
	// a standard function for this is strnchr?
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
	int len = strlen(string);
	if (len == 0) {
		return -ATOI_ERROR_CODE;
	}
	if (string[0] == '-') {
		return -atoi(string + 1);
	}
	int result = 0;

	for (int i = 0; i < len; i++) {
		char ch = string[i];
		if (ch < '0' || ch > '9') {
			if (i > 0) {
				break;
			} else {
				return ATOI_ERROR_CODE;
			}
		}
		int c = ch - '0';
		result = result * 10 + c;
	}

	return result;
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
 * string to float. NaN input is supported
 *
 * @return NAN in case of invalid string
 * todo: explicit value for error code? probably not, NaN is only returned in case of an error
 */
float atoff(const char *param) {
static char todofixthismesswithcopy[100];

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
