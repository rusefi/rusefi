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
	if (code == ObdCode::None) {
		return;
	}

	// Set last code
	lastErrorCode = code;
	// Reset the "any warning" timer too
	timeSinceLastWarning.reset();

	warning_t* existing = recentWarnings.find(code);
	if (!existing) {
		chibios_rt::CriticalSectionLocker csl;

		// Add the code to the list
		existing = recentWarnings.add(warning_t(code));
		if (existing) {
			warningCounter++;
		}
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
}

void WarningCodeState::removeErrorCode(size_t idx) {
	warning_t& warn = recentWarnings.get(idx);
	if (m_msgWarning == &warn) {
		m_msg[0] = 0;
		m_msgWarning = nullptr;
	}
	warn.Code = ObdCode::None;
	warningCounter--;
}

void WarningCodeState::removeErrorCode(ObdCode code) {
	warning_t* warn = recentWarnings.find(code);
	if (warn) {
		if (m_msgWarning == warn) {
			m_msg[0] = 0;
			m_msgWarning = nullptr;
		}
		warn->Code = ObdCode::None;
		warningCounter--;
	}
}

void WarningCodeState::getCodes(error_codes_set_s *copy, ObdCodeFilter filter) {
	*copy = {};
	for (size_t i = 0; (i < recentWarnings.getCount()) && (copy->count < efi::size(copy->error_codes)); i++) {
		const auto warn = recentWarnings.get(i);
		if (warn.Code == ObdCode::None) {
			continue;
		}
		if (!filter || filter(warn.Code)) {
			copy->error_codes[copy->count++] = warn.Code;
		}
	}
}

bool WarningCodeState::hasCodes(ObdCodeFilter filter) {
	for (size_t i = 0; i < recentWarnings.getCount(); i++) {
		const auto warn = recentWarnings.get(i);
		if (warn.Code == ObdCode::None) {
			continue;
		}
		if (!filter || filter(warn.Code)) {
			return true;
		}
	}
	return false;
}

void WarningCodeState::refresh() {
	// remove all outdated codes
	const int period = maxI(3, engineConfiguration->warningPeriod);
	for (size_t j = 0; j < recentWarnings.getCount(); j++) {
		warning_t& warn = recentWarnings.get(j);
		if (warn.Code != ObdCode::None) {
			if (warn.LastTriggered.hasElapsedSec(period)) {
				// warning message is outdated, stop showing to TS
				removeErrorCode(j);
			}
		}
	}
}

void WarningCodeState::refreshTs() {
	output_channels_s *tsOutputChannels = &engine->outputChannels;

	// This should be periodically called, not from TS only
	refresh();

	// TODO: do we need this sticky warning code?
	tsOutputChannels->lastErrorCode = static_cast<uint16_t>(lastErrorCode);

	// TODO: fix OBD codes "jumping" between positions when one of codes disapears
	size_t i = 0;
	for (size_t j = 0; j < recentWarnings.getCount(); j++) {
		warning_t& warn = recentWarnings.get(j);
		if (warn.Code == ObdCode::None) {
			continue;
		}
		if (i < efi::size(tsOutputChannels->recentErrorCode)) {
			tsOutputChannels->recentErrorCode[i] = static_cast<uint16_t>(warn.Code);
		}
		i++;
	}

	tsOutputChannels->warningCounter = i;

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

void clearWarnings(void) {
	engine->engineState.warnings.clear();
}

void addError(ObdCode errorCode) {
	engine->engineState.warnings.addWarningCode(errorCode);
}

void removeError(ObdCode errorCode) {
	engine->engineState.warnings.removeErrorCode(errorCode);
}

void getErrorCodes(error_codes_set_s *copy, ObdCodeFilter filter) {
	engine->engineState.warnings.getCodes(copy, filter);
}

bool hasErrorCodes(ObdCodeFilter filter) {
	return engine->engineState.warnings.hasCodes(filter);
}
