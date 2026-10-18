#pragma once

#include "exp_average.h"
#include "efitime.h"
#include <cmath>

// Like PPS smoothing, this advances once per resolved sensor read, not per ADC sample.
// Callers must reset on invalid input and sensor reconfiguration.
class SmartExpAverage {
public:
	void reset() {
		m_average.reset();
		m_hasPrevious = false;
	}

	float update(float value, float alpha, float resetThreshold, float resetTimeMs) {
		const efitick_t nowNt = getTimeNowNt();
		const bool transient = m_hasPrevious
			&& resetThreshold > 0
			&& resetTimeMs > 0
			&& std::abs(value - m_lastRawValue) >= resetThreshold
			&& static_cast<float>(nowNt - m_lastRawTime) < MSF2NT(resetTimeMs);

		// Zero is a valid closed-throttle sample, so do not use initOrAverage's zero sentinel.
		// Invalid live calibration should pass through until startup/burn repairs it.
		const bool passThrough = !std::isfinite(alpha) || alpha <= 0 || alpha > 1;
		m_average.setSmoothingFactor(!m_hasPrevious || transient || passThrough ? 1 : alpha);
		m_lastRawValue = value;
		m_lastRawTime = nowNt;
		m_hasPrevious = true;
		return m_average.average(value);
	}

private:
	ExpAverage m_average;
	float m_lastRawValue = 0;
	efitick_t m_lastRawTime = 0;
	bool m_hasPrevious = false;
};
