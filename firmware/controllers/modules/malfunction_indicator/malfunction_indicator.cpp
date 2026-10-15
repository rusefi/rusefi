/**
 * @file malfunction_indicator.cpp
 * @brief We can blink out OBD-II error codes using Malfunction Indicator Light (MIL)
 *
 *
 * @date Dec 20, 2013
 * @author Konstantin Nikonenko
 * @author Andrey Belomutskiy, (c) 2012-2020
 * first we start on OFF state, then ON when we have ignition voltage, then OFF after we have rpm,
 * then if we have any error:
 * we show 4 digit error code - 1,5sec * (4xxx+1) digit + 0,4sec * (x3xxx+1) + ....
 * ATTENTION!!! 0 = 1 blink, 1 = 2 blinks, ...., 9 = 10 blinks
 * sequence is the constant!!!
 *
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "pch.h"

#include "bench_test.h"
#include "board_overrides.h"
#include "malfunction_central.h"
#include "malfunction_indicator.h"

static constexpr float ShortPulseMs = 400;
static constexpr float LongPulseMs = 1500;
static constexpr float PulseGapMs = 400;

// Optional board hook: decides which OBD codes are allowed to be blinked out at all.
std::optional<board_mil_code_filter_type> custom_board_milCodeFilter;
// Optional board hook: maps an OBD code onto a shorter board specific number which is
// what actually gets blinked out. Not installed by default, in that case the OBD code
// itself is blinked.
std::optional<board_mil_code_translator_type> custom_board_milCodeTranslator;

bool MILController::isLampOn() const {
	const auto state = m_lampState.load(std::memory_order_relaxed);
	return state == LampState::On || state == LampState::DiagnosticOn;
}

bool MILController::isDiagnosticLampOn() const {
	return m_lampState.load(std::memory_order_relaxed) == LampState::DiagnosticOn;
}

bool MILController::isBlinking() const {
	const auto state = m_lampState.load(std::memory_order_relaxed);
	return state == LampState::DiagnosticOn || state == LampState::DiagnosticOff;
}

bool MILController::isError() const {
	return m_activeObdCode != ObdCode::None;
}

void MILController::setLamp(bool on, bool diagnostic) {
	m_lampState.store(diagnostic
		? (on ? LampState::DiagnosticOn : LampState::DiagnosticOff)
		: (on ? LampState::On : LampState::Off), std::memory_order_relaxed);
	enginePins.checkEnginePin.setValue("MIL", on);
}

void MILController::onIgnitionStateChanged(bool ignitionOn) {
	m_ignitionOn = ignitionOn;
}

void MILController::startPulse() {
	setLamp(true, true);
	m_phase = Phase::Pulse;
	m_phaseTimer.reset();
}

void MILController::startDigit() {
	// blinking is driven by the translated (displayed) number, not by the raw OBD code
	const int code = static_cast<int>(m_activeCode);
	m_pulsesRemaining = (code / m_divisor) % 10 + 1;
	startPulse();
}

void MILController::startCode(ObdCode code) {
	// m_activeObdCode is the identity of the code: it is matched against the error code set,
	// while m_activeCode is only the number we blink out.
	m_activeObdCode = code;
	m_divisor = 1;
	m_digitPlace = 0;
	// board may shorten the code, otherwise fall back to the OBD code value itself
	m_activeCode = get_board_override_result(custom_board_milCodeTranslator,
		static_cast<uint32_t>(m_activeObdCode), m_activeObdCode);

	// find the highest decimal place so that digits are blinked from the most significant one
	for (int value = static_cast<int>(code); value >= 10; value /= 10) {
		m_divisor *= 10;
		m_digitPlace++;
	}

	startDigit();
}

bool MILController::isActiveCodePresent(const error_codes_set_s& codes) const {
	for (size_t i = 0; i < codes.count; i++) {
		if (codes.error_codes[i] == m_activeObdCode) {
			return true;
		}
	}

	return false;
}

void MILController::startNextCode(const error_codes_set_s& codes) {
	if (codes.count == 0) {
		m_phase = Phase::Idle;
		m_activeObdCode = ObdCode::None;
		return;
	}

	int next = 0;
	for (size_t i = 0; i < codes.count; i++) {
		if (codes.error_codes[i] == m_activeObdCode) {
			next = (i + 1) % codes.count;
			break;
		}
	}

	startCode(codes.error_codes[next]);
}

void MILController::onSlowCallback() {
	if (getOutputOnTheBenchTest() == &enginePins.checkEnginePin) {
		m_wasBenchActive = true;
		m_lampState.store(enginePins.checkEnginePin.getLogicValue() ? LampState::On : LampState::Off,
			std::memory_order_relaxed);
		return;
	}

	if (m_wasBenchActive) {
		m_wasBenchActive = false;
		m_phase = Phase::Idle;
		m_activeObdCode = ObdCode::None;
	}

	if (!m_ignitionOn) {
		m_phase = Phase::Idle;
		m_activeObdCode = ObdCode::None;
		setLamp(false);
		return;
	}

	// Use one filtered snapshot for presence checks and code rotation so a
	// rejected code can never start or keep a blink sequence alive.
	error_codes_set_s codes;
	getErrorCodes(&codes, custom_board_milCodeFilter.value_or(nullptr));
	if (codes.count == 0) {
		m_phase = Phase::Idle;
		m_activeObdCode = ObdCode::None;
		setLamp(engine->rpmCalculator.isStopped());
		return;
	}

	if (m_phase == Phase::Idle || !isActiveCodePresent(codes)) {
		startNextCode(codes);
		return;
	}

	if (m_phase == Phase::Pulse) {
		const float duration = m_digitPlace % 2 == 0 ? ShortPulseMs : LongPulseMs;
		if (m_phaseTimer.hasElapsedMs(duration)) {
			setLamp(false, true);
			m_phase = Phase::Gap;
			m_phaseTimer.reset();
		}
		return;
	}

	if (!m_phaseTimer.hasElapsedMs(PulseGapMs)) {
		return;
	}

	if (--m_pulsesRemaining > 0) {
		startPulse();
	} else if (m_divisor > 1) {
		m_divisor /= 10;
		m_digitPlace--;
		startDigit();
	} else {
		startNextCode(codes);
	}
}
