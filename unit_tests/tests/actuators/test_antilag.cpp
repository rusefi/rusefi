#include "pch.h"
#include "fuel_math.h"

TEST(Actuators, AntiLag) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->antiLagEnabled = true;

	setTable(config->ALSFuelAdjustment, 50 /* percent */);
	Sensor::setMockValue(SensorType::Clt, 70);
	Sensor::setMockValue(SensorType::Rpm, 1000);
	Sensor::setMockValue(SensorType::Tps1, 1);
	Sensor::setMockValue(SensorType::AcceleratorPedal, 1);

	engineConfiguration->antiLagActivationMode = SWITCH_INPUT_ANTILAG;
	engine->periodicFastCallback();
	float fuelWithCorrections = 1.01;
	ASSERT_NEAR(fuelWithCorrections, getRunningFuel(1), EPS4D);
	// in unit tests we pretend that physical swiych is always OFF
	ASSERT_EQ(0, engine->antilagController.ALSSwitchCondition);
	ASSERT_EQ(0.0, engine->antilagController.fuelALSCorrection);

	engineConfiguration->antiLagActivationMode = ALWAYS_ON_ANTILAG;
	engine->periodicFastCallback();
	ASSERT_EQ(1, engine->antilagController.ALSSwitchCondition);
	ASSERT_NEAR(50, engine->antilagController.fuelALSCorrection, EPS4D);

	ASSERT_EQ(1, engine->antilagController.ALSMinRPMCondition);
	ASSERT_EQ(1, engine->antilagController.ALSMaxRPMCondition);
	ASSERT_EQ(1, engine->antilagController.ALSMinCLTCondition);
	ASSERT_EQ(1, engine->antilagController.ALSMaxCLTCondition);
	ASSERT_EQ(1, engine->antilagController.ALSMaxThrottleIntentCondition);

	ASSERT_EQ(1, engine->antilagController.isAntilagCondition);
	ASSERT_NEAR(1.5 * fuelWithCorrections, getRunningFuel(1), EPS4D);
}
