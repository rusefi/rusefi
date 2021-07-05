#pragma once

#include "rusefi_types.h"

template <typename TValue, efitick_t TTimeoutPeriod>
class PeakDetect {
public:
	TValue detect(TValue currentValue, efitick_t nowNt) {
		if (
			(nowNt > m_lastPeakTime + TTimeoutPeriod) ||
			(currentValue > m_peak)) {
			m_peak = currentValue;
			m_lastPeakTime = nowNt;
		}

		return m_peak;
	}

private:
	TValue m_peak;
	efitick_t m_lastPeakTime;
};
