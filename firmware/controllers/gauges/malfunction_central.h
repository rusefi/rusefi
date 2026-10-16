/**
 * @file malfunction_central.h
 * @brief This data structure holds current malfunction codes
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "obd_error_codes.h"
#include "past_codes.h"

#define MAX_STORED_ERROR_CODES_COUNT	32
#define MAX_ERROR_CODES_COUNT			10

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

typedef static_vector<warning_t, MAX_STORED_ERROR_CODES_COUNT> warningBuffer_t;

struct error_codes_set_s {
	size_t count = 0;
	ObdCode error_codes[MAX_ERROR_CODES_COUNT]{};
};

// Return true to include a code. Predicates must not modify the shared store.
using ObdCodeFilter = bool (*)(ObdCode);

class WarningCodeState {
public:
	WarningCodeState();
	void addWarningCode(ObdCode code, const char *text = nullptr);
	void removeErrorCode(ObdCode code);
	void getCodes(error_codes_set_s *copy, ObdCodeFilter filter);
	bool hasCodes(ObdCodeFilter filter);

	bool isWarningNow() const;
	bool isWarningNow(ObdCode code) const;
	bool hasWarningMessage();
	const char* getWarningMessage();
	void refreshTs();
	void clear();

	int warningCounter = 0;
	ObdCode lastErrorCode = ObdCode::None;
#if EFI_FILE_LOGGING || EFI_UNIT_TEST
	// Independent of active warnings: clearing/expiry must not erase history.
	PastCodeHistory pastCodes;
#endif
private:
	void removeErrorCode(size_t idx);
	void refresh();
	const char *description;

	Timer timeSinceLastWarning;

	// text that may be assotiated with some warning and should be reported to TS
	critical_msg_t m_msg;
	warning_t *m_msgWarning = nullptr;

#if EFI_UNIT_TEST
public:
#endif
	// todo: we need a way to post multiple recent warnings into TS
	warningBuffer_t recentWarnings;
};

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
 * A null filter includes all codes. Filtering preserves storage order and
 * does not remove codes from the shared diagnostic store.
 */
void getErrorCodes(error_codes_set_s * buffer, ObdCodeFilter filter = nullptr);

bool hasErrorCodes(ObdCodeFilter filter = nullptr);
