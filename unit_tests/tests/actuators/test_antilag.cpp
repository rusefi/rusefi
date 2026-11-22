#include "pch.h"
#include "fuel_math.h"

TEST(Actuators, AntiLag) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->antiLagEnabled = true;
	engineConfiguration->ALSMaxDuration = 0;

	setTable(config->ALSFuelAdjustment, 50 /* percent */);
	Sensor::setMockValue(SensorType::Clt, 70);
	Sensor::setMockValue(SensorType::Rpm, 1000);
	Sensor::setMockValue(SensorType::Tps1, 1);
	Sensor::setMockValue(SensorType::AcceleratorPedal, 1);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 1);

	engineConfiguration->antiLagActivationMode = SWITCH_INPUT_ANTILAG;
	engine->periodicFastCallback();
	float fuelWithCorrections = 1.01f;
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

TEST(Actuators, AntilagStopsAfterConfiguredTimeout) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->antiLagEnabled = true;
	engineConfiguration->antiLagActivationMode = ALWAYS_ON_ANTILAG;
	engineConfiguration->ALSMinRPM = 400;
	engineConfiguration->ALSMaxRPM = 3200;
	engineConfiguration->ALSMinCLT = 0;
	engineConfiguration->ALSMaxCLT = 105;
	engineConfiguration->ALSMaxTPS = 5;
	engineConfiguration->ALSMaxDuration = 1;
	setTable(config->ALSFuelAdjustment, 50);
	Sensor::setMockValue(SensorType::Clt, 70);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 1);

	Sensor::setMockValue(SensorType::Rpm, 4000);
	engine->antilagController.update();
	ASSERT_FALSE(engine->antilagController.isAntilagCondition);

	Sensor::setMockValue(SensorType::Rpm, 1000);
	engine->antilagController.update();
	ASSERT_TRUE(engine->antilagController.isAntilagCondition);
	ASSERT_TRUE(engine->antilagController.isInsideALSTimerCondition());

	advanceTimeUs(1100000);
	engine->antilagController.update();
	ASSERT_FALSE(engine->antilagController.isInsideALSTimerCondition());
	EXPECT_FALSE(engine->antilagController.ALSTimerCondition);
	EXPECT_FALSE(engine->antilagController.isAntilagCondition);
	EXPECT_FLOAT_EQ(0, engine->antilagController.fuelALSCorrection);

	Sensor::setMockValue(SensorType::Rpm, 4000);
	engine->antilagController.update();
	Sensor::setMockValue(SensorType::Rpm, 1000);
	engine->antilagController.update();
	EXPECT_TRUE(engine->antilagController.isAntilagCondition);
}

static void setupAntilagWithTimeout(int timeoutSeconds) {
	engineConfiguration->antiLagEnabled = true;
	engineConfiguration->antiLagActivationMode = LUA_ANTILAG;
	engineConfiguration->ALSMinRPM = 400;
	engineConfiguration->ALSMaxRPM = 7000;
	engineConfiguration->ALSMinCLT = 0;
	engineConfiguration->ALSMaxCLT = 105;
	engineConfiguration->ALSMaxTPS = 5;
	engineConfiguration->ALSMaxDuration = timeoutSeconds;
	Sensor::setMockValue(SensorType::Clt, 70);
	Sensor::setMockValue(SensorType::Rpm, 3000);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 1);
}

// https://github.com/rusefi/rusefi/issues/10701
// The engine never exceeds "Max RPM" here, as with a Max RPM set near the rev limit.
TEST(Actuators, AntilagTimeoutStartsWhenAntilagIsRequested) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupAntilagWithTimeout(3);

	// Long after start-up, with anti-lag not requested.
	engine->antilagController.luaAntilagState = false;
	advanceTimeUs(60 * 1000000);
	engine->antilagController.update();
	ASSERT_FALSE(engine->antilagController.isAntilagCondition);

	engine->antilagController.luaAntilagState = true;
	engine->antilagController.update();
	EXPECT_TRUE(engine->antilagController.isAntilagCondition);
	EXPECT_TRUE(engine->antilagController.ALSTimerCondition);

	advanceTimeUs(2900000);
	engine->antilagController.update();
	EXPECT_TRUE(engine->antilagController.isAntilagCondition);

	advanceTimeUs(200000);
	engine->antilagController.update();
	EXPECT_FALSE(engine->antilagController.isAntilagCondition);
	EXPECT_FALSE(engine->antilagController.ALSTimerCondition);
}

TEST(Actuators, AntilagTimeoutRearmsAfterRequestIsReleased) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupAntilagWithTimeout(3);
	engine->antilagController.luaAntilagState = true;

	engine->antilagController.update();
	ASSERT_TRUE(engine->antilagController.isAntilagCondition);

	advanceTimeUs(3100000);
	engine->antilagController.update();
	ASSERT_FALSE(engine->antilagController.isAntilagCondition);

	// The driver gets back on the throttle and lifts again.
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50);
	engine->antilagController.update();
	ASSERT_FALSE(engine->antilagController.isAntilagCondition);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 1);
	engine->antilagController.update();
	EXPECT_TRUE(engine->antilagController.isAntilagCondition);
}
