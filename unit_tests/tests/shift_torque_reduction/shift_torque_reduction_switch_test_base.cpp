//
// Created by kifir on 12/17/24.
//

#include "pch.h"

#include "shift_torque_reduction_switch_test_base.h"

const EngineConfig ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG = EngineConfig()
	.setTorqueReductionEnabled(true)
	.setTorqueReductionTriggerPin(TEST_TORQUE_REDUCTION_BUTTON_PIN)
	.setLaunchActivatePin(TEST_LAUNCH_BUTTON_PIN)
	.setClutchDownPin(TEST_CLUTCH_DOWN_PIN);

void ShiftTorqueReductionSwitchTestBase::SetUp() {
	TestBase::SetUp();
	setUpEngineConfiguration(TEST_ENGINE_CONFIG);
}

void ShiftTorqueReductionSwitchTestBase::checkShiftTorqueReductionState(
	const char* const context,
	const bool expectedIsTorqueReductionTriggerPinValid,
	const bool expectedTorqueReductionTriggerPinState
) {
	periodicFastCallback(); // to update isTorqueReductionTriggerPinValid and torqueReductionTriggerPinState
	EXPECT_EQ(
		engine->shiftTorqueReductionController.isTorqueReductionTriggerPinValid,
		expectedIsTorqueReductionTriggerPinValid
	) << context;

	EXPECT_EQ(
		engine->shiftTorqueReductionController.torqueReductionTriggerPinState,
		expectedTorqueReductionTriggerPinState
	) << context;
}

void ShiftTorqueReductionSwitchTestBase::setPinState(const TestSwitchPin pin, const bool state) {
	switch (pin) {
		case TestSwitchPin::TORQUE_REDUCTION: {
			setMockState(TEST_TORQUE_REDUCTION_BUTTON_PIN, state);;
			break;
		}
		case TestSwitchPin::LUA_TORQUE_REDUCTION: {
			getTestLuaScriptExecutor().setTorqueReductionState(state);
			break;
		}
		case TestSwitchPin::LAUNCH: {
			setMockState(TEST_LAUNCH_BUTTON_PIN, state);;
			break;
		}
		case TestSwitchPin::CLUTCH_DOWN: {
			setMockState(TEST_CLUTCH_DOWN_PIN, state);;
			break;
		}
		case TestSwitchPin::LUA_CLUTCH_DOWN: {
			getTestLuaScriptExecutor().setClutchDownState(state);
			break;
		}
		default: {
			FAIL() << "Unexpected pin:" << static_cast<int>(pin);
			break;
		}
	}
}
