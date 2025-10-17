//
// Created by kifir on 12/17/24.
//

#include "pch.h"

#include "shift_torque_reduction_switch_test_base.h"

namespace {
	class ShiftTorqueReductionSwitchTest : public ShiftTorqueReductionSwitchTestBase {
	protected:
		void checkThatOtherPinsAreIgnored(TestSwitchPin exceptPin, const std::string& context);
	};

	void ShiftTorqueReductionSwitchTest::checkThatOtherPinsAreIgnored(
		const TestSwitchPin exceptPin,
		const std::string& context
	) {
		static const TestSwitchPin testInputPins[] = {
			TestSwitchPin::TORQUE_REDUCTION,
			TestSwitchPin::LUA_TORQUE_REDUCTION,
			TestSwitchPin::LAUNCH,
			TestSwitchPin::CLUTCH_DOWN,
			TestSwitchPin::LUA_CLUTCH_DOWN,
			TestSwitchPin::CLUTCH_UP,
			TestSwitchPin::LUA_CLUTCH_UP,
		};
		const bool expectedIsTorqueReductionTriggerPinValid =
			engine->shiftTorqueReductionController.isTorqueReductionTriggerPinValid;
		const bool expectedTorqueReductionTriggerPinState =
			engine->shiftTorqueReductionController.torqueReductionTriggerPinState;
		for (const TestSwitchPin& pin: testInputPins) {
			if (pin != exceptPin) {
				setPinState(pin, true);
				checkShiftTorqueReductionState(
					context.c_str(),
					expectedIsTorqueReductionTriggerPinValid,
					expectedTorqueReductionTriggerPinState
				);

				setPinState(pin, false);
				checkShiftTorqueReductionState(
					context.c_str(),
					expectedIsTorqueReductionTriggerPinValid,
					expectedTorqueReductionTriggerPinState
				);
			}
		}
	}

	TEST_P(ShiftTorqueReductionSwitchTest, checkTorqueReductionTriggerPinSwitch) {
		setUpEngineConfiguration(GetParam().config);

		const bool expectedIsTorqueReductionTriggerPinValid = GetParam().expectedIsTorqueReductionTriggerPinValid;

		checkShiftTorqueReductionState("default", expectedIsTorqueReductionTriggerPinValid, false);

		const TestSwitchPin inputPin = GetParam().inputPin;
		setPinState(inputPin, true);
		checkShiftTorqueReductionState(
			"Torque reduction trigger pin is on",
			expectedIsTorqueReductionTriggerPinValid,
			true
		);
		checkThatOtherPinsAreIgnored(inputPin, "Other pins are ignored when torque reduction trigger pin is on");

		setPinState(GetParam().inputPin, false);
		checkShiftTorqueReductionState(
			"Torque reduction trigger pin is off",
			expectedIsTorqueReductionTriggerPinValid,
			false
		);
		checkThatOtherPinsAreIgnored(inputPin, "Other pins are ignored when torque reduction trigger pin is off");
	}

	INSTANTIATE_TEST_SUITE_P(
		ShiftTorqueReductionSwitchTestForVariousInputPins,
		ShiftTorqueReductionSwitchTest,
		testing::Values(
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::TORQUE_REDUCTION,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON),
				/* expectedIsTorqueReductionTriggerPinValid = */ true,
				/* description = */ "TORQUE_REDUCTION"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::TORQUE_REDUCTION,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
					.setTorqueReductionTriggerPinMode(PI_DEFAULT),
				/* expectedIsTorqueReductionTriggerPinValid = */ true,
				/* description = */ "TORQUE_REDUCTION (pinMode = PI_DEFAULT)"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::LUA_TORQUE_REDUCTION,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
					.setTorqueReductionTriggerPin(Gpio::Unassigned),
				/* expectedIsTorqueReductionTriggerPinValid = */ false,
				/* description = */ "LUA_TORQUE_REDUCTION"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::LUA_TORQUE_REDUCTION,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
					.setTorqueReductionTriggerPin(Gpio::Unassigned)
					.setTorqueReductionTriggerPinMode(PI_DEFAULT),
				/* expectedIsTorqueReductionTriggerPinValid = */ false,
				/* description = */ "LUA_TORQUE_REDUCTION (pinMode = PI_DEFAULT)"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::LUA_TORQUE_REDUCTION,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
					.setTorqueReductionTriggerPin(Gpio::Unassigned)
					.setTorqueReductionTriggerPinMode(PI_INVERTED_DEFAULT),
				/* expectedIsTorqueReductionTriggerPinValid = */ false,
				/* description = */ "LUA_TORQUE_REDUCTION (pinMode = PI_INVERTED_DEFAULT)"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::LAUNCH,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::LAUNCH_BUTTON),
				/* expectedIsTorqueReductionTriggerPinValid = */ true,
				/* description = */ "LAUNCH"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::LAUNCH,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::LAUNCH_BUTTON)
					.setLaunchActivatePinMode(PI_DEFAULT),
				/* expectedIsTorqueReductionTriggerPinValid = */ true,
				/* description = */ "LAUNCH (pinMode = PI_DEFAULT)"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::CLUTCH_DOWN,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_DOWN_SWITCH),
				/* expectedIsTorqueReductionTriggerPinValid = */ true,
				/* description = */ "CLUTCH_DOWN"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::CLUTCH_DOWN,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_DOWN_SWITCH)
					.setClutchDownPinMode(PI_DEFAULT),
				/* expectedIsTorqueReductionTriggerPinValid = */ true,
				/* description = */ "CLUTCH_DOWN (pinMode = PI_DEFAULT)"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::LUA_CLUTCH_DOWN,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_DOWN_SWITCH)
					.setClutchDownPin(Gpio::Unassigned),
				/* expectedIsTorqueReductionTriggerPinValid = */ false,
				/* description = */ "LUA_CLUTCH_DOWN"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::LUA_CLUTCH_DOWN,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_DOWN_SWITCH)
					.setClutchDownPin(Gpio::Unassigned)
					.setClutchDownPinMode(PI_DEFAULT),
				/* expectedIsTorqueReductionTriggerPinValid = */ false,
				/* description = */ "LUA_CLUTCH_DOWN (pinMode = PI_DEFAULT)"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::LUA_CLUTCH_DOWN,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_DOWN_SWITCH)
					.setClutchDownPin(Gpio::Unassigned)
					.setClutchDownPinMode(PI_INVERTED_DEFAULT),
				/* expectedIsTorqueReductionTriggerPinValid = */ false,
				/* description = */ "LUA_CLUTCH_DOWN (pinMode = PI_INVERTED_DEFAULT)"
			},
			ShiftTorqueReductionSwitchParams {
				/* inputPin = */ TestSwitchPin::CLUTCH_UP,
				/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
					.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_UP_SWITCH)
					.setClutchUpPinMode(PI_INVERTED_DEFAULT),
				/* expectedIsTorqueReductionTriggerPinValid = */ true,
				/* description = */ "CLUTCH_UP"
			},
			ShiftTorqueReductionSwitchParams {
					/* inputPin = */ TestSwitchPin::LUA_CLUTCH_UP,
					/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
						.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_UP_SWITCH)
						.setClutchUpPin(Gpio::Unassigned),
					/* expectedIsTorqueReductionTriggerPinValid = */ false,
					/* description = */ "LUA_CLUTCH_UP"
			},
			ShiftTorqueReductionSwitchParams {
					/* inputPin = */ TestSwitchPin::LUA_CLUTCH_UP,
					/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
		                 .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_UP_SWITCH)
		                 .setClutchUpPin(Gpio::Unassigned)
		                 .setClutchUpPinMode(PI_DEFAULT),
					/* expectedIsTorqueReductionTriggerPinValid = */ false,
					/* description = */ "LUA_CLUTCH_UP (pinMode = PI_DEFAULT)"
			},
			ShiftTorqueReductionSwitchParams {
					/* inputPin = */ TestSwitchPin::LUA_CLUTCH_UP,
					/* config = */ ShiftTorqueReductionSwitchTestBase::TEST_ENGINE_CONFIG.clone()
						.setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_UP_SWITCH)
						.setClutchUpPin(Gpio::Unassigned)
						.setClutchUpPinMode(PI_INVERTED_DEFAULT),
					/* expectedIsTorqueReductionTriggerPinValid = */ false,
					/* description = */ "LUA_CLUTCH_UP (pinMode = PI_INVERTED_DEFAULT)"
			}
		)
	);
}