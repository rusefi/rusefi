//
// Created by kifir on 5/17/24.
//

#include "pch.h"

constexpr float TEST_TRACTION_CONTROL_IGNITION_SKIP = 17.0f;
constexpr float TEST_LUA_SOFT_SPARK_SKIP = 239.0f;
constexpr float TEST_LUA_HARD_SPARK_SKIP = 174.0f;

static void setUpTestParameters() {
    std::fill_n(
        &engineConfiguration->tractionControlIgnitionSkip[0][0],
        TRACTION_CONTROL_ETB_DROP_SIZE * TRACTION_CONTROL_ETB_DROP_SIZE,
        static_cast<int8_t>(TEST_TRACTION_CONTROL_IGNITION_SKIP)
    );
    engine->engineState.luaSoftSparkSkip = TEST_LUA_SOFT_SPARK_SKIP;
    engine->engineState.luaHardSparkSkip = TEST_LUA_HARD_SPARK_SKIP;
}

TEST(targetSkipRatio, doNotUseSkipInTraction) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    setUpTestParameters();
    EXPECT_FALSE(engineConfiguration->useHardSkipInTraction);

    eth.engine.periodicFastCallback();
    EXPECT_EQ(engine->softSparkLimiter.getTargetSkipRatio(), TEST_LUA_SOFT_SPARK_SKIP + TEST_TRACTION_CONTROL_IGNITION_SKIP);
    EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), TEST_LUA_HARD_SPARK_SKIP);
}

TEST(targetSkipRatio, useSkipInTraction) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);

    setUpTestParameters();
    engineConfiguration->useHardSkipInTraction = true;

    eth.engine.periodicFastCallback();
    EXPECT_EQ(engine->softSparkLimiter.getTargetSkipRatio(), TEST_LUA_SOFT_SPARK_SKIP);
    EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), TEST_LUA_HARD_SPARK_SKIP + TEST_TRACTION_CONTROL_IGNITION_SKIP);
}
