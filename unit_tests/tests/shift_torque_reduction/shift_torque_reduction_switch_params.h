//
// Created by kifir on 12/17/24.
//

#pragma once

#include "util/engine_config.h"

enum class TestSwitchPin {
	TORQUE_REDUCTION,
	LUA_TORQUE_REDUCTION,
	LAUNCH,
	CLUTCH_DOWN,
	LUA_CLUTCH_DOWN,
	CLUTCH_UP,
	LUA_CLUTCH_UP,
};

struct ShiftTorqueReductionSwitchParams {
	const TestSwitchPin inputPin;
	const EngineConfig config;
	const bool expectedIsTorqueReductionTriggerPinValid;
	const std::string description;
};

// the following function is used by GoogleTest for pretty printing ShiftTorqueReductionSwitchParams
void PrintTo(const ShiftTorqueReductionSwitchParams& params, std::ostream* os);