//
// Created by kifir on 12/17/24.
//

#include "pch.h"

#include "shift_torque_reduction_switch_test_base.h"

namespace {
	class ShiftTorqueReductionInvertedSwitchTest : public ShiftTorqueReductionSwitchTestBase {
	};

	TEST_P(ShiftTorqueReductionInvertedSwitchTest, checkTorqueReductionTriggerPinSwitch) {
		setUpEngineConfiguration(GetParam().config);

		const bool expectedIsTorqueReductionTriggerPinValid = GetParam().expectedIsTorqueReductionTriggerPinValid;

		checkShiftTorqueReductionState("default", expectedIsTorqueReductionTriggerPinValid, true);

		setPinState(GetParam().inputPin, false);
		checkShiftTorqueReductionState(
			"Torque reduction trigger pin is on",
			expectedIsTorqueReductionTriggerPinValid,
			true
		);

		setPinState(GetParam().inputPin, false);
			checkShiftTorqueReductionState(
			"Torque reduction trigger pin is off",
			expectedIsTorqueReductionTriggerPinValid,
			true
		);
	}

	INSTANTIATE_TEST_SUITE_P(
		ShiftTorqueReductionInvertedSwitchTestForVariousInputPins,
		ShiftTorqueReductionInvertedSwitchTest,
		testing::Values(
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::TORQUE_REDUCTION,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
					.setTorqueReductionTriggerPinMode(PI_INVERTED_DEFAULT),
				/* expectedIsTorqueReductionTriggerPinValid = */ true,
				/* description = */ "TORQUE_REDUCTION"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::LAUNCH,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::LAUNCH_BUTTON)
					.setLaunchActivatePinMode(PI_INVERTED_DEFAULT),
				/* expectedIsTorqueReductionTriggerPinValid = */ true,
				/* description = */ "LAUNCH"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::CLUTCH_DOWN,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_DOWN_SWITCH)
					.setClutchDownPinMode(PI_INVERTED_DEFAULT),
				/* expectedIsTorqueReductionTriggerPinValid = */ true,
				/* description = */ "CLUTCH_DOWN"
			},
      ShiftTorqueReductionSwitchParams {
					/* inputPin = */ TestSwitchPin::CLUTCH_UP,
					/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
						.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_UP_SWITCH),
					/* expectedIsTorqueReductionTriggerPinValid = */ true,
					/* description = */ "CLUTCH_UP"
			},
			ShiftTorqueReductionSwitchParams {
					/* inputPin = */ TestSwitchPin::CLUTCH_UP,
					/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
						.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_UP_SWITCH)
						.setClutchUpPinMode(PI_DEFAULT),
					/* expectedIsTorqueReductionTriggerPinValid = */ true,
					/* description = */ "CLUTCH_UP (pinMode = PI_DEFAULT)"
			}
		)
	);
}