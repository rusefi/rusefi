/**
 * @file malfunction_central.c
 * @brief This data structure holds current malfunction codes
 *
 * todo: make this a class
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "malfunction_central.h"

static error_codes_set_s error_codes_set;

void clearWarnings(void) {
	error_codes_set.count = 0;
}

// TODO: wow this is not used by real firmware?!
#if EFI_UNIT_TEST
/**
 * Search if code is present
 * @return -1 if code not found
 */
static int find_position(ObdCode e_code) {
	// cycle for searching element equal seaching code
	for (int t = 0; t < error_codes_set.count; t++)
		if (error_codes_set.error_codes[t] == e_code)
			return t;			// we found position where this code is present
	return -1;														// -1 if code not found
}

void addError(ObdCode errorCode) {
	if (error_codes_set.count < MAX_ERROR_CODES_COUNT && find_position(errorCode) == -1) {
		error_codes_set.error_codes[error_codes_set.count] = errorCode;
		error_codes_set.count++;
	}
}

void removeError(ObdCode errorCode) {
	int pos = find_position(errorCode);
	if (pos >= 0) {
		// shift all right elements to one pos left
		for (int t = pos; t < error_codes_set.count - 1; t++) {
			error_codes_set.error_codes[t] = error_codes_set.error_codes[t + 1];
		}

		error_codes_set.error_codes[--error_codes_set.count] = (ObdCode)0;				// place 0
	}
}
#endif // EFI_UNIT_TEST

void getErrorCodes(error_codes_set_s * copy) {
	copy->count = error_codes_set.count;
	copyArray(copy->error_codes, error_codes_set.error_codes);
}

bool hasErrorCodes(void) {
	return error_codes_set.count > 0;
}
