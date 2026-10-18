#pragma once

#include "burst_knock_state_generated.h"
#include <rusefi/timer.h>

// Feed-forward transient timing retard using the TPS acceleration-enrichment detector.
// This does not use acoustic knock events.
class BurstKnock : public burst_knock_state_s, public EngineModule {
public:
	using interface_t = BurstKnock;

	void setDefaultConfiguration() override;
	void onSlowCallback() override;
	float getTimingRetard() const;

private:
	Timer m_decayTimer;
	float m_latchedRetard = 0;
};
