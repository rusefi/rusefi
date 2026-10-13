/**
 * @file malfunction_central.cpp
 * @brief This data structure holds current malfunction codes
 *
 * todo: make this a class
 *
 * @date Dec 20, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "rusefi/efistring.h"
#include "malfunction_central.h"

WarningCodeState::WarningCodeState() {
	clear();
}

void WarningCodeState::clear() {
	warningCounter = 0;
	lastErrorCode = ObdCode::None;
	recentWarnings.clear();
}

void WarningCodeState::addWarningCode(ObdCode code, const char *text) {
	warningCounter++;
	lastErrorCode = code;

	warning_t* existing = recentWarnings.find(code);
	if (!existing) {
		chibios_rt::CriticalSectionLocker csl;

		// Add the code to the list
		existing = recentWarnings.add(warning_t(code));
	}

	if (existing) {
		// Reset the timer on the code to now
		existing->LastTriggered.reset();

		// no pending message? lets try to add this
		if ((m_msgWarning == nullptr) && (text)) {
			strlncpy(m_msg, text, sizeof(m_msg));
			m_msgWarning = existing;
		}
	}

	// Reset the "any warning" timer too
	timeSinceLastWarning.reset();
}

void WarningCodeState::refreshTs() {
	output_channels_s *tsOutputChannels = &engine->outputChannels;
	const int period = maxI(3, engineConfiguration->warningPeriod);

	// TODO: do we need this sticky warning code?
	tsOutputChannels->warningCounter = engine->engineState.warnings.warningCounter;
	tsOutputChannels->lastErrorCode = static_cast<uint16_t>(engine->engineState.warnings.lastErrorCode);

	// TODO: fix OBD codes "jumping" between positions when one of codes disapears

	size_t i = 0;
	for (size_t j = 0; j < recentWarnings.getCount(); j++) {
		warning_t& warn = recentWarnings.get(j);
		if (warn.Code != ObdCode::None) {
			if (!warn.LastTriggered.hasElapsedSec(period)) {
				if (i < efi::size(tsOutputChannels->recentErrorCode)) {
					tsOutputChannels->recentErrorCode[i] = static_cast<uint16_t>(warn.Code);
					i++;
				}
			} else {
				// warning message is outdated, stop showing to TS
				if (m_msgWarning == &warn) {
					m_msg[0] = 0;
					m_msgWarning = nullptr;
				}
				// TODO:
				// reset warning as it is outdated
			}
		}
	}

	// reset rest
	for ( ; i < efi::size(tsOutputChannels->recentErrorCode); i++) {
		tsOutputChannels->recentErrorCode[i] = 0;
	}
}

bool WarningCodeState::hasWarningMessage() {
	// Do we have any error code to show as text?
	return (m_msgWarning != nullptr);
}

const char* WarningCodeState::getWarningMessage() {
	return m_msg;
}

/**
 * @param forIndicator if we want to retrieving value for TS indicator, this case a minimal period is applued
 */
bool WarningCodeState::isWarningNow() const {
	int period = maxI(3, engineConfiguration->warningPeriod);

	return !timeSinceLastWarning.hasElapsedSec(period);
}

// Check whether a particular warning is active
bool WarningCodeState::isWarningNow(ObdCode code) const {
	warning_t* warn = recentWarnings.find(code);

	// No warning found at all
	if (!warn) {
		return false;
	}

	// If the warning is old, it is not active
	return !warn->LastTriggered.hasElapsedSec(maxI(3, engineConfiguration->warningPeriod));
}

static error_codes_set_s error_codes_set;

void clearWarnings(void) {
	error_codes_set.count = 0;
}

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

void getErrorCodes(error_codes_set_s * copy, ObdCodeFilter filter) {
	*copy = {};
	for (int i = 0; i < error_codes_set.count; i++) {
		const auto code = error_codes_set.error_codes[i];
		if (!filter || filter(code)) {
			copy->error_codes[copy->count++] = code;
		}
	}
}

bool hasErrorCodes(ObdCodeFilter filter) {
	for (int i = 0; i < error_codes_set.count; i++) {
		if (!filter || filter(error_codes_set.error_codes[i])) {
			return true;
		}
	}
	return false;
}
