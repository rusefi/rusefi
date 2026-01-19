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

	// Test with various offsets
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle(0));
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle(1));
	EXPECT_FALSE(rotIdle.shouldSkipSparkRotationalIdle(-1));
}
