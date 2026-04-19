#include "pch.h"

#include "fan_control.h"

static void updateFans() {
	engine->module<FanControl1>()->onSlowCallback();
}

struct MockAcOff : public AcController {
	bool isAcEnabled() const override { return false; }
};

struct MockAcOn : public AcController {
	bool isAcEnabled() const override { return true; }
};

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

	// Break the CLT sensor - fan turns on
	Sensor::setInvalidMockValue(SensorType::Clt);
	updateFans();
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// CLT sensor back to normal, fan turns off
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

// Helper: configure fan 1 for PWM mode with a linear 80-110°C → 0-100% curve.
// min/max 0-100%, no AC adder, no soft-start unless overridden by caller.
static void setupFan1Pwm(EngineTestHelper& /*eth*/) {
	engineConfiguration->fan1PwmEnabled = true;
	setLinearCurve(engineConfiguration->fan1TempBins, 80, 110);
	setLinearCurve(engineConfiguration->fan1PwmValues, 0, 100);
	engineConfiguration->fan1MinPwm = 0;
	engineConfiguration->fan1MaxPwm = 100;
	engineConfiguration->fan1AcAdder = 0;
	engineConfiguration->fan1SoftStartSec = 0.0f;
}

TEST(Actuators, FanPwm_ActiveFlag) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);

	Sensor::setMockValue(SensorType::Clt, 95);

	// Relay mode: pwmActive must be false
	engineConfiguration->fan1PwmEnabled = false;
	updateFans();
	EXPECT_EQ(false, engine->module<FanControl1>()->pwmActive);

	// PWM mode: pwmActive must be true
	setupFan1Pwm(eth);
	updateFans();
	EXPECT_EQ(true, engine->module<FanControl1>()->pwmActive);
}

TEST(Actuators, FanPwm_CurveInterpolation) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	setupFan1Pwm(eth);

	// Below curve start → 0%
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans();
	EXPECT_NEAR(0.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// Midpoint of curve ~95°C → ~50%
	Sensor::setMockValue(SensorType::Clt, 95);
	updateFans();
	EXPECT_NEAR(50.0f, engine->module<FanControl1>()->pwmAppliedPwm, 2.0f);

	// Above curve end → 100%
	Sensor::setMockValue(SensorType::Clt, 115);
	updateFans();
	EXPECT_NEAR(100.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// Relay pin must NOT be driven in PWM mode
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());
}

TEST(Actuators, FanPwm_MinMaxClamp) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	setupFan1Pwm(eth);

	// With minPwm=30, a cold temperature (curve→0%) should be clamped up to 30%
	engineConfiguration->fan1MinPwm = 30;
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans();
	EXPECT_NEAR(30.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// With maxPwm=60, a hot temperature (curve→100%) should be clamped down to 60%
	engineConfiguration->fan1MaxPwm = 60;
	Sensor::setMockValue(SensorType::Clt, 115);
	updateFans();
	EXPECT_NEAR(60.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);
}

TEST(Actuators, FanPwm_AcAdder) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAcOff;
	MockAcOn mockAcOn;
	engine->module<AcController>().set(&mockAcOff);
	setupFan1Pwm(eth);
	engineConfiguration->fan1AcAdder = 20;

	// At 80°C curve gives 0%; AC off → target 0%, clamped to min 0%
	Sensor::setMockValue(SensorType::Clt, 80);
	updateFans();
	EXPECT_NEAR(0.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// AC on → 0% + 20 adder = 20%
	engine->module<AcController>().set(&mockAcOn);
	updateFans();
	EXPECT_NEAR(20.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// AC adder is clamped by maxPwm: hot temp (100%) + 20 adder = 120% → clamped to 100%
	Sensor::setMockValue(SensorType::Clt, 115);
	updateFans();
	EXPECT_NEAR(100.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);
}

TEST(Actuators, FanPwm_InvalidClt) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	setupFan1Pwm(eth);
	engineConfiguration->fan1MaxPwm = 100;

	// Start cold so we know current PWM is low
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans();
	EXPECT_NEAR(0.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// Break the sensor → output must jump to maxPwm (fail-safe, not 0)
	Sensor::setInvalidMockValue(SensorType::Clt);
	updateFans();
	EXPECT_NEAR(100.0f, engine->module<FanControl1>()->pwmAppliedPwm, 0.01f);
	EXPECT_EQ(true, engine->module<FanControl1>()->pwmActive);

	// Sensor restored → normal output resumes
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans();
	EXPECT_NEAR(0.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);
}

TEST(Actuators, FanPwm_SoftStart) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	setupFan1Pwm(eth);

	// 2s soft-start at 20 Hz = 40 steps to ramp 0→100%, so 2.5% per step
	engineConfiguration->fan1SoftStartSec = 2.0f;

	// Hot temperature so curve target is 100%
	Sensor::setMockValue(SensorType::Clt, 115);

	// First step: applied must be ~2.5%, not 100%
	updateFans();
	float after1 = engine->module<FanControl1>()->pwmAppliedPwm;
	EXPECT_NEAR(2.5f, after1, 0.5f);

	// After enough steps the ramp should reach the target
	for (int i = 0; i < 50; i++) {
		updateFans();
	}
	EXPECT_NEAR(100.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// Downward change (cooler) is instant – no ramp on the way down
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans();
	EXPECT_NEAR(0.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);
}

TEST(Actuators, FanPwm_RelayModeUnchanged) {
	// Ensure PWM disabled still uses the existing on/off relay path
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);

	engineConfiguration->fan1PwmEnabled = false;
	engineConfiguration->fanOnTemperature = 90;
	engineConfiguration->fanOffTemperature = 80;

	Sensor::setMockValue(SensorType::Clt, 95);
	updateFans();
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());
	EXPECT_EQ(false, engine->module<FanControl1>()->pwmActive);

	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans();
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());
}
