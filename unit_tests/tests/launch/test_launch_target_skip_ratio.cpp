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
        EXPECT_EQ(engine->softSparkLimiter.getTargetSkipRatio(), TEST_LUA_SOFT_SPARK_SKIP + TEST_TRACTION_CONTROL_IGNITION_SKIP / 100.0f);
        EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), TEST_LUA_HARD_SPARK_SKIP);
    }

    TEST_F(LaunchTargetSkipRatioTest, useSkipInTraction) {
        engineConfiguration->useHardSkipInTraction = true;

        periodicFastCallback();
        EXPECT_EQ(engine->softSparkLimiter.getTargetSkipRatio(), TEST_LUA_SOFT_SPARK_SKIP);
        EXPECT_EQ(engine->hardSparkLimiter.getTargetSkipRatio(), TEST_LUA_HARD_SPARK_SKIP + TEST_TRACTION_CONTROL_IGNITION_SKIP / 100.0f);
    }
}

TEST(TractionSparkSkip, OnePercentHardCutSkipsAboutOnePercent) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->useHardSkipInTraction = true;
	setTable(engineConfiguration->tractionControlIgnitionSkip, 1);
	Sensor::setMockValue(SensorType::VehicleSpeed, 50);
	Sensor::setMockValue(SensorType::WheelSlipRatio, 5);
	engine->rpmCalculator.setRpmValue(2000);
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(1, engine->engineState.tractionControlSparkSkip);
	EXPECT_FLOAT_EQ(0.01f, engine->hardSparkLimiter.getTargetSkipRatio());
	int skipped = 0;
	for (int i = 0; i < 100; i++) {
		skipped += engine->hardSparkLimiter.shouldSkip();
	}
	EXPECT_LT(skipped, 10);
}

TEST(TractionSparkSkip, OnePercentSoftCutSkipsAboutOnePercent) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->useHardSkipInTraction = false;
	setTable(engineConfiguration->tractionControlIgnitionSkip, 1);
	Sensor::setMockValue(SensorType::VehicleSpeed, 50);
	Sensor::setMockValue(SensorType::WheelSlipRatio, 5);
	engine->rpmCalculator.setRpmValue(2000);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ(0.01f, engine->softSparkLimiter.getTargetSkipRatio());
	int skipped = 0;
	for (int i = 0; i < 100; i++) {
		skipped += engine->softSparkLimiter.shouldSkip();
	}
	EXPECT_LT(skipped, 10);
}

TEST(TractionSparkSkip, TenPercentHardCutUsesNormalizedRatio) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->useHardSkipInTraction = true;
	setTable(engineConfiguration->tractionControlIgnitionSkip, 10);
	Sensor::setMockValue(SensorType::VehicleSpeed, 50);
	Sensor::setMockValue(SensorType::WheelSlipRatio, 5);
	engine->rpmCalculator.setRpmValue(2000);
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(10, engine->engineState.tractionControlSparkSkip);
	EXPECT_FLOAT_EQ(0.1f, engine->hardSparkLimiter.getTargetSkipRatio());
	EXPECT_FLOAT_EQ(0, engine->softSparkLimiter.getTargetSkipRatio());
	int skipped = 0;
	for (int i = 0; i < 10000; i++) {
		skipped += engine->hardSparkLimiter.shouldSkip();
	}
	EXPECT_GT(skipped, 800);
	EXPECT_LT(skipped, 1200);
}

TEST(TractionSparkSkip, TenPercentSoftCutUsesNormalizedRatio) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->useHardSkipInTraction = false;
	setTable(engineConfiguration->tractionControlIgnitionSkip, 10);
	Sensor::setMockValue(SensorType::VehicleSpeed, 50);
	Sensor::setMockValue(SensorType::WheelSlipRatio, 5);
	engine->rpmCalculator.setRpmValue(2000);
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(10, engine->engineState.tractionControlSparkSkip);
	EXPECT_FLOAT_EQ(0.1f, engine->softSparkLimiter.getTargetSkipRatio());
	EXPECT_FLOAT_EQ(0, engine->hardSparkLimiter.getTargetSkipRatio());
	int skipped = 0;
	bool previousSkipped = false;
	for (int i = 0; i < 10000; i++) {
		const bool skip = engine->softSparkLimiter.shouldSkip();
		EXPECT_FALSE(skip && previousSkipped);
		skipped += skip;
		previousSkipped = skip;
	}
	// Soft limiting doubles the draw probability and prevents adjacent skips.
	EXPECT_GT(skipped, 1400);
	EXPECT_LT(skipped, 1900);
}
