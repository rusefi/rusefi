/*
 * @file test_rotational_idle.cpp
 *
 * @date: ene 18, 2026
 * @author FDSoftware
 */

#include "pch.h"
#include "rotational_idle.h"

class RotationalIdleTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		EngineTestHelper eth(engine_type_e::TEST_ENGINE);

		// Reset configuration
		engineConfiguration->rotationalIdleController.enabled = false;
		engineConfiguration->rotationalIdleController.auto_engage = false;
		engineConfiguration->rotationalIdleController.auto_engage_clt_enable = false;
		engineConfiguration->rotationalIdleController.max_tps = 5;
		engineConfiguration->rotationalIdleController.auto_engage_clt = 80;
	}
};

TEST_F(RotationalIdleTest, disabledFeature)
{
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	RotationalIdle rotIdle;

	// Feature disabled
	engineConfiguration->rotationalIdleController.enabled = false;

	EXPECT_FALSE(rotIdle.shouldEngageRotationalIdle());
}

TEST_F(RotationalIdleTest, autoEngageByClt)
{
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	RotationalIdle rotIdle;

	engineConfiguration->rotationalIdleController.enabled = true;
	engineConfiguration->rotationalIdleController.auto_engage_clt_enable = true;
	engineConfiguration->rotationalIdleController.auto_engage_clt = 80;

	// CLT below threshold - should not engage
	Sensor::setMockValue(SensorType::Clt, 70);
	EXPECT_FALSE(rotIdle.shouldEngageRotationalIdle());

	// CLT above threshold - should engage
	Sensor::setMockValue(SensorType::Clt, 85);
	EXPECT_TRUE(rotIdle.shouldEngageRotationalIdle());
}

TEST_F(RotationalIdleTest, autoEngageByTps)
{
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	RotationalIdle rotIdle;

	engineConfiguration->rotationalIdleController.enabled = true;
	engineConfiguration->rotationalIdleController.auto_engage = true;
	engineConfiguration->rotationalIdleController.max_tps = 5;

	// TPS above threshold - should not engage
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 10);
	EXPECT_FALSE(rotIdle.shouldEngageRotationalIdle());

	// TPS below threshold - should engage
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 3);
	EXPECT_TRUE(rotIdle.shouldEngageRotationalIdle());

	// TPS at threshold - should engage
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 5);
	EXPECT_TRUE(rotIdle.shouldEngageRotationalIdle());
}

TEST_F(RotationalIdleTest, shouldSkipSpark)
{
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	RotationalIdle rotIdle;

	// Should not skip when feature is disabled
	engineConfiguration->rotationalIdleController.enabled = false;
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle());

	// Enable rotational idle
	engineConfiguration->rotationalIdleController.enabled = true;
	engineConfiguration->rotationalIdleController.auto_engage = true;
	engineConfiguration->rotationalIdleController.max_tps = 5;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 3);

	// Should not skip when acc_max is 0 (disabled)
	engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Spark;
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle());

	// Test cut_mode = 1 (Fuel only) - should never skip spark
	engineConfiguration->rotationalIdleController.accumulators[0].acc_max = 2;
	engineConfiguration->rotationalIdleController.accumulators[0].acc_adder = 1;
	engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Fuel;
	engine->engineState.globalSparkCounter = 1;
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // Fuel mode never skips spark

	// Test cut_mode = 0 (Spark mode) - should skip spark based on pattern
	engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Spark;
	EXPECT_TRUE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=1: (1 % 2) + 1 = 2 >= 2

	// Test cut_mode = 2 (Both mode) - should skip spark based on pattern
	engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Both;
	EXPECT_TRUE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=1: (1 % 2) + 1 = 2 >= 2

	// Configure skip pattern: test with various counter values (cut_mode = 0)
	engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Spark;
	engineConfiguration->rotationalIdleController.accumulators[0].acc_max = 2;
	engineConfiguration->rotationalIdleController.accumulators[0].acc_adder = 0;
	engineConfiguration->rotationalIdleController.accumulators[0].acc_offset = 0;

	// Set global spark counter to test pattern
	engine->engineState.globalSparkCounter = 0;
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=0: (0 % 2) + 0 = 0 < 2

	engine->engineState.globalSparkCounter = 1;
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=1: (1 % 2) + 0 = 1 < 2

	engine->engineState.globalSparkCounter = 2;
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=2: (2 % 2) + 0 = 0 < 2

	// Test with adder to create skip pattern
	engineConfiguration->rotationalIdleController.accumulators[0].acc_adder = 1;

	engine->engineState.globalSparkCounter = 0;
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=0: (0 % 2) + 1 = 1 < 2

	engine->engineState.globalSparkCounter = 1;
	EXPECT_TRUE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=1: (1 % 2) + 1 = 2 >= 2

	engine->engineState.globalSparkCounter = 2;
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=2: (2 % 2) + 1 = 1 < 2
	
}

TEST_F(RotationalIdleTest, multipleAccumulators)
{
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	RotationalIdle rotIdle;

	engineConfiguration->rotationalIdleController.enabled = true;
	engineConfiguration->rotationalIdleController.auto_engage = true;
	engineConfiguration->rotationalIdleController.max_tps = 5;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 3);
	engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Spark;

	// Accumulator 0: skip every 2nd (1, 3, 5...)
	engineConfiguration->rotationalIdleController.accumulators[0].acc_max = 2;
	engineConfiguration->rotationalIdleController.accumulators[0].acc_adder = 1;
	engineConfiguration->rotationalIdleController.accumulators[0].acc_offset = 0;

	// Accumulator 1: skip every 3rd (2, 5, 8...)
	engineConfiguration->rotationalIdleController.accumulators[1].acc_max = 3;
	engineConfiguration->rotationalIdleController.accumulators[1].acc_adder = 1;
	engineConfiguration->rotationalIdleController.accumulators[1].acc_offset = 0;

	// globalSparkCounter = 0
	// Acc 0: (0%2)+1 = 1 < 2 (Fire)
	// Acc 1: (0%3)+1 = 1 < 3 (Fire)
	engine->engineState.globalSparkCounter = 0;
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle());

	// globalSparkCounter = 1
	// Acc 0: (1%2)+1 = 2 >= 2 (Skip)
	// Acc 1: (1%3)+1 = 2 < 3 (Fire)
	// Result: Skip
	engine->engineState.globalSparkCounter = 1;
	EXPECT_TRUE(rotIdle.shouldSkipSparkRotationalIdle());

	// globalSparkCounter = 2
	// Acc 0: (2%2)+1 = 1 < 2 (Fire)
	// Acc 1: (2%3)+1 = 3 >= 3 (Skip)
	// Result: Skip
	engine->engineState.globalSparkCounter = 2;
	EXPECT_TRUE(rotIdle.shouldSkipSparkRotationalIdle());

	// globalSparkCounter = 3
	// Acc 0: (3%2)+1 = 2 >= 2 (Skip)
	// Acc 1: (3%3)+1 = 1 < 3 (Fire)
	// Result: Skip
	engine->engineState.globalSparkCounter = 3;
	EXPECT_TRUE(rotIdle.shouldSkipSparkRotationalIdle());

	// globalSparkCounter = 4
	// Acc 0: (4%2)+1 = 1 < 2 (Fire)
	// Acc 1: (4%3)+1 = 2 < 3 (Fire)
	// Result: Fire
	engine->engineState.globalSparkCounter = 4;
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle());
}
