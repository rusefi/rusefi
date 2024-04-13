// file ignition_controller.h

#pragma once

#include "engine_module.h"

#include <rusefi/timer.h>

class IgnitionController : public EngineModule {
public:
	void onSlowCallback() override;

private:
	Timer m_timeSinceIgnVoltage;
	bool m_lastState = false;
};

// USB power or vehicle power?
bool isIgnVoltage();
