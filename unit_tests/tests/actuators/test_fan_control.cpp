#include "pch.h"

static void updateFans() {
	engine->module<FanControl1>()->onSlowCallback();
}

TEST(Actuators, Fan) {
	struct MockAc : public AcController {
		bool acState = false;

		bool isAcEnabled() const override {
			return acState;
		}
	};

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAc mockAc;
	engine->module<AcController>().set(&mockAc);

	engineConfiguration->fanOnTemperature = 90;
	engineConfiguration->fanOffTemperature = 80;
	engineConfiguration->enableFan1WithAc = false;

	// Cold, fan should be off
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans();
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	// Between thresholds, should still be off
	Sensor::setMockValue(SensorType::Clt, 85);
	updateFans();
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	// Hot, fan should turn on
	Sensor::setMockValue(SensorType::Clt, 95);
	updateFans();
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Between thresholds, should stay on
	Sensor::setMockValue(SensorType::Clt, 85);
	updateFans();
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Below threshold, should turn off
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans();
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	engineConfiguration->enableFan1WithAc = true;
	// Now AC is on, fan should turn on!
	mockAc.acState = true;
	updateFans();
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Turn off AC, fan should turn off too.
	mockAc.acState = false;
	updateFans();
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	// Back to hot, fan should turn on
	Sensor::setMockValue(SensorType::Clt, 95);
	updateFans();
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Engine starts cranking, fan should turn off
	engine->rpmCalculator.setRpmValue(100);
	updateFans();
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	// Engine running, fan should turn back on
	engine->rpmCalculator.setRpmValue(1000);
	updateFans();
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Stop the engine, fan should stay on
	engine->rpmCalculator.setRpmValue(0);
	updateFans();
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// Set configuration to inhibit fan while engine is stopped, fan should stop
	engineConfiguration->disableFan1WhenStopped = true;
	updateFans();
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());
}
