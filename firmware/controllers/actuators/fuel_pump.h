#pragma once

#include "engine_module.h"
#include "fuel_pump_generated.h"

struct FuelPumpController : public EngineModule, public fuel_pump_control_s {
	void onSlowCallback() override;
	bool getPumpState();
};
