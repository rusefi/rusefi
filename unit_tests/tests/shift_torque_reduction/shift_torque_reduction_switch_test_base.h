//
// Created by kifir on 12/17/24.
//

#pragma once

#include "util/test_base.h"

#include "shift_torque_reduction_switch_params.h"

class ShiftTorqueReductionSwitchTestBase : public TestBase<
	testing::TestWithParam<ShiftTorqueReductionSwitchParams>
> {
public:
	static const EngineConfig TEST_ENGINE_CONFIG;
protected:
	static constexpr switch_input_pin_e TEST_TORQUE_REDUCTION_BUTTON_PIN = Gpio::F15;
	static constexpr switch_input_pin_e TEST_LAUNCH_BUTTON_PIN = Gpio::G15;
	static constexpr switch_input_pin_e TEST_CLUTCH_DOWN_PIN = Gpio::E15;
	static constexpr switch_input_pin_e TEST_CLUTCH_UP_PIN = Gpio::D15;

	void SetUp() override;

	void checkShiftTorqueReductionState(
		const char* context,
		bool expectedIsTorqueReductionTriggerPinValid,
		bool expectedTorqueReductionTriggerPinState
	);

	void setPinState(TestSwitchPin pin, bool state);
};