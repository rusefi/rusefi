//
// Created by kifir on 10/4/24.
//

#include "pch.h"

#include "util/test_base.h"

namespace {
    class ShiftTorqueReductionFlatShiftConditionTest : public TestBase<> {
    protected:
        static constexpr switch_input_pin_e TEST_TORQUE_REDUCTION_BUTTON_PIN = Gpio::E13;
        static constexpr float TEST_TORQUE_REDUCTION_TIME = 123.45f;
        static constexpr float TEST_TORQUE_REDUCTION_ARMING_RPM = 678.90f;
        static constexpr float TEST_TORQUE_REDUCTION_ARMING_APP = 12.3f;
        static constexpr float IMMEDIATELY = 0.0f;

        void SetUp() override;

        void triggerPin();
        void satisfyRpmCondition();
        void satisfyAppCondition();

        void waitAndCheckFlatShiftCondition(float timeoutInMs, bool expectedFlatShiftCondition, const char* context);
    };

    void ShiftTorqueReductionFlatShiftConditionTest::SetUp() {
        TestBase::SetUp();
        setUpEngineConfiguration(EngineConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionActivationMode(torqueReductionActivationMode_e::TORQUE_REDUCTION_BUTTON)
            .setTorqueReductionTriggerPin(TEST_TORQUE_REDUCTION_BUTTON_PIN)
            .setLimitTorqueReductionTime(true)
            .setTorqueReductionTime(TEST_TORQUE_REDUCTION_TIME)
            .setTorqueReductionArmingRpm(TEST_TORQUE_REDUCTION_ARMING_RPM)
            .setTorqueReductionArmingApp(TEST_TORQUE_REDUCTION_ARMING_APP)
        );
    }

    void ShiftTorqueReductionFlatShiftConditionTest::triggerPin() {
        setMockState(TEST_TORQUE_REDUCTION_BUTTON_PIN, true);
        periodicFastCallback();
        EXPECT_TRUE(engine->shiftTorqueReductionController.isTorqueReductionTriggerPinValid);
        EXPECT_TRUE(engine->shiftTorqueReductionController.torqueReductionTriggerPinState);
        EXPECT_TRUE(engine->shiftTorqueReductionController.isTimeConditionSatisfied);
    }

    void ShiftTorqueReductionFlatShiftConditionTest::satisfyRpmCondition() {
        updateRpm(TEST_TORQUE_REDUCTION_ARMING_RPM);
        EXPECT_TRUE(engine->shiftTorqueReductionController.isRpmConditionSatisfied);
    }

    void ShiftTorqueReductionFlatShiftConditionTest::satisfyAppCondition() {
        updateApp(TEST_TORQUE_REDUCTION_ARMING_APP);
        EXPECT_TRUE(engine->shiftTorqueReductionController.isAppConditionSatisfied);
    }

    void ShiftTorqueReductionFlatShiftConditionTest::waitAndCheckFlatShiftCondition(
        const float timeoutInMs,
        const bool expectedFlatShiftCondition,
        const char* const context
    ) {
        advanceTimeUs(MS2US(timeoutInMs));
        periodicFastCallback();
        EXPECT_EQ(engine->shiftTorqueReductionController.isFlatShiftConditionSatisfied, expectedFlatShiftCondition)
            << context;
    }

    TEST_F(ShiftTorqueReductionFlatShiftConditionTest, checkSatisfiedFlatShiftCondition) {
        triggerPin();
        satisfyRpmCondition();
        satisfyAppCondition();
        waitAndCheckFlatShiftCondition(IMMEDIATELY, true, "App condition is satisfied");
    }

    TEST_F(ShiftTorqueReductionFlatShiftConditionTest, checkWithoutTriggeredPin) {
        //triggerPin();
        satisfyRpmCondition();
        satisfyAppCondition();
        waitAndCheckFlatShiftCondition(IMMEDIATELY, false, "Without triggered pin");
    }

    TEST_F(ShiftTorqueReductionFlatShiftConditionTest, checkWithoutRpmCondition) {
        triggerPin();
        //satisfyRpmCondition();
        satisfyAppCondition();
        waitAndCheckFlatShiftCondition(IMMEDIATELY, false, "Without rpm condition");
    }

    TEST_F(ShiftTorqueReductionFlatShiftConditionTest, checkWithoutAppCondition) {
        triggerPin();
        satisfyRpmCondition();
        //satisfyAppCondition();
        waitAndCheckFlatShiftCondition(IMMEDIATELY, false, "Without app condition");
    }

    TEST_F(ShiftTorqueReductionFlatShiftConditionTest, checkSatisfiedFlatShiftConditionExpiration) {
        waitAndCheckFlatShiftCondition(IMMEDIATELY, false, "Default");

        triggerPin();
        waitAndCheckFlatShiftCondition(IMMEDIATELY, false, "Time condition is satisfied");

        satisfyRpmCondition();
        waitAndCheckFlatShiftCondition(IMMEDIATELY, false, "Rpm condition is satisfied");

        satisfyAppCondition();
        waitAndCheckFlatShiftCondition(IMMEDIATELY, true, "App condition is satisfied");

        waitAndCheckFlatShiftCondition(TEST_TORQUE_REDUCTION_TIME, true, "Flat shift condition is still satisfied");

        waitAndCheckFlatShiftCondition(EPS3D, false, "Flat shift condition is expired");
    }
}