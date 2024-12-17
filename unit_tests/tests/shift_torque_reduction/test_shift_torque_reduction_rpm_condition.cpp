//
// Created by kifir on 10/3/24.
//

#include "pch.h"

#include "util/test_base.h"

namespace {
    constexpr float TEST_TORQUE_REDUCTION_ARMING_RPM = 239.0f;

    class ShiftTorqueReductionRpmConditionTest : public TestBase<> {
    protected:
        void checkRpmCondition(float rpm, bool expectedRpmCondition, const char* context);
    };

    void ShiftTorqueReductionRpmConditionTest::checkRpmCondition(
        const float rpm,
        const bool expectedRpmCondition,
        const char* const context
    ) {
        updateRpm(rpm);
        EXPECT_EQ(engine->shiftTorqueReductionController.isRpmConditionSatisfied, expectedRpmCondition) << context;
    }

    TEST_F(ShiftTorqueReductionRpmConditionTest, checkZeroArmingRpm) {
        setUpEngineConfiguration(EngineConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionArmingRpm(0.0f)
        );
        checkRpmCondition(0.0f, true, "Zero RPM");
        checkRpmCondition(TEST_TORQUE_REDUCTION_ARMING_RPM, true, "Non-zero RPM");
    }

    TEST_F(ShiftTorqueReductionRpmConditionTest, checkArmingRpm) {
        setUpEngineConfiguration(EngineConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionArmingRpm(TEST_TORQUE_REDUCTION_ARMING_RPM)
        );
        checkRpmCondition(0.0f, false, "Zero RPM");
        checkRpmCondition(TEST_TORQUE_REDUCTION_ARMING_RPM - EPS5D, false, "Below arming RPM");
        checkRpmCondition(TEST_TORQUE_REDUCTION_ARMING_RPM, true, "Exact arming RPM");
        checkRpmCondition(TEST_TORQUE_REDUCTION_ARMING_RPM + EPS5D, true, "Above arming RPM");
    }
}