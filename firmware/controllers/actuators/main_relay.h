#pragma once

#include "engine_module.h"
#include "main_relay_generated.h"

class MainRelayController : public EngineModule, public main_relay_s {
public:
	void onSlowCallback() override;
	bool needsDelayedShutoff() override;

private:
	Timer m_lastIgnitionTime;
};
