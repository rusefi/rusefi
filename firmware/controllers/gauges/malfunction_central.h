/**
 * @file malfunction_central.h
 * @brief This data structure holds current malfunction codes
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "obd_error_codes.h"

// This is moved from engine_parts.h
struct warning_t {
	Timer LastTriggered;
	ObdCode Code = ObdCode::None;

	warning_t() { }

	explicit warning_t(ObdCode code)
		: Code(code)
	{
	}

	// Equality just checks the code, timer doesn't matter
	bool operator ==(const warning_t& other) const {
		return other.Code == Code;
	}

	// Compare against a plain OBD code
	bool operator ==(const ObdCode other) const {
		return other == Code;
	}
};

typedef static_vector<warning_t, 24> warningBuffer_t;

class WarningCodeState {
public:
	WarningCodeState();
	void addWarningCode(ObdCode code, const char *text = nullptr);
	bool isWarningNow() const;
	bool isWarningNow(ObdCode code) const;
	bool hasWarningMessage();
	const char* getWarningMessage();
	void refreshTs();
	void clear();
	int warningCounter;
	ObdCode lastErrorCode = ObdCode::None;
	const char *description;

	Timer timeSinceLastWarning;

	// text that may be assotiated with some warning and should be reported to TS
	critical_msg_t m_msg;
	warning_t *m_msgWarning = nullptr;

	// todo: we need a way to post multiple recent warnings into TS
	warningBuffer_t recentWarnings;
};

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
