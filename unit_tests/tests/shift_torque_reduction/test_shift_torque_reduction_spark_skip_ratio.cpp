//
// Created by kifir on 11/4/24.
//

#include "pch.h"

#include "flat_shift_condition_test_base.h"

namespace {
    constexpr float TEST_LUA_SOFT_SPARK_SKIP = 1.23f;
    constexpr float TEST_LUA_HARD_SPARK_SKIP = 4.56f;

    constexpr int8_t TEST_TORQUE_REDUCTION_IGNITION_CUT = 17;

    class ShiftTorqueReductionSparkSkipRatioTest : public FlatShiftConditionTestBase {
    protected:
        ShiftTorqueReductionSparkSkipRatioTest();

        void SetUp() override;

        void checkTargetSkipRatios(
            const char* context,
            float expectedHardSparkLimiterTargetSkipRatio = TEST_LUA_HARD_SPARK_SKIP
        );
    };

    ShiftTorqueReductionSparkSkipRatioTest::ShiftTorqueReductionSparkSkipRatioTest()
    : FlatShiftConditionTestBase(TEST_TORQUE_REDUCTION_IGNITION_CUT, 0.0f) {
    }

    void ShiftTorqueReductionSparkSkipRatioTest::SetUp() {
        FlatShiftConditionTestBase::SetUp();

        getTestLuaScriptExecutor().setSparkSkipRatio(TEST_LUA_SOFT_SPARK_SKIP);
        getTestLuaScriptExecutor().setSparkHardSkipRatio(TEST_LUA_HARD_SPARK_SKIP);
    }

    void ShiftTorqueReductionSparkSkipRatioTest::checkTargetSkipRatios(
        const char* const context,
        const float expectedHardSparkLimiterTargetSkipRatio
    ) {
        periodicFastCallback();
        EXPECT_EQ(engine->softSparkLimiter.getTargetSkipRatio(), TEST_LUA_SOFT_SPARK_SKIP) << context;
        EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), expectedHardSparkLimiterTargetSkipRatio) << context;
    }

    TEST_F(ShiftTorqueReductionSparkSkipRatioTest, checkHardSparkLimiterTargetSkipRatio) {
        checkTargetSkipRatios("Initial state");

        satisfyFlatShiftCondition();
        checkTargetSkipRatios(
            "Flat Shift condition is satisfied",
            TEST_LUA_HARD_SPARK_SKIP + TEST_TORQUE_REDUCTION_IGNITION_CUT / 100.0f
        );

        unsatisfyFlatShiftCondition();
        checkTargetSkipRatios("Flat Shift condition is not satisfied");
    }
}