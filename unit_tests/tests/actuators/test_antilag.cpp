#include "pch.h"

TEST(Actuators, AntiLag) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->antiLagEnabled = true;

	Sensor::setMockValue(SensorType::Clt, 70);

	engine->periodicFastCallback();
	ASSERT_EQ(1, engine->antilagController.ALSMinCLTCondition);
	ASSERT_EQ(1, engine->antilagController.ALSMaxCLTCondition);

	// todo: let's have this 'true'
	ASSERT_EQ(0, engine->antilagController.isAntilagCondition);
}
