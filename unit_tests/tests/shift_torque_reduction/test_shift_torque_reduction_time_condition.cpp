//
// Created by kifir on 10/2/24.
//

#include "pch.h"

#include "shift_torque_reduction_test_base.h"

namespace {
    constexpr switch_input_pin_e TEST_TORQUE_REDUCTION_BUTTON_PIN = Gpio::F13;
    constexpr float TEST_TORQUE_REDUCTION_TIME = 239.17;
    constexpr float IMMEDIATELY = 0.0f;

    class ShiftTorqueReductionTimeConditionTest : public ShiftTorqueReductionTestBase {
    protected:
        void waitAndCheckTimeCondition(
            float timeoutInMs,
            bool expectedTriggerPinState,
            bool expectedTimeCondition,
            const char* context
        );
    };

    void ShiftTorqueReductionTimeConditionTest::waitAndCheckTimeCondition(
        const float timeoutInMs,
        const bool expectedTriggerPinState,
        const bool expectedTimeCondition,
        const char* const context
    ) {
        if (timeoutInMs > IMMEDIATELY) {
            advanceTimeUs(MS2US(timeoutInMs));
        }
        periodicFastCallback();
        EXPECT_EQ(
            engine->shiftTorqueReductionController.torqueReductionTriggerPinState,
            expectedTriggerPinState
        ) << context;
        EXPECT_EQ(
            engine->shiftTorqueReductionController.isTimeConditionSatisfied,
            expectedTimeCondition
        ) << context;
    }

    TEST_F(ShiftTorqueReductionTimeConditionTest, checkExpiration) {
        setUpTestConfig(ShiftTorqueReductionTestConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
            .setTriggerPin(TEST_TORQUE_REDUCTION_BUTTON_PIN)
            .setTorqueReductionTime(TEST_TORQUE_REDUCTION_TIME)
        );

        waitAndCheckTimeCondition(IMMEDIATELY, false, false, "Initial state");

        setMockState(TEST_TORQUE_REDUCTION_BUTTON_PIN, true);
        waitAndCheckTimeCondition(IMMEDIATELY, true, true, "Pin has just been actvated");

        waitAndCheckTimeCondition(TEST_TORQUE_REDUCTION_TIME, true, true, "Before timeout exriration");

        waitAndCheckTimeCondition(EPS3D, true, false, "After timeout expiration");
    }

    TEST_F(ShiftTorqueReductionTimeConditionTest, checkDeactivation) {
        setUpTestConfig(ShiftTorqueReductionTestConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
            .setTriggerPin(TEST_TORQUE_REDUCTION_BUTTON_PIN)
            .setTorqueReductionTime(TEST_TORQUE_REDUCTION_TIME)
        );

        setMockState(TEST_TORQUE_REDUCTION_BUTTON_PIN, true);
        waitAndCheckTimeCondition(IMMEDIATELY, true, true, "Pin has just been actvated");

        waitAndCheckTimeCondition(TEST_TORQUE_REDUCTION_TIME / 2, true, true, "Before pin deactivation");

        setMockState(TEST_TORQUE_REDUCTION_BUTTON_PIN, false);
        waitAndCheckTimeCondition(IMMEDIATELY, false, false, "Pin has just been deactvated");
    }

    TEST_F(ShiftTorqueReductionTimeConditionTest, checkReactivation) {
        setUpTestConfig(ShiftTorqueReductionTestConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
            .setTriggerPin(TEST_TORQUE_REDUCTION_BUTTON_PIN)
            .setTorqueReductionTime(TEST_TORQUE_REDUCTION_TIME)
        );

        setMockState(TEST_TORQUE_REDUCTION_BUTTON_PIN, true);
        waitAndCheckTimeCondition(IMMEDIATELY, true, true, "Pin has just been actvated");

        waitAndCheckTimeCondition(TEST_TORQUE_REDUCTION_TIME / 3, true, true, "Before pin deactivation");

        setMockState(TEST_TORQUE_REDUCTION_BUTTON_PIN, false);
        waitAndCheckTimeCondition(IMMEDIATELY, false, false, "Pin has just been deactvated");

        waitAndCheckTimeCondition(TEST_TORQUE_REDUCTION_TIME / 3, false, false, "Pin is still deactvated");
        setMockState(TEST_TORQUE_REDUCTION_BUTTON_PIN, true);
        waitAndCheckTimeCondition(IMMEDIATELY, true, true, "Pin has just been reactivated");

        waitAndCheckTimeCondition(TEST_TORQUE_REDUCTION_TIME, true, true, "Before timeout expiration");
        waitAndCheckTimeCondition(EPS3D, true, false, "After timeout expiration");
    }

    TEST_F(ShiftTorqueReductionTimeConditionTest, checkTimeConditionIsNeverSatisfiedWithZeroTorqueReductionTime) {
        setUpTestConfig(ShiftTorqueReductionTestConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
            .setTriggerPin(TEST_TORQUE_REDUCTION_BUTTON_PIN)
            .setTorqueReductionTime(IMMEDIATELY)
        );

        setMockState(TEST_TORQUE_REDUCTION_BUTTON_PIN, true);
        waitAndCheckTimeCondition(IMMEDIATELY, true, false, "Pin has just been actvated");

        waitAndCheckTimeCondition(TEST_TORQUE_REDUCTION_TIME / 3, true, false, "Before pin deactivation");

        setMockState(TEST_TORQUE_REDUCTION_BUTTON_PIN, false);
        waitAndCheckTimeCondition(IMMEDIATELY, false, false, "Pin has just been deactvated");

        waitAndCheckTimeCondition(TEST_TORQUE_REDUCTION_TIME / 3, false, false, "Pin is still deactvated");
        setMockState(TEST_TORQUE_REDUCTION_BUTTON_PIN, true);
        waitAndCheckTimeCondition(IMMEDIATELY, true, false, "Pin has just been reactivated");

        waitAndCheckTimeCondition(TEST_TORQUE_REDUCTION_TIME, true, false, "Before timeout expiration");
        waitAndCheckTimeCondition(EPS3D, true, false, "After timeout expiration");
    }
}