#include "efi_gpio.h"
#include "engine_test_helper.h"
#include "fan_control.h"
#include "sensor.h"

TEST(FanControl, fan1) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	engineConfiguration->fanOnTemperature = 90;
	engineConfiguration->fanOffTemperature = 80;
	engineConfiguration->enableFan1WithAc = false;
	engine->acSwitchState = false;

	// Cold, fan should be off
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	// Between thresholds, should still be off
	Sensor::setMockValue(SensorType::Clt, 85);
	updateFans(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	// Hot, fan should turn on
	Sensor::setMockValue(SensorType::Clt, 95);
	updateFans(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Between thresholds, should stay on
	Sensor::setMockValue(SensorType::Clt, 85);
	updateFans(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Below threshold, should turn off
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	engineConfiguration->enableFan1WithAc = true;
	// Now AC is on, fan should turn on!
	engine->acSwitchState = true;
	updateFans(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Turn off AC, fan should turn off too.
	engine->acSwitchState = false;
	updateFans(PASS_ENGINE_PARAMETER_SIGNATURE);
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());
}
