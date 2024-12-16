//
// Created by kifir on 9/30/24.
//

#include "pch.h"

#include "util/test_base.h"

namespace {
    constexpr switch_input_pin_e TEST_TORQUE_REDUCTION_BUTTON_PIN = Gpio::F15;
    constexpr switch_input_pin_e TEST_LAUNCH_BUTTON_PIN = Gpio::G15;
    constexpr switch_input_pin_e TEST_CLUTCH_DOWN_PIN = Gpio::E15;

    struct ShiftTorqueReductionTriggerPinTestData {
        const std::string context;
        const bool isTorqueReductionTriggerPinValid;
        const bool torqueReductionTriggerPinState;
    };

    class ShiftTorqueReductionTriggerPinTest : public TestBase {
    protected:
        void checkShiftTorqueReductionState(const ShiftTorqueReductionTriggerPinTestData& expected);
        void checkShiftTorqueReductionStateAfterPeriodicFastCallback(
            const ShiftTorqueReductionTriggerPinTestData& expected
        );
        void checkShiftTorqueReductionStateWithPinState(
            switch_input_pin_e triggerPin,
            bool pinState,
            const ShiftTorqueReductionTriggerPinTestData& expected
        );
        void checkShiftTorqueReductionStateWithLuaTorqueReductionState(
            bool luaTorqueReductionState,
            const ShiftTorqueReductionTriggerPinTestData& expected
        );
        void checkShiftTorqueReductionStateWithLuaClutchDownState(
                bool luaClutchDownState,
                const ShiftTorqueReductionTriggerPinTestData& expected
        );
    };

    void ShiftTorqueReductionTriggerPinTest::checkShiftTorqueReductionState(
        const ShiftTorqueReductionTriggerPinTestData& expected
    ) {
        EXPECT_EQ(
            engine->shiftTorqueReductionController.isTorqueReductionTriggerPinValid,
            expected.isTorqueReductionTriggerPinValid
        ) << expected.context;

        EXPECT_EQ(
            engine->shiftTorqueReductionController.torqueReductionTriggerPinState,
            expected.torqueReductionTriggerPinState
        ) << expected.context;
    }

    void ShiftTorqueReductionTriggerPinTest::checkShiftTorqueReductionStateAfterPeriodicFastCallback(
        const ShiftTorqueReductionTriggerPinTestData& expected
    ) {
        periodicFastCallback();
        checkShiftTorqueReductionState(expected);
    }

    void ShiftTorqueReductionTriggerPinTest::checkShiftTorqueReductionStateWithPinState(
        const switch_input_pin_e triggerPin,
        const bool pinState,
        const ShiftTorqueReductionTriggerPinTestData& expected
    ) {
        setMockState(triggerPin, pinState);
        checkShiftTorqueReductionStateAfterPeriodicFastCallback(expected);
    }

    void ShiftTorqueReductionTriggerPinTest::checkShiftTorqueReductionStateWithLuaTorqueReductionState(
        const bool luaTorqueReductionState,
        const ShiftTorqueReductionTriggerPinTestData& expected
    ) {
        getTestLuaScriptExecutor().setTorqueReductionState(luaTorqueReductionState);
        checkShiftTorqueReductionStateAfterPeriodicFastCallback(expected);
    }

    void ShiftTorqueReductionTriggerPinTest::checkShiftTorqueReductionStateWithLuaClutchDownState(
        const bool luaClutchDownState,
        const ShiftTorqueReductionTriggerPinTestData& expected
    ) {
        getTestLuaScriptExecutor().setClutchDownState(luaClutchDownState);
        checkShiftTorqueReductionStateAfterPeriodicFastCallback(expected);
    }

    TEST_F(ShiftTorqueReductionTriggerPinTest, checkDefaultConfiguration) {
        setUpEngineConfiguration(EngineConfig());

        checkShiftTorqueReductionStateAfterPeriodicFastCallback({ "Default trigger pin state", false, false });
    }

    TEST_F(ShiftTorqueReductionTriggerPinTest, checkTorqueReductionTriggerPinSwitch) {
        setUpEngineConfiguration(EngineConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
            .setTorqueReductionTriggerPin(TEST_TORQUE_REDUCTION_BUTTON_PIN)
            .setLaunchActivatePin(TEST_LAUNCH_BUTTON_PIN)
        );

        checkShiftTorqueReductionStateAfterPeriodicFastCallback({ "Default trigger pin state", true, false });
        checkShiftTorqueReductionStateWithPinState(
            TEST_TORQUE_REDUCTION_BUTTON_PIN,
            true,
            { "Torque reduction trigger pin is on", true, true }
        );
        checkShiftTorqueReductionStateWithPinState(
            TEST_TORQUE_REDUCTION_BUTTON_PIN,
            false,
            { "Torque reduction trigger pin is off", true, false }
        );

        // Check that launch button pin switching does not switch shift torque reduction trigger pin:
        checkShiftTorqueReductionStateWithPinState(
            TEST_LAUNCH_BUTTON_PIN,
            true,
            { "Launch activate pin is on", true, false }
        );
        checkShiftTorqueReductionStateWithPinState(
            TEST_LAUNCH_BUTTON_PIN,
            false,
            { "Launch activate pin is off", true, false }
        );
    }

        TEST_F(ShiftTorqueReductionTriggerPinTest, checkClutchDownPinSwitch) {
        setUpEngineConfiguration(EngineConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_DOWN_SWITCH)
            .setTorqueReductionTriggerPin(TEST_TORQUE_REDUCTION_BUTTON_PIN)
            .setLaunchActivatePin(TEST_LAUNCH_BUTTON_PIN)
            .setClutchDownPin(TEST_CLUTCH_DOWN_PIN)
        );

        checkShiftTorqueReductionStateAfterPeriodicFastCallback({ "Default trigger pin state", true, false });
        checkShiftTorqueReductionStateWithPinState(
            TEST_CLUTCH_DOWN_PIN,
            true,
            { "Clutch down pin is on", true, true }
        );
        checkShiftTorqueReductionStateWithPinState(
            TEST_CLUTCH_DOWN_PIN,
            false,
            { "Clutch down pin is off", true, false }
        );

        // Check that launch button pin switching does not switch shift torque reduction trigger pin:
        checkShiftTorqueReductionStateWithPinState(
            TEST_LAUNCH_BUTTON_PIN,
            true,
            { "Clutch down pin is on", true, false }
        );
        checkShiftTorqueReductionStateWithPinState(
            TEST_LAUNCH_BUTTON_PIN,
            false,
            { "Launch activate pin is off", true, false }
        );
    }

    TEST_F(ShiftTorqueReductionTriggerPinTest, checkTorqueReductionTriggerInvertedPinSwitch) {
        setUpEngineConfiguration(EngineConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
            .setTorqueReductionTriggerPin(TEST_TORQUE_REDUCTION_BUTTON_PIN)
            .setTorqueReductionTriggerPinInverted(true)
        );

        checkShiftTorqueReductionStateAfterPeriodicFastCallback({ "Default trigger pin state", true, true });
        checkShiftTorqueReductionStateWithPinState(
            TEST_TORQUE_REDUCTION_BUTTON_PIN,
            true,
            { "Torque reduction trigger pin is on", true, false }
        );
        checkShiftTorqueReductionStateWithPinState(
            TEST_TORQUE_REDUCTION_BUTTON_PIN,
            false,
            { "Torque reduction trigger pin is off", true, true }
        );
    }

    TEST_F(ShiftTorqueReductionTriggerPinTest, checkLaunchActivatePinSwitch) {
        setUpEngineConfiguration(EngineConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionActivationMode(torqueReductionActivationMode_e::LAUNCH_BUTTON)
            .setTorqueReductionTriggerPin(TEST_TORQUE_REDUCTION_BUTTON_PIN)
            .setLaunchActivatePin(TEST_LAUNCH_BUTTON_PIN)
        );

        checkShiftTorqueReductionStateAfterPeriodicFastCallback({ "Default trigger pin state", true, false });
        checkShiftTorqueReductionStateWithPinState(
            TEST_LAUNCH_BUTTON_PIN,
            true,
            { "Launch activate pin is on", true, true }
        );
        checkShiftTorqueReductionStateWithPinState(
            TEST_LAUNCH_BUTTON_PIN,
            false,
            { "Launch activate pin is off", true, false }
        );

        // Check that launch button pin switching does not switch shift torque reduction state:
        checkShiftTorqueReductionStateWithPinState(
            TEST_TORQUE_REDUCTION_BUTTON_PIN,
            true,
            { "Torque reduction trigger pin is on", true, false }
        );
        checkShiftTorqueReductionStateWithPinState(
            TEST_TORQUE_REDUCTION_BUTTON_PIN,
            false,
            { "Torque reduction trigger pin is off", true, false }
        );

        // Check that Lua `setTorqueReductionState` hook does not switch shift torque reduction state:
        checkShiftTorqueReductionStateWithLuaTorqueReductionState(
            true,
            { "Lua torque reduction state is on", true, false }
        );
        checkShiftTorqueReductionStateWithLuaTorqueReductionState(
            false,
            { "Lua torque reduction state is off", true, false }
        );
}

    TEST_F(ShiftTorqueReductionTriggerPinTest, checkTorqueReductionTriggerPinSwitchWithDisabledTorqueReduction) {
        setUpEngineConfiguration(EngineConfig()
            .setTorqueReductionEnabled(false)
            .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
            .setTorqueReductionTriggerPin(TEST_TORQUE_REDUCTION_BUTTON_PIN)
            .setLaunchActivatePin(TEST_LAUNCH_BUTTON_PIN)
        );

        checkShiftTorqueReductionStateAfterPeriodicFastCallback({ "Default trigger pin state", false, false });
        checkShiftTorqueReductionStateWithPinState(
            TEST_TORQUE_REDUCTION_BUTTON_PIN,
            true,
            { "Torque reduction trigger pin is on", false, false }
        );
        checkShiftTorqueReductionStateWithPinState(
            TEST_TORQUE_REDUCTION_BUTTON_PIN,
            false,
            { "Torque reduction trigger pin is off", false, false }
        );
        checkShiftTorqueReductionStateWithPinState(
            TEST_LAUNCH_BUTTON_PIN,
            true,
            { "Launch activate pin is on", false, false }
        );
        checkShiftTorqueReductionStateWithPinState(
            TEST_LAUNCH_BUTTON_PIN,
            false,
            { "Launch activate pin is off", false, false }
        );
    }

    TEST_F(ShiftTorqueReductionTriggerPinTest, checkTorqueReductionLuaSwitch) {
        setUpEngineConfiguration(EngineConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
            .setTorqueReductionTriggerPin(Gpio::Unassigned)
        );

        checkShiftTorqueReductionStateAfterPeriodicFastCallback({ "Default trigger pin state", false, false });
        checkShiftTorqueReductionStateWithLuaTorqueReductionState(
            true,
            { "Lua torque reduction state is on", false, true }
        );
        checkShiftTorqueReductionStateWithLuaTorqueReductionState(
            false,
            { "Lua torque reduction state is off", false, false }
        );
    }

    TEST_F(ShiftTorqueReductionTriggerPinTest, checkClutchDownLuaSwitch) {
        setUpEngineConfiguration(EngineConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_CLUTCH_DOWN_SWITCH)
            .setClutchDownPin(Gpio::Unassigned)
        );

        checkShiftTorqueReductionStateAfterPeriodicFastCallback({ "Default trigger pin state", false, false });
        checkShiftTorqueReductionStateWithLuaClutchDownState(
            true,
            { "Lua clutch down state is on", false, true }
        );
        checkShiftTorqueReductionStateWithLuaClutchDownState(
            false,
            { "Lua clutch down state is off", false, false }
        );
    }

    TEST_F(ShiftTorqueReductionTriggerPinTest, checkTorqueReductionLuaSwitchWithValidTorqueReductionButonPin) {
        setUpEngineConfiguration(EngineConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
            .setTorqueReductionTriggerPin(TEST_TORQUE_REDUCTION_BUTTON_PIN)
        );

        checkShiftTorqueReductionStateAfterPeriodicFastCallback({ "Default trigger pin state", true, false });
        checkShiftTorqueReductionStateWithLuaTorqueReductionState(
            true,
            { "Lua torque reduction state is on", true, false }
        );
        checkShiftTorqueReductionStateWithLuaTorqueReductionState(
            false,
            { "Lua torque reduction state is off", true, false }
        );
    }
}