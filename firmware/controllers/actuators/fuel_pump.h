#pragma once

#include "engine_module.h"
#include "fuel_pump_generated.h"

struct FuelPumpController : public EngineModule {
	void onSlowCallback() override;
};
