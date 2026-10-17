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
	engineConfiguration->fan1AcMode = fan_ac_mode_e::Disabled;

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

	engineConfiguration->fan1AcMode = fan_ac_mode_e::Relay;
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

TEST(Actuators, FanPwm_MinMaxAsOffAndFullDuty) {
	// fan1MinPwm/fan1MaxPwm are the duty cycles meaning "off" (0% demand) and "full speed"
	// (100% demand); the demand curve is interpolated between them, not clamped against them.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	setupFan1Pwm(eth);

	// Cold -> fan disabled -> output at the "off" duty, 30%
	engineConfiguration->fan1MinPwm = 30;
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans();
	EXPECT_NEAR(30.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// Hot -> fan enabled, curve saturated at 100% demand -> output at the "full speed" duty, 60%
	engineConfiguration->fan1MaxPwm = 60;
	Sensor::setMockValue(SensorType::Clt, 115);
	updateFans();
	EXPECT_NEAR(60.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);
}

TEST(Actuators, FanPwm_AcRelayForcesMaxSpeed) {
	// Relay mode: the condenser needs full airflow whenever the compressor relay is engaged, so
	// PWM requests full demand. Soft-start is disabled in this test.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAcOff;
	MockAcOn mockAcOn;
	engine->module<AcController>().set(&mockAcOff);
	setupFan1Pwm(eth);
	engineConfiguration->fan1MinPwm = 10; // "off" duty, to prove AC-on isn't just landing on 0
	engineConfiguration->fan1AcMode = fan_ac_mode_e::Relay;

	// Cool engine, AC off -> curve says 0% -> disabled (cold) -> "off" duty
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans();
	EXPECT_NEAR(10.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// Still cool, but AC compressor engages -> fan must jump straight to 100%, not a partial adder
	engine->module<AcController>().set(&mockAcOn);
	updateFans();
	EXPECT_NEAR(100.0f, engine->module<FanControl1>()->pwmAppliedPwm, 0.01f);

	// AC off again -> back to curve-driven (still cold -> "off" duty)
	engine->module<AcController>().set(&mockAcOff);
	updateFans();
	EXPECT_NEAR(10.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);
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

// TDB coverage for https://github.com/rusefi/rusefi/issues/10248.
// Originally asserted stale indicators; now require PWM callbacks to refresh them.
template <typename TFan>
static void checkFanPwmIndicatorsUpdate() {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAcOff;
	MockAcOn mockAcOn;
	engine->module<AcController>().set(&mockAcOff);
	auto& fan = engine->module<TFan>().unmock();

	setupFan1Pwm(eth);
	setLinearCurve(engineConfiguration->fan2TempBins, 80, 110);
	setLinearCurve(engineConfiguration->fan2PwmValues, 0, 100);
	engineConfiguration->fan2MinPwm = 0;
	engineConfiguration->fan2MaxPwm = 100;
	engineConfiguration->fan2SoftStartSec = 0;
	engineConfiguration->fanOnTemperature = engineConfiguration->fan2OnTemperature = 90;
	engineConfiguration->fanOffTemperature = engineConfiguration->fan2OffTemperature = 80;
	engineConfiguration->fan1AcMode = engineConfiguration->fan2AcMode = fan_ac_mode_e::Relay;
	engineConfiguration->disableFan1WhenStopped = engineConfiguration->disableFan2WhenStopped = true;
	engineConfiguration->disableFan1AtSpeed = engineConfiguration->disableFan2AtSpeed = 50;
	engineConfiguration->cranking.rpm = 400;

	// Seed real indicators through relay mode: cold, stopped engine, AC off.
	engineConfiguration->fan1PwmEnabled = engineConfiguration->fan2PwmEnabled = false;
	Sensor::setMockValue(SensorType::Clt, 75);
	Sensor::setMockValue(SensorType::VehicleSpeed, 0);
	engine->rpmCalculator.setRpmValue(0);
	fan.onSlowCallback();
	EXPECT_FALSE(fan.pwmActive);
	EXPECT_FALSE(fan.cranking);
	EXPECT_TRUE(fan.notRunning);
	EXPECT_TRUE(fan.disabledWhileEngineStopped);
	EXPECT_FALSE(fan.brokenClt);
	EXPECT_FALSE(fan.enabledForAc);
	EXPECT_FALSE(fan.hot);
	EXPECT_TRUE(fan.cold);
	EXPECT_FALSE(fan.disabledBySpeed);
	EXPECT_EQ(static_cast<int>(RadiatorFanState::EngineStopped), fan.radiatorFanStatus);

	// Hot, running engine, AC on, above the speed limit. PWM refreshes the
	// indicators as well as its output.
	engineConfiguration->fan1PwmEnabled = engineConfiguration->fan2PwmEnabled = true;
	Sensor::setMockValue(SensorType::Clt, 95);
	Sensor::setMockValue(SensorType::VehicleSpeed, 100);
	engine->rpmCalculator.setRpmValue(1000);
	engine->module<AcController>().set(&mockAcOn);
	fan.onSlowCallback();
	EXPECT_TRUE(fan.pwmActive);
	EXPECT_EQ(0.0f, fan.pwmAppliedPwm);
	EXPECT_FALSE(fan.m_state);
	EXPECT_FALSE(fan.cranking);
	EXPECT_FALSE(fan.notRunning);
	EXPECT_FALSE(fan.disabledWhileEngineStopped);
	EXPECT_FALSE(fan.brokenClt);
	EXPECT_TRUE(fan.enabledForAc);
	EXPECT_TRUE(fan.hot);
	EXPECT_FALSE(fan.cold);
	EXPECT_TRUE(fan.disabledBySpeed);
	EXPECT_EQ(static_cast<int>(RadiatorFanState::VehicleIsTooFast), fan.radiatorFanStatus);

	// Relay-mode control: the same inputs refresh all indicators.
	engineConfiguration->fan1PwmEnabled = engineConfiguration->fan2PwmEnabled = false;
	fan.onSlowCallback();
	EXPECT_FALSE(fan.pwmActive);
	EXPECT_FALSE(fan.cranking);
	EXPECT_FALSE(fan.notRunning);
	EXPECT_FALSE(fan.disabledWhileEngineStopped);
	EXPECT_FALSE(fan.brokenClt);
	EXPECT_TRUE(fan.enabledForAc);
	EXPECT_TRUE(fan.hot);
	EXPECT_FALSE(fan.cold);
	EXPECT_TRUE(fan.disabledBySpeed);
	EXPECT_EQ(static_cast<int>(RadiatorFanState::VehicleIsTooFast), fan.radiatorFanStatus);

	// The invalid-CLT PWM fallback also refreshes indicators before returning,
	// reporting the broken sensor and the new cranking state.
	engineConfiguration->fan1PwmEnabled = engineConfiguration->fan2PwmEnabled = true;
	Sensor::setInvalidMockValue(SensorType::Clt);
	Sensor::setMockValue(SensorType::VehicleSpeed, 0);
	// Stop first to leave the running-state RPM hysteresis before cranking again.
	engine->rpmCalculator.setRpmValue(0);
	engine->rpmCalculator.setRpmValue(100);
	engine->module<AcController>().set(&mockAcOff);
	fan.onSlowCallback();
	EXPECT_TRUE(fan.pwmActive);
	EXPECT_FLOAT_EQ(0.0f, fan.pwmAppliedPwm);
	EXPECT_FALSE(fan.m_state);
	EXPECT_TRUE(fan.cranking);
	EXPECT_TRUE(fan.notRunning);
	EXPECT_TRUE(fan.disabledWhileEngineStopped);
	EXPECT_TRUE(fan.brokenClt);
	EXPECT_FALSE(fan.enabledForAc);
	EXPECT_FALSE(fan.hot);
	EXPECT_TRUE(fan.cold);
	EXPECT_FALSE(fan.disabledBySpeed);
	EXPECT_EQ(static_cast<int>(RadiatorFanState::Cranking), fan.radiatorFanStatus);

	// Relay-mode control for the cranking and broken-CLT flags.
	engineConfiguration->fan1PwmEnabled = engineConfiguration->fan2PwmEnabled = false;
	fan.onSlowCallback();
	EXPECT_FALSE(fan.pwmActive);
	EXPECT_TRUE(fan.cranking);
	EXPECT_TRUE(fan.notRunning);
	EXPECT_TRUE(fan.disabledWhileEngineStopped);
	EXPECT_TRUE(fan.brokenClt);
	EXPECT_FALSE(fan.enabledForAc);
	EXPECT_FALSE(fan.hot);
	EXPECT_TRUE(fan.cold);
	EXPECT_FALSE(fan.disabledBySpeed);
	EXPECT_EQ(static_cast<int>(RadiatorFanState::Cranking), fan.radiatorFanStatus);
}

TEST(Actuators, Fan1Pwm_IndicatorsUpdate_Issue10248) {
	checkFanPwmIndicatorsUpdate<FanControl1>();
}

TEST(Actuators, Fan2Pwm_IndicatorsUpdate_Issue10248) {
	checkFanPwmIndicatorsUpdate<FanControl2>();
}

TEST(Actuators, FanPwm_DisableWhenStopped) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    MockAcOff mockAc;
    engine->module<AcController>().set(&mockAc);
    setupFan1Pwm(eth);

    // Enable safety flag to inhibit fan when the engine is stopped
    engineConfiguration->disableFan1WhenStopped = true;

    // Engine stopped (0 RPM) and high coolant temperature (105°C)
    engine->rpmCalculator.setRpmValue(0);
    Sensor::setMockValue(SensorType::Clt, 105);

    updateFans();

    // Engine stopped with the safety flag set: PWM output must be fully inhibited (#10251)
    EXPECT_EQ(0.0f, engine->module<FanControl1>()->fanSpeedTarget);
    EXPECT_EQ(0.0f, engine->module<FanControl1>()->pwmAppliedPwm);
}

// Companion to FanPwm_DisableWhenStopped: with the same safety flag set, a running
// engine must never be inhibited - the fan follows the CLT curve as usual. This holds
// on master today and must keep holding once the stopped-engine inhibit is fixed.
TEST(Actuators, FanPwm_DisableWhenStopped_RunningEngineStaysActive) {
    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
    MockAcOff mockAc;
    engine->module<AcController>().set(&mockAc);
    setupFan1Pwm(eth);

    engineConfiguration->disableFan1WhenStopped = true;

    // Coolant above the curve start so the curve requests a non-zero duty
    Sensor::setMockValue(SensorType::Clt, 100.0f);

    // Engine running (1000 RPM). The RpmCalculator is itself the RPM sensor, so setRpmValue()
    // drives both isRunning() (relay-path flag) and Sensor::get(SensorType::Rpm).
    engine->rpmCalculator.setRpmValue(1000);

    // Advance mock time so any soft-start ramp has a chance to step up
    eth.moveTimeForwardSec(1);
    updateFans();

    EXPECT_FALSE(engine->module<FanControl1>()->disabledWhileEngineStopped);
    EXPECT_GT(engine->module<FanControl1>()->pwmAppliedPwm, 0.0f);
}

TEST(Actuators, FanPwm_DisableAtSpeed) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	setupFan1Pwm(eth);
	engineConfiguration->disableFan1AtSpeed = 50;

	// Hot running engine, the curve asks for 50%
	Sensor::setMockValue(SensorType::Clt, 95);
	engine->rpmCalculator.setRpmValue(1000);

	// Driving above the speed limit
	Sensor::setMockValue(SensorType::VehicleSpeed, 100);
	updateFans();

	EXPECT_TRUE(engine->module<FanControl1>()->disabledBySpeed);
	EXPECT_EQ(0.0f, engine->module<FanControl1>()->pwmAppliedPwm);
}

TEST(Actuators, FanPwm_CrankingAndSensorFailureInhibit) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	setupFan1Pwm(eth);
	engine->rpmCalculator.setRpmValue(100);
	Sensor::setMockValue(SensorType::Clt, 115);
	auto& fan = engine->module<FanControl1>().unmock();
	fan.onSlowCallback();
	EXPECT_TRUE(fan.cranking);
	// Cranking now inhibits PWM just as it inhibits the relay.
	EXPECT_FLOAT_EQ(0, fan.pwmAppliedPwm);

	Sensor::setInvalidMockValue(SensorType::Clt);
	fan.onSlowCallback();
	EXPECT_TRUE(fan.brokenClt);
	EXPECT_TRUE(fan.cranking);
	// A sensor failure must not bypass the cranking inhibit.
	EXPECT_FLOAT_EQ(0, fan.pwmAppliedPwm);
}

TEST(Actuators, FanPwm_InvertedRange) {
	// Hardware that drives the fan through an inverted PWM path (e.g. NPN transistor + pull-up):
	// a high duty cycle keeps the fan off, a low duty cycle drives it at full speed. fan1MinPwm
	// (the "off" duty) can be set higher than fan1MaxPwm (the "full speed" duty) to describe this.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	setupFan1Pwm(eth);
	engineConfiguration->fan1MinPwm = 90;
	engineConfiguration->fan1MaxPwm = 10;

	// Fan should be off (cold) -> duty at the "off" level, 90%
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans();
	EXPECT_NEAR(90.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// Fan should be full speed (hot) -> duty at the "full" level, 10%
	Sensor::setMockValue(SensorType::Clt, 115);
	updateFans();
	EXPECT_NEAR(10.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// Midpoint of the curve (~95C, ~50% demand) -> duty halfway between 90 and 10, i.e. 50%
	Sensor::setMockValue(SensorType::Clt, 95);
	updateFans();
	EXPECT_NEAR(50.0f, engine->module<FanControl1>()->pwmAppliedPwm, 2.0f);
}

TEST(Actuators, FanPwm_DisableGatesUseOffDuty) {
	// Cranking/disable-when-stopped/disable-at-speed (isHardInhibited(), shared with the relay
	// path) still force PWM mode to the curve's lowest-bin ("off") demand, regardless of what the
	// curve says for the current temperature.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	setupFan1Pwm(eth);
	engineConfiguration->fan1MinPwm = 15;
	engineConfiguration->fan1MaxPwm = 100;

	// Hot enough that the curve alone would call for full speed
	Sensor::setMockValue(SensorType::Clt, 115);
	updateFans();
	EXPECT_NEAR(100.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// Cranking inhibits the fan even in PWM mode -> output drops to the "off" duty (15%), not 0%
	engine->rpmCalculator.setRpmValue(100);
	updateFans();
	EXPECT_NEAR(15.0f, engine->module<FanControl1>()->pwmAppliedPwm, 0.01f);

	// Running again, still hot -> back to full speed
	engine->rpmCalculator.setRpmValue(1000);
	updateFans();
	EXPECT_NEAR(100.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// disableFan1WhenStopped also inhibits PWM mode now
	engineConfiguration->disableFan1WhenStopped = true;
	engine->rpmCalculator.setRpmValue(0);
	updateFans();
	EXPECT_NEAR(15.0f, engine->module<FanControl1>()->pwmAppliedPwm, 0.01f);
}

TEST(Actuators, FanPwm_IgnoresOnOffTemperature) {
	// PWM mode has no separate on/off temperature threshold - the curve is the sole source of
	// truth. Deliberately set fanOnTemperature/fanOffTemperature to values that would force the
	// fan off under the old (relay-style) logic, and confirm PWM output tracks the curve anyway.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	setupFan1Pwm(eth);
	engineConfiguration->fanOnTemperature = 200;
	engineConfiguration->fanOffTemperature = 190;

	// Well below fanOffTemperature (190) - old logic would call this "cold" -> off. New logic:
	// just reads the curve, which says ~50% at 95C.
	Sensor::setMockValue(SensorType::Clt, 95);
	updateFans();
	EXPECT_NEAR(50.0f, engine->module<FanControl1>()->pwmAppliedPwm, 2.0f);
}

TEST(Actuators, FanPwm_CurveEndpointsAreOffAndFull) {
	// The curve's own lowest/highest bin values - not a hardcoded 0/100 - are used as the "off"
	// and "full speed" demand for CLT-broken and hard-inhibited (e.g. cranking) cases. Use a curve
	// whose endpoints are NOT 0/100 to prove this isn't coincidental.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	setupFan1Pwm(eth);
	setLinearCurve(engineConfiguration->fan1PwmValues, 20, 80); // lowest bin=20%, highest bin=80%
	engineConfiguration->fan1MinPwm = 0;
	engineConfiguration->fan1MaxPwm = 100;

	// Below the curve's range -> clamped to the lowest bin's value, 20%, not 0%.
	Sensor::setMockValue(SensorType::Clt, 50);
	updateFans();
	EXPECT_NEAR(20.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// Above the curve's range -> clamped to the highest bin's value, 80%, not 100%.
	Sensor::setMockValue(SensorType::Clt, 200);
	updateFans();
	EXPECT_NEAR(80.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// Broken CLT -> fail safe at the highest bin's value, 80%, not 100%.
	Sensor::setInvalidMockValue(SensorType::Clt);
	updateFans();
	EXPECT_NEAR(80.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);

	// Cranking (hard-inhibited) -> lowest bin's value, 20%, not 0%.
	Sensor::setMockValue(SensorType::Clt, 200);
	engine->rpmCalculator.setRpmValue(100);
	updateFans();
	EXPECT_NEAR(20.0f, engine->module<FanControl1>()->pwmAppliedPwm, 1.0f);
}

TEST(Actuators, FanPwm_AcPressureProportional) {
	// Pressure mode: demand ramps from 0% at the Off threshold to 100% at the On threshold, and
	// is never lower than what the temperature curve alone would call for.
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	setupFan1Pwm(eth);

	// Keep the temperature curve out of it: cold engine, unreachable on/off thresholds
	engineConfiguration->fanOnTemperature = 200;
	engineConfiguration->fanOffTemperature = 190;
	Sensor::setMockValue(SensorType::Clt, 75);

	engineConfiguration->fan1AcMode = fan_ac_mode_e::Pressure;
	engineConfiguration->fan1AcPressureOn = 1400;
	engineConfiguration->fan1AcPressureOff = 1100;

	// At/above the On threshold -> full speed
	Sensor::setMockValue(SensorType::AcPressure, 1500);
	updateFans();
	EXPECT_NEAR(100.0f, engine->module<FanControl1>()->pwmAppliedPwm, 0.01f);

	// Halfway between Off (1100) and On (1400) -> ~50%
	Sensor::setMockValue(SensorType::AcPressure, 1250);
	updateFans();
	EXPECT_NEAR(50.0f, engine->module<FanControl1>()->pwmAppliedPwm, 2.0f);

	// Below the Off threshold -> pressure demand is 0, curve alone (cold) also says 0 -> "off" duty
	Sensor::setMockValue(SensorType::AcPressure, 1000);
	updateFans();
	EXPECT_NEAR(0.0f, engine->module<FanControl1>()->pwmAppliedPwm, 0.01f);

	// Pressure assistance cannot reduce a larger temperature demand.
	Sensor::setMockValue(SensorType::Clt, 115);
	Sensor::setMockValue(SensorType::AcPressure, 1250);
	updateFans();
	EXPECT_FLOAT_EQ(100, engine->module<FanControl1>()->fanSpeedTarget);
	EXPECT_TRUE(engine->module<FanControl1>()->enabledForAcPressure);

	// A missing sensor or invalid pressure range leaves temperature control active.
	Sensor::setInvalidMockValue(SensorType::AcPressure);
	updateFans();
	EXPECT_FLOAT_EQ(100, engine->module<FanControl1>()->fanSpeedTarget);
	EXPECT_FALSE(engine->module<FanControl1>()->enabledForAcPressure);
	Sensor::setMockValue(SensorType::Clt, 75);
	Sensor::setMockValue(SensorType::AcPressure, 1500);
	engineConfiguration->fan1AcPressureOn = engineConfiguration->fan1AcPressureOff;
	updateFans();
	EXPECT_FLOAT_EQ(0, engine->module<FanControl1>()->fanSpeedTarget);
	EXPECT_FALSE(engine->module<FanControl1>()->enabledForAcPressure);
}

TEST(Actuators, FanVssHysteresis) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	engineConfiguration->fan1AcMode = fan_ac_mode_e::Disabled;

	engineConfiguration->fanOnTemperature = 90;
	engineConfiguration->fanOffTemperature = 80;
	engineConfiguration->disableFan1AtSpeed = 50;
	engineConfiguration->disableFan1AtSpeedHysteresis = 10;

	// Hot CLT, VSS below threshold → fan on
	Sensor::setMockValue(SensorType::Clt, 95);
	Sensor::setMockValue(SensorType::VehicleSpeed, 30);
	updateFans();
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// VSS exceeds threshold → fan disabled by speed
	Sensor::setMockValue(SensorType::VehicleSpeed, 55);
	updateFans();
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());
	EXPECT_EQ(true, engine->module<FanControl1>()->disabledBySpeed);

	// VSS drops below threshold but still inside hysteresis band (45 < 50 but > 50-10=40) → stays off
	Sensor::setMockValue(SensorType::VehicleSpeed, 45);
	updateFans();
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());
	EXPECT_EQ(true, engine->module<FanControl1>()->disabledBySpeed);

	// VSS drops below (threshold - hysteresis) → fan re-enabled
	Sensor::setMockValue(SensorType::VehicleSpeed, 38);
	updateFans();
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());
	EXPECT_EQ(false, engine->module<FanControl1>()->disabledBySpeed);
}

TEST(Actuators, FanAcPressureModeIgnoresCompressorState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);

	// Cold CLT and speed/etc. all neutral so only the pressure logic can turn the fan on.
	engineConfiguration->fanOnTemperature = 200;
	engineConfiguration->fanOffTemperature = 190;
	Sensor::setMockValue(SensorType::Clt, 75);

	engineConfiguration->fan1AcMode = fan_ac_mode_e::Pressure;
	engineConfiguration->fan1AcPressureOn = 1400;
	engineConfiguration->fan1AcPressureOff = 1100;

	// Compressor is off, but pressure is above the On threshold -> fan must turn on anyway.
	// Inside the band, retain the previous off state.
	Sensor::setMockValue(SensorType::AcPressure, 1250);
	updateFans();
	EXPECT_FALSE(enginePins.fanRelay.getLogicValue());

	Sensor::setMockValue(SensorType::AcPressure, 1500);
	updateFans();
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());

	// The off threshold itself retains the on state.
	Sensor::setMockValue(SensorType::AcPressure, 1100);
	updateFans();
	EXPECT_TRUE(enginePins.fanRelay.getLogicValue());
	EXPECT_TRUE(engine->module<FanControl1>()->enabledForAcPressure);

	// Pressure drops below the Off threshold -> fan turns back off, still with compressor off.
	Sensor::setMockValue(SensorType::AcPressure, 1000);
	updateFans();
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());

	// Invalid pressure reading -> fan must not be commanded on by pressure logic.
	Sensor::setMockValue(SensorType::AcPressure, 1400);
	updateFans();
	EXPECT_EQ(true, enginePins.fanRelay.getLogicValue());
	Sensor::setInvalidMockValue(SensorType::AcPressure);
	updateFans();
	EXPECT_EQ(false, enginePins.fanRelay.getLogicValue());
}

TEST(Actuators, FanPwm_InvertedSoftStartAndFailure) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOff mockAc;
	engine->module<AcController>().set(&mockAc);
	setupFan1Pwm(eth);
	engineConfiguration->fan1MinPwm = 90;
	engineConfiguration->fan1MaxPwm = 10;
	engineConfiguration->fan1SoftStartSec = 2;
	auto& fan = engine->module<FanControl1>().unmock();

	Sensor::setMockValue(SensorType::Clt, 115);
	updateFans();
	EXPECT_FLOAT_EQ(100, fan.fanSpeedTarget);
	EXPECT_FLOAT_EQ(2.5f, fan.fanSpeedApplied);
	EXPECT_FLOAT_EQ(88, fan.pwmAppliedPwm);
	EXPECT_TRUE(fan.m_state);

	// Decreasing demand is immediate even though raw duty increases.
	Sensor::setMockValue(SensorType::Clt, 75);
	updateFans();
	EXPECT_FLOAT_EQ(0, fan.fanSpeedApplied);
	EXPECT_FLOAT_EQ(90, fan.pwmAppliedPwm);
	EXPECT_FALSE(fan.m_state);

	// Fault response bypasses the soft-start, but never overrides speed inhibition.
	Sensor::setInvalidMockValue(SensorType::Clt);
	updateFans();
	EXPECT_FLOAT_EQ(100, fan.fanSpeedApplied);
	EXPECT_FLOAT_EQ(10, fan.pwmAppliedPwm);
	engineConfiguration->disableFan1AtSpeed = 50;
	Sensor::setMockValue(SensorType::VehicleSpeed, 60);
	updateFans();
	EXPECT_FLOAT_EQ(0, fan.fanSpeedApplied);
	EXPECT_FLOAT_EQ(90, fan.pwmAppliedPwm);
	EXPECT_FALSE(fan.m_state);
	EXPECT_TRUE(fan.brokenClt);
	EXPECT_EQ(static_cast<int>(RadiatorFanState::VehicleIsTooFast), fan.radiatorFanStatus);
}

TEST(Actuators, FanPwm_SpeedHysteresisAndInvalidSensor) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupFan1Pwm(eth);
	engineConfiguration->disableFan1AtSpeed = 50;
	engineConfiguration->disableFan1AtSpeedHysteresis = 10;
	Sensor::setMockValue(SensorType::Clt, 115);
	auto& fan = engine->module<FanControl1>().unmock();
	Sensor::setMockValue(SensorType::VehicleSpeed, 51);
	updateFans();
	EXPECT_FLOAT_EQ(0, fan.fanSpeedTarget);
	Sensor::setMockValue(SensorType::VehicleSpeed, 40);
	updateFans();
	EXPECT_FLOAT_EQ(0, fan.fanSpeedTarget);
	Sensor::setMockValue(SensorType::VehicleSpeed, 39);
	updateFans();
	EXPECT_FLOAT_EQ(100, fan.fanSpeedTarget);
	Sensor::setMockValue(SensorType::VehicleSpeed, 50);
	updateFans();
	EXPECT_FALSE(fan.disabledBySpeed);
	Sensor::setMockValue(SensorType::VehicleSpeed, 51);
	updateFans();
	Sensor::setInvalidMockValue(SensorType::VehicleSpeed);
	updateFans();
	EXPECT_FALSE(fan.disabledBySpeed);
	EXPECT_FLOAT_EQ(100, fan.fanSpeedTarget);
}

TEST(Actuators, FanAcModesIndependent) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	MockAcOn mockAc;
	engine->module<AcController>().set(&mockAc);
	Sensor::setMockValue(SensorType::Clt, 75);
	Sensor::setMockValue(SensorType::AcPressure, 1900);
	engineConfiguration->fan1AcMode = fan_ac_mode_e::Disabled;
	engineConfiguration->fan2AcMode = fan_ac_mode_e::Pressure;
	engineConfiguration->fan2PwmEnabled = true;
	engineConfiguration->fan2SoftStartSec = 0;
	auto& fan1 = engine->module<FanControl1>().unmock();
	auto& fan2 = engine->module<FanControl2>().unmock();
	fan1.onSlowCallback();
	fan2.onSlowCallback();
	EXPECT_FALSE(fan1.m_state);
	EXPECT_FALSE(fan1.enabledForAc);
	EXPECT_FALSE(fan1.enabledForAcPressure);
	EXPECT_FLOAT_EQ(100, fan2.fanSpeedTarget);
	EXPECT_FALSE(fan2.enabledForAc);
	EXPECT_TRUE(fan2.enabledForAcPressure);

	engineConfiguration->fan1AcMode = fan_ac_mode_e::Relay;
	engineConfiguration->fan2AcMode = fan_ac_mode_e::Disabled;
	fan1.onSlowCallback();
	fan2.onSlowCallback();
	EXPECT_TRUE(fan1.m_state);
	EXPECT_TRUE(fan1.enabledForAc);
	EXPECT_FALSE(fan2.m_state);
	EXPECT_FALSE(fan2.enabledForAcPressure);
}
