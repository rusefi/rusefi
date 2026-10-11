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
	ObdCode error_codes[MAX_ERROR_CODES_COUNT]{};
};

// Return true to include a code. Predicates must not modify the shared store.
using ObdCodeFilter = bool (*)(ObdCode);

/**
 * @brief Adds an error code into the set of current errors.
 * The error code is placed into the fixed-size data structure if it fits into it.
 * The error code stays in the data structure till it is removed by 'clearError'
 *
 */
void addError(ObdCode errorCode);
/**
 * @brief Removed the error code from the set of current errors.
 *
 */
void removeError(ObdCode errorCode);

void clearWarnings(void);
/**
 * @brief Copies the current set of errors into the specified buffer.
 * A null filter includes all codes. Filtering preserves insertion order and
 * does not remove codes from the shared diagnostic store.
 */
void getErrorCodes(error_codes_set_s * buffer, ObdCodeFilter filter = nullptr);

bool hasErrorCodes(ObdCodeFilter filter = nullptr);
