#pragma once

#include "engine_module.h"
#include "main_relay_generated.h"

struct MainRelayController : public EngineModule, public main_relay_s {
	void onSlowCallback() override;
};
