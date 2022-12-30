#include "pch.h"

TEST(Actuators, AntiLag) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->antiLagEnabled = true;

	Sensor::setMockValue(SensorType::Clt, 70);

	engineConfiguration->antiLagActivationMode = SWITCH_INPUT_ANTILAG;
	engine->periodicFastCallback();
	// in unit tests we pretend that physical swiych is always OFF
	ASSERT_EQ(0, engine->antilagController.ALSSwitchCondition);

	engineConfiguration->antiLagActivationMode = ALWAYS_ON_ANTILAG;
	engine->periodicFastCallback();
	ASSERT_EQ(1, engine->antilagController.ALSSwitchCondition);

	ASSERT_EQ(1, engine->antilagController.ALSMinCLTCondition);
	ASSERT_EQ(1, engine->antilagController.ALSMaxCLTCondition);

	// todo: let's have this 'true'
	ASSERT_EQ(0, engine->antilagController.isAntilagCondition);
}
