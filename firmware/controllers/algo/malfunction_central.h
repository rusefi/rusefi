/**
 * @file malfunction_central.h
 * @brief This data structure holds current malfunction codes
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef MALFUNCTION_CENTRAL_H_
#define MALFUNCTION_CENTRAL_H_

#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "obd_error_codes.h"

#define MAX_ERROR_CODES_COUNT 10

typedef struct {
	int count;
	obd_code_e error_codes[MAX_ERROR_CODES_COUNT];
} error_codes_set_s;

void initMalfunctionCentral(void);

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

/**
 * @brief Copies the current set of errors into the specified buffer
 */
void getErrorCodes(error_codes_set_s * buffer);

bool hasErrorCodes(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MALFUNCTION_CENTRAL_H_ */
