#pragma once

#include <rusefi/timer.h>

// [tag:transient_timing] Signed ignition timing correction during throttle opening and
// closing events of the TPS detector (TpsAccelEnrichment). See transient_timing.cpp.
class TransientTiming : public EngineModule {
public:
	using interface_t = TransientTiming;

	void setDefaultConfiguration() override;
	void onEngineStop() override;

	// Evaluates this fast-callback sample. Engine::periodicFastCallback() calls it after
	// the TPS detector and before the ignition timing is computed.
	void update(efitick_t nowNt);

	// Degrees added to the ignition timing (positive advances). Zero unless the mode is
	// Apply.
	float getTimingCorrection() const {
		return m_applied;
	}

	enum class State : uint8_t {
		Idle = 0,
		Opening = 1,
		Closing = 2,
		Hold = 3,
		Decay = 4,
	};

#if EFI_UNIT_TEST
public:
#else
private:
#endif
	void reset();

	State m_state = State::Idle;
	// Table or curve value of the current or last event, before hold/decay and limits.
	float m_eventValue = 0;
	// After hold/decay shaping and limits; what Apply adds.
	float m_target = 0;
	float m_applied = 0;
	Timer m_holdTimer;
};

// Defaults, also used to seed tunes that predate the setting.
void setDefaultTransientTiming();
