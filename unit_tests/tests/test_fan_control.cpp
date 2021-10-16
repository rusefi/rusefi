#include "pch.h"

#include "fan_control.h"

TEST(FanControl, fan1) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	engineConfiguration->fanOnTemperature = 90;
	engineConfiguration->fanOffTemperature = 80;
	engineConfiguration->enableFan1WithAc = false;

	// Cold, fan should be off
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans(false PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	// Between thresholds, should still be off
	Sensor::setMockValue(SensorType::Clt, 85);
	updateFans(false PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	// Hot, fan should turn on
	Sensor::setMockValue(SensorType::Clt, 95);
	updateFans(false PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Between thresholds, should stay on
	Sensor::setMockValue(SensorType::Clt, 85);
	updateFans(false PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Below threshold, should turn off
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans(false PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	engineConfiguration->enableFan1WithAc = true;
	// Now AC is on, fan should turn on!
	updateFans(true PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Turn off AC, fan should turn off too.
	updateFans(false PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	// Back to hot, fan should turn on
	Sensor::setMockValue(SensorType::Clt, 95);
	updateFans(false PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Engine starts cranking, fan should turn off
	ENGINE(rpmCalculator).setRpmValue(100);
	updateFans(false PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	// Engine running, fan should turn back on
	ENGINE(rpmCalculator).setRpmValue(1000);
	updateFans(false PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Stop the engine, fan should stay on
	ENGINE(rpmCalculator).setRpmValue(0);
	updateFans(false PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Set configuration to inhibit fan while engine is stopped, fan should stop
	engineConfiguration->disableFan1WhenStopped = true;
	updateFans(false PASS_ENGINE_PARAMETER_SUFFIX);
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());
}
