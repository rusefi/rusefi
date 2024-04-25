#pragma once

#include "rusefi_types.h"

/**
 * Stores the recent peak value, preventing loss of intermittent peaks in a signal.
 */
template <typename TValue, int64_t TTimeoutPeriod>
class PeakDetect {
public:
	TValue detect(TValue currentValue, efitick_t nowNt) {
		if ((nowNt > m_lastPeakTime + efidur_t{TTimeoutPeriod}) ||	// if timed out
			(currentValue > m_peak)) {						// or current is higher than the previous peak
			// store new peak and time
			m_peak = currentValue;
			m_lastPeakTime = nowNt;
		}

		return m_peak;
	}

private:
	TValue m_peak = std::numeric_limits<TValue>::min();
	efitick_t m_lastPeakTime = std::numeric_limits<efitick_t>::min();
};
