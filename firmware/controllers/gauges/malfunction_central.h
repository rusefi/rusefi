/**
 * @file malfunction_central.h
 * @brief This data structure holds current malfunction codes
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "obd_error_codes.h"

#define MAX_ERROR_CODES_COUNT 10

struct error_codes_set_s {
	int count = 0;
	obd_code_e error_codes[MAX_ERROR_CODES_COUNT];
};

/**
 * @brief Adds an error code into the set of current errors.
 * The error code is placed into the fixed-size data structure if it fits into it.
 * The error code stays in the data structure till it is removed by 'clearError'
 *
 */
void addError(obd_code_e errorCode);
/**
 * @brief Removed the error code from the set of current errors.
 *
 */
void removeError(obd_code_e errorCode);

void setError(bool isError, obd_code_e errorCode);

void clearWarnings(void);
/**
 * @brief Copies the current set of errors into the specified buffer
 */
void getErrorCodes(error_codes_set_s * buffer);

bool hasErrorCodes(void);
