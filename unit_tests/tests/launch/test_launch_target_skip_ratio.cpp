//
// Created by kifir on 5/17/24.
//

#include "pch.h"

#include "util/test_base.h"

namespace {
    constexpr float TEST_TRACTION_CONTROL_IGNITION_SKIP = 17.0f;
    constexpr float TEST_LUA_SOFT_SPARK_SKIP = 239.0f;
    constexpr float TEST_LUA_HARD_SPARK_SKIP = 174.0f;

    class LaunchTargetSkipRatioTest : public TestBase<> {
    protected:
        void SetUp() override;
    };

    void LaunchTargetSkipRatioTest::SetUp() {
        TestBase::SetUp();
        std::fill_n(
            &engineConfiguration->tractionControlIgnitionSkip[0][0],
            TRACTION_CONTROL_ETB_DROP_SLIP_SIZE * TRACTION_CONTROL_ETB_DROP_SPEED_SIZE,
            static_cast<int8_t>(TEST_TRACTION_CONTROL_IGNITION_SKIP)
        );
        getTestLuaScriptExecutor().setSparkSkipRatio(TEST_LUA_SOFT_SPARK_SKIP);
        getTestLuaScriptExecutor().setSparkHardSkipRatio(TEST_LUA_HARD_SPARK_SKIP);
    }

    TEST_F(LaunchTargetSkipRatioTest, doNotUseSkipInTraction) {
        EXPECT_FALSE(engineConfiguration->useHardSkipInTraction);

        periodicFastCallback();
        EXPECT_EQ(engine->softSparkLimiter.getTargetSkipRatio(), TEST_LUA_SOFT_SPARK_SKIP + TEST_TRACTION_CONTROL_IGNITION_SKIP);
        EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), TEST_LUA_HARD_SPARK_SKIP);
    }

    TEST_F(LaunchTargetSkipRatioTest, useSkipInTraction) {
        engineConfiguration->useHardSkipInTraction = true;

        periodicFastCallback();
        EXPECT_EQ(engine->softSparkLimiter.getTargetSkipRatio(), TEST_LUA_SOFT_SPARK_SKIP);
        EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), TEST_LUA_HARD_SPARK_SKIP + TEST_TRACTION_CONTROL_IGNITION_SKIP);
    }
}
