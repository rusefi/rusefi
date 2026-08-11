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

class MILController : public EngineModule {
public:
	void onSlowCallback() override;

private:
	enum class Phase : uint8_t {
		Idle,
		Pulse,
		Gap,
	};

	void startCode(ObdCode code);
	void startDigit();
	void startPulse();
	void startNextCode();
	bool isActiveCodePresent() const;

	Phase m_phase = Phase::Idle;
	ObdCode m_activeCode = ObdCode::None;
	Timer m_phaseTimer;
	int m_divisor = 1;
	int m_digitPlace = 0;
	int m_pulsesRemaining = 0;
	bool m_wasBenchActive = false;
};
