/**
 * @file malfunction_indicator.h
 * @brief We can blink out OBD-II error codes using Malfunction Indicator Light (MIL)
 *
 *
 * @date Dec 20, 2013
 * @author Konstantin Nikonenko
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_module.h"
#include "obd_error_codes.h"
#include "rusefi/timer.h"
#include <atomic>

struct error_codes_set_s;

class MILController : public EngineModule {
public:
	void onSlowCallback() override;
	void onIgnitionStateChanged(bool ignitionOn) override;

	// Logical output, available even with no checkEnginePin assigned. Safe to
	// read from a CAN TX thread; the slow callback owns the blink state machine.
	bool isLampOn() const;
	// True only during diagnostic pulses, excluding the stopped-engine lamp
	// check and GPIO bench tests. Useful when CAN has its own lamp-check policy.
	bool isDiagnosticLampOn() const;
	// True throughout a diagnostic sequence, including its off gaps.
	bool isBlinking() const;
	// True if any code present
	bool isError() const;

private:
	enum class LampState : uint8_t {
		Off,
		On,
		DiagnosticOff,
		DiagnosticOn,
	};
	void setLamp(bool on, bool diagnostic = false);
	enum class Phase : uint8_t {
		Idle,
		// steady lamp before the first code and after the last one
		Prelude,
		Pulse,
		Gap,
		// long pause between two consecutive codes
		CodeGap,
		// long pause after last code
		CodeGapLast,
	};

	void startPrelude();
	void startCode(ObdCode code);
	void startDigit();
	void startPulse();
	void startNextCode(const error_codes_set_s& codes);
	bool isActiveCodePresent(const error_codes_set_s& codes) const;
	bool isLastCode(const error_codes_set_s& codes) const;

	std::atomic<LampState> m_lampState{LampState::Off};

	Phase m_phase = Phase::Idle;
	// the code we are currently blinking out, as stored in the error codes set
	ObdCode m_activeObdCode = ObdCode::None;
	// the same code after the optional board translation: this is the number actually blinked
	uint32_t m_activeCode = 0;
	Timer m_phaseTimer;
	int m_divisor = 1;
	int m_digitPlace = 0;
	int m_pulsesRemaining = 0;
	bool m_wasBenchActive = false;
	bool m_ignitionOn = false;
};
