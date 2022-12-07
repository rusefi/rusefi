#pragma once

#include "ac_control_generated.h"
#include "timer.h"

class AcController final : public ac_control_s, public EngineModule {
public:
	void onSlowCallback() override;

	bool isAcEnabled() const;

private:
	bool getAcState();

	Timer m_timeSinceNoAc;
};
