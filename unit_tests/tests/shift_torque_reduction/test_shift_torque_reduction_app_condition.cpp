//
// Created by kifir on 10/3/24.
//

#include "pch.h"

#include "util/test_base.h"

namespace {
    constexpr float TEST_TORQUE_REDUCTION_ARMING_APP = 17.0f;

    class ShiftTorqueReductionAppConditionTest : public TestBase<> {
    protected:
        void checkAppCondition(std::optional<float> rpm, bool expectedAppCondition, const char* context);
    };

    void ShiftTorqueReductionAppConditionTest::checkAppCondition(
        const std::optional<float> app,
        const bool expectedAppCondition,
        const char* const context
    ) {
        updateApp(app);
        EXPECT_EQ(engine->shiftTorqueReductionController.isAppConditionSatisfied, expectedAppCondition) << context;
    }

    TEST_F(ShiftTorqueReductionAppConditionTest, checkZeroArmingApp) {
        setUpEngineConfiguration(EngineConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionArmingApp(0.0f)
        );
        checkAppCondition(0.0f, true, "Zero APP");
        checkAppCondition(TEST_TORQUE_REDUCTION_ARMING_APP, true, "Non-zero APP");
        checkAppCondition({}, false, "Missed APP");
    }

    TEST_F(ShiftTorqueReductionAppConditionTest, checkArmingApp) {
        setUpEngineConfiguration(EngineConfig()
            .setTorqueReductionEnabled(true)
            .setTorqueReductionArmingApp(TEST_TORQUE_REDUCTION_ARMING_APP)
        );

        periodicFastCallback();
        EXPECT_FALSE(engine->shiftTorqueReductionController.isAppConditionSatisfied) << "Default";

        checkAppCondition(0.0f, false, "Zero APP");
        checkAppCondition(TEST_TORQUE_REDUCTION_ARMING_APP - EPS5D, false, "Below arming APP");
        checkAppCondition(TEST_TORQUE_REDUCTION_ARMING_APP, true, "Exact arming APP");
        checkAppCondition(TEST_TORQUE_REDUCTION_ARMING_APP + EPS5D, true, "Above arming APP");
        checkAppCondition({}, false, "Missed APP");
    }
}