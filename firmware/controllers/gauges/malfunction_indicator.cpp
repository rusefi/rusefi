/**
 * @file malfunction_indicator.cpp
 * @brief We can blink out OBD-II error codes using Malfunction Indicator Light (MIL)
 *
 *
 * @date Dec 20, 2013
 * @author Konstantin Nikonenko
 * @author Andrey Belomutskiy, (c) 2012-2020
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
 * TODO: move it to firmware/controllers/modules
 */

#include "pch.h"

#include "bench_test.h"
#include "malfunction_central.h"
#include "malfunction_indicator.h"

static constexpr float ShortPulseMs = 400;
static constexpr float LongPulseMs = 1500;
static constexpr float PulseGapMs = 400;

void MILController::startPulse() {
	enginePins.checkEnginePin.setValue("MIL", true);
	m_phase = Phase::Pulse;
	m_phaseTimer.reset();
}

void MILController::startDigit() {
	const int code = static_cast<int>(m_activeCode);
	m_pulsesRemaining = (code / m_divisor) % 10 + 1;
	startPulse();
}

void MILController::startCode(ObdCode code) {
	m_activeCode = code;
	m_divisor = 1;
	m_digitPlace = 0;

	for (int value = static_cast<int>(code); value >= 10; value /= 10) {
		m_divisor *= 10;
		m_digitPlace++;
	}

	startDigit();
}

bool MILController::isActiveCodePresent() const {
	error_codes_set_s codes;
	getErrorCodes(&codes);

	for (int i = 0; i < codes.count; i++) {
		if (codes.error_codes[i] == m_activeCode) {
			return true;
		}
	}

	return false;
}

void MILController::startNextCode() {
	error_codes_set_s codes;
	getErrorCodes(&codes);

	if (codes.count == 0) {
		m_phase = Phase::Idle;
		m_activeCode = ObdCode::None;
		return;
	}

	int next = 0;
	for (int i = 0; i < codes.count; i++) {
		if (codes.error_codes[i] == m_activeCode) {
			next = (i + 1) % codes.count;
			break;
		}
	}

	startCode(codes.error_codes[next]);
}

void MILController::onSlowCallback() {
	if (getOutputOnTheBenchTest() == &enginePins.checkEnginePin) {
		m_wasBenchActive = true;
		return;
	}

	if (m_wasBenchActive) {
		m_wasBenchActive = false;
		m_phase = Phase::Idle;
		m_activeCode = ObdCode::None;
	}

	if (!hasErrorCodes()) {
		m_phase = Phase::Idle;
		m_activeCode = ObdCode::None;
		enginePins.checkEnginePin.setValue("MIL", false);
		return;
	}

	if (m_phase == Phase::Idle || !isActiveCodePresent()) {
		startNextCode();
		return;
	}

	if (m_phase == Phase::Pulse) {
		const float duration = m_digitPlace % 2 == 0 ? ShortPulseMs : LongPulseMs;
		if (m_phaseTimer.hasElapsedMs(duration)) {
			enginePins.checkEnginePin.setValue("MIL", false);
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
		startNextCode();
	}
}
