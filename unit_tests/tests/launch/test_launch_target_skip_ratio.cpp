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

TEST(TractionSparkSkip, OnePercentHardCutSkipsEverySpark) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->useHardSkipInTraction = true;
	setTable(engineConfiguration->tractionControlIgnitionSkip, 1);
	Sensor::setMockValue(SensorType::VehicleSpeed, 50);
	Sensor::setMockValue(SensorType::WheelSlipRatio, 5);
	engine->rpmCalculator.setRpmValue(2000);
	engine->periodicFastCallback();
	ASSERT_FLOAT_EQ(1, engine->engineState.tractionControlSparkSkip);
	EXPECT_FLOAT_EQ(1, engine->hardSparkLimiter.getTargetSkipRatio());
	int skipped = 0;
	for (int i = 0; i < 100; i++) {
		skipped += engine->hardSparkLimiter.shouldSkip();
	}
	EXPECT_EQ(100, skipped); // The configured value is 1%, not a ratio of 1.
}

TEST(TractionSparkSkip, OnePercentSoftCutSkipsHalfTheSparks) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->useHardSkipInTraction = false;
	setTable(engineConfiguration->tractionControlIgnitionSkip, 1);
	Sensor::setMockValue(SensorType::VehicleSpeed, 50);
	Sensor::setMockValue(SensorType::WheelSlipRatio, 5);
	engine->rpmCalculator.setRpmValue(2000);
	engine->periodicFastCallback();
	EXPECT_FLOAT_EQ(1, engine->softSparkLimiter.getTargetSkipRatio());
	int skipped = 0;
	for (int i = 0; i < 100; i++) {
		skipped += engine->softSparkLimiter.shouldSkip();
	}
	EXPECT_EQ(50, skipped); // The consecutive-skip guard limits the erroneous cut.
}
