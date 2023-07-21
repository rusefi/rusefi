#pragma once

#include "ac_control_generated.h"
#include "timer.h"

class AcController : public ac_control_s, public EngineModule {
public:
	using interface_t = AcController;

	void onSlowCallback() override;

	virtual bool isAcEnabled() const;

private:
	bool getAcState();

	Timer m_timeSinceNoAc;
};
