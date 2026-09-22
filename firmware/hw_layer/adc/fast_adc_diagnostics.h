#pragma once

#include <cstddef>
#include <cstdint>

// Caller serializes record() and takePending() with the ADC interrupt.
// Coalesce repeated events, but retain each distinct reason until reported.
class FastAdcDiagnostics {
public:
	static constexpr uint8_t CompletionPending = 1;
	static constexpr uint8_t ConversionBusy = 2;
	static constexpr uint8_t Other = 4;

	void record(bool active, size_t remaining, bool transferComplete, bool dmaEnabled, bool hardwareError) {
		if (!active || hardwareError) {
			m_pending |= Other;
		} else if (transferComplete) {
			// TC was sampled after NDTR: completion can occur between the reads.
			m_pending |= CompletionPending;
		} else if (dmaEnabled && remaining > 0) {
			// Describes the observed state, not proof of a late software start.
			m_pending |= ConversionBusy;
		} else {
			// Includes the NDTR=0 / TC-not-yet-visible transition. Don't guess.
			m_pending |= Other;
		}
	}

	uint8_t takePending() {
		auto pending = m_pending;
		m_pending = 0;
		return pending;
	}

private:
	uint8_t m_pending = 0;
};
