#pragma once

#include "engine_module.h"

#include "timer.h"

class IgnitionController : public EngineModule {
public:
	void onSlowCallback() override;

private:
	Timer m_timeout;
	bool m_lastState = false;
};
