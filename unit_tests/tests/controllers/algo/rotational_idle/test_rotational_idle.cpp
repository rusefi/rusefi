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

	// Should not skip when feature is disabled
	{
		RotationalIdle rotIdle;
		engineConfiguration->rotationalIdleController.enabled = false;
		EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle());
	}

	engineConfiguration->rotationalIdleController.enabled = true;
	engineConfiguration->rotationalIdleController.auto_engage = true;
	engineConfiguration->rotationalIdleController.max_tps = 5;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 3);

	// Should not skip when acc_max is 0 (disabled) - fresh instance, counter starts at 1
	{
		RotationalIdle rotIdle;
		engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Spark;
		EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // acc_max=0, always fire
	}

	// cut_mode = Fuel: should never skip spark regardless of pattern
	{
		RotationalIdle rotIdle;
		engineConfiguration->rotationalIdleController.accumulators[0].acc_max = 2;
		engineConfiguration->rotationalIdleController.accumulators[0].acc_adder = 1;
		engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Fuel;
		EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // Fuel mode never skips spark
		EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle());
	}

	// cut_mode = Spark: counter advances on each call, pattern applies
	// sparkPatternCounter starts at 1; with acc_max=2, acc_adder=1:
	//   counter=1: (1%2)+1=2>=2 -> skip
	//   counter=2: (2%2)+1=1<2  -> fire
	{
		RotationalIdle rotIdle;
		engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Spark;
		engineConfiguration->rotationalIdleController.accumulators[0].acc_max = 2;
		engineConfiguration->rotationalIdleController.accumulators[0].acc_adder = 1;
		engineConfiguration->rotationalIdleController.accumulators[0].acc_offset = 0;
		EXPECT_TRUE(rotIdle.shouldSkipSparkRotationalIdle());  // counter=1: skip
		EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=2: fire
		EXPECT_TRUE(rotIdle.shouldSkipSparkRotationalIdle());  // counter=3: skip
		EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=4: fire
	}

	// cut_mode = Both: same spark skip behavior
	{
		RotationalIdle rotIdle;
		engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Both;
		EXPECT_TRUE(rotIdle.shouldSkipSparkRotationalIdle());  // counter=1: skip
		EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=2: fire
	}

	// adder=0: never skips (counter % max never reaches max when adder=0)
	{
		RotationalIdle rotIdle;
		engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Spark;
		engineConfiguration->rotationalIdleController.accumulators[0].acc_max = 2;
		engineConfiguration->rotationalIdleController.accumulators[0].acc_adder = 0;
		engineConfiguration->rotationalIdleController.accumulators[0].acc_offset = 0;
		EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=1: (1%2)+0=1<2
		EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=2: (2%2)+0=0<2
		EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle()); // counter=3: (3%2)+0=1<2
	}
}

TEST_F(RotationalIdleTest, diagnosticFlags)
{
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	RotationalIdle rotIdle;

	engineConfiguration->rotationalIdleController.enabled = true;

	// No engage method configured: all flags false
	EXPECT_FALSE(rotIdle.shouldEngageRotationalIdle());
	EXPECT_FALSE(rotIdle.rotIdleTpsTooHigh);
	EXPECT_FALSE(rotIdle.rotIdleEngineTooLowClt);

	// CLT method enabled but CLT is too cold
	engineConfiguration->rotationalIdleController.auto_engage_clt_enable = true;
	engineConfiguration->rotationalIdleController.auto_engage_clt = 80;
	Sensor::setMockValue(SensorType::Clt, 60);
	EXPECT_FALSE(rotIdle.shouldEngageRotationalIdle());
	EXPECT_TRUE(rotIdle.rotIdleEngineTooLowClt);
	EXPECT_FALSE(rotIdle.rotIdleTpsTooHigh);

	// CLT is warm enough - engaged, no flags
	Sensor::setMockValue(SensorType::Clt, 90);
	EXPECT_TRUE(rotIdle.shouldEngageRotationalIdle());
	EXPECT_FALSE(rotIdle.rotIdleEngineTooLowClt);
	EXPECT_FALSE(rotIdle.rotIdleTpsTooHigh);
	engineConfiguration->rotationalIdleController.auto_engage_clt_enable = false;

	// TPS method enabled but throttle is too high
	engineConfiguration->rotationalIdleController.auto_engage = true;
	engineConfiguration->rotationalIdleController.max_tps = 5;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 20);
	EXPECT_FALSE(rotIdle.shouldEngageRotationalIdle());
	EXPECT_FALSE(rotIdle.rotIdleEngineTooLowClt);
	EXPECT_TRUE(rotIdle.rotIdleTpsTooHigh);

	// TPS within threshold - engaged, no flags
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 3);
	EXPECT_TRUE(rotIdle.shouldEngageRotationalIdle());
	EXPECT_FALSE(rotIdle.rotIdleTpsTooHigh);
}

TEST_F(RotationalIdleTest, shouldSkipFuel)
{
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	RotationalIdle rotIdle;

	engineConfiguration->rotationalIdleController.enabled = true;
	engineConfiguration->rotationalIdleController.auto_engage = true;
	engineConfiguration->rotationalIdleController.max_tps = 5;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 3);

	// cut_mode = Spark: should never skip fuel
	engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Spark;
	engineConfiguration->rotationalIdleController.accumulators[0].acc_max = 2;
	engineConfiguration->rotationalIdleController.accumulators[0].acc_adder = 1;
	EXPECT_FALSE(rotIdle.shouldSkipFuelRotationalIdle());
	EXPECT_FALSE(rotIdle.shouldSkipFuelRotationalIdle());

	// cut_mode = Fuel: should skip based on accumulator pattern
	// Fresh RotationalIdle instance so fuelPatternCounter starts at 1
	RotationalIdle rotIdle2;
	engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Fuel;
	engineConfiguration->rotationalIdleController.accumulators[0].acc_max = 2;
	engineConfiguration->rotationalIdleController.accumulators[0].acc_adder = 1;
	engineConfiguration->rotationalIdleController.accumulators[0].acc_offset = 0;

	// counter=1: (1%2)+1=2>=2 → skip
	EXPECT_TRUE(rotIdle2.shouldSkipFuelRotationalIdle());
	// counter=2: (2%2)+1=1<2 → fire
	EXPECT_FALSE(rotIdle2.shouldSkipFuelRotationalIdle());
	// counter=3: (3%2)+1=2>=2 → skip
	EXPECT_TRUE(rotIdle2.shouldSkipFuelRotationalIdle());
	// counter=4: (4%2)+1=1<2 → fire
	EXPECT_FALSE(rotIdle2.shouldSkipFuelRotationalIdle());

	// cut_mode = Both: should also skip fuel
	RotationalIdle rotIdle3;
	engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Both;
	// counter=1: skip
	EXPECT_TRUE(rotIdle3.shouldSkipFuelRotationalIdle());
	// counter=2: fire
	EXPECT_FALSE(rotIdle3.shouldSkipFuelRotationalIdle());

	// Fuel skip disabled when feature is disabled
	RotationalIdle rotIdle4;
	engineConfiguration->rotationalIdleController.enabled = false;
	engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Fuel;
	EXPECT_FALSE(rotIdle4.shouldSkipFuelRotationalIdle());

	// Fuel counter advances even when not engaged (TPS too high), maintaining pattern consistency
	RotationalIdle rotIdle5;
	engineConfiguration->rotationalIdleController.enabled = true;
	engineConfiguration->rotationalIdleController.cut_mode = RotationalCutMode::Fuel;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50); // throttle open - not engaged
	EXPECT_FALSE(rotIdle5.shouldSkipFuelRotationalIdle()); // counter=1 consumed, not engaged
	EXPECT_FALSE(rotIdle5.shouldSkipFuelRotationalIdle()); // counter=2 consumed, not engaged
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 3); // back to idle
	// counter=3: (3%2)+1=2>=2 → engaged and skip
	EXPECT_TRUE(rotIdle5.shouldSkipFuelRotationalIdle());
	// counter=4: (4%2)+1=1<2 → engaged and fire
	EXPECT_FALSE(rotIdle5.shouldSkipFuelRotationalIdle());
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

	// sparkPatternCounter starts at 1 and auto-increments each call.

	// globalSparkCounter = 1
	// Acc 0: (1%2)+1 = 2 >= 2 (Skip)
	// Acc 1: (1%3)+1 = 2 < 3 (Fire)
	// Result: Skip
	EXPECT_TRUE(rotIdle.shouldSkipSparkRotationalIdle());

	// globalSparkCounter = 2
	// Acc 0: (2%2)+1 = 1 < 2 (Fire)
	// Acc 1: (2%3)+1 = 3 >= 3 (Skip)
	// Result: Skip
	EXPECT_TRUE(rotIdle.shouldSkipSparkRotationalIdle());

	// globalSparkCounter = 3
	// Acc 0: (3%2)+1 = 2 >= 2 (Skip)
	// Acc 1: (3%3)+1 = 1 < 3 (Fire)
	// Result: Skip
	EXPECT_TRUE(rotIdle.shouldSkipSparkRotationalIdle());

	// globalSparkCounter = 4
	// Acc 0: (4%2)+1 = 1 < 2 (Fire)
	// Acc 1: (4%3)+1 = 2 < 3 (Fire)
	// Result: Fire
	engine->engineState.globalSparkCounter = 4;
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle());

	// counter=5: Acc0: (5%2)+1=2>=2 (Skip), Acc1: (5%3)+1=3>=3 (Skip) -> Skip
	EXPECT_TRUE(rotIdle.shouldSkipSparkRotationalIdle());
}
