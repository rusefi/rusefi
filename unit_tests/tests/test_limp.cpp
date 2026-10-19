#include "pch.h"

#include "fuel_math.h"
#include "limp_manager.h"

TEST(limp, zeroLambdaDelayDoesNotCutWhenProtectionDisabled) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->lambdaProtectionEnable = false;
	engineConfiguration->lambdaProtectionTimeout = 0;
	engineConfiguration->lambdaProtectionRestoreRpm = 2000;
	Sensor::setMockValue(SensorType::Lambda1, 0.85f);

	engine->lambdaMonitor.update(3000, 80);
	ASSERT_TRUE(engine->lambdaMonitor.lambdaCurrentlyGood);
	EXPECT_FALSE(engine->lambdaMonitor.isCut());

	LimpManager dut;
	dut.updateState(3000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());
}

TEST(limp, disablingLambdaProtectionReleasesActiveCut) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->lambdaProtectionEnable = false;
	engineConfiguration->lambdaProtectionTimeout = 0;
	engineConfiguration->lambdaProtectionRestoreRpm = 2000;
	engine->lambdaMonitor.lambdaMonitorCut = true;

	engine->lambdaMonitor.update(3000, 80);
	EXPECT_FALSE(engine->lambdaMonitor.isCut());
}

TEST(limp, zeroLowOilDelayDoesNotCutWithHealthyPressure) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->minOilPressureAfterStart = 0;
	engineConfiguration->enableOilPressureProtect = true;
	engineConfiguration->minimumOilPressureTimeout = 0;
	setArrayValues(config->minimumOilPressureValues, 100);
	setArrayValues(config->maximumOilPressureValues, 0);
	Sensor::setMockValue(SensorType::OilPressure, 200);
	engine->rpmCalculator.setRpmValue(1000);

	LimpManager dut;
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// A positive delay avoids the false cut with exactly the same inputs.
	engineConfiguration->minimumOilPressureTimeout = 0.1f;
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// Zero delay still cuts immediately when pressure is genuinely low.
	engineConfiguration->minimumOilPressureTimeout = 0;
	Sensor::setMockValue(SensorType::OilPressure, 50);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_EQ(ClearReason::OilPressure, dut.allowInjection().reason);
}

TEST(limp, zeroHighOilDelayDoesNotCutWithHealthyPressure) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->minOilPressureAfterStart = 0;
	engineConfiguration->enableOilPressureProtect = false;
	engineConfiguration->maxOilPressureTimeout = 0;
	setArrayValues(config->maximumOilPressureValues, 500);
	Sensor::setMockValue(SensorType::OilPressure, 200);
	engine->rpmCalculator.setRpmValue(1000);

	LimpManager dut;
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	engineConfiguration->maxOilPressureTimeout = 0.1f;
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	engineConfiguration->maxOilPressureTimeout = 0;
	Sensor::setMockValue(SensorType::OilPressure, 600);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_EQ(ClearReason::OilPressure, dut.allowInjection().reason);
}

TEST(limp, zeroInjectorDutyDelayDoesNotCutBelowBothLimits) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->maxInjectorDutyInstant = 95;
	engineConfiguration->maxInjectorDutySustained = 85;
	engineConfiguration->maxInjectorDutySustainedTimeout = 0;
	engine->engineState.injectionDuration = 12;

	// At 3000 RPM the 720-degree cycle is 40 ms, so duty is 30%.
	ASSERT_NEAR(30, getInjectorDutyCycle(3000), 0.01f);
	LimpManager dut;
	dut.updateState(3000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// 36 ms in a 40 ms cycle exceeds the sustained limit but not the instant limit.
	engine->engineState.injectionDuration = 36;
	ASSERT_NEAR(90, getInjectorDutyCycle(3000), 0.01f);
	dut.updateState(3000, getTimeNowNt());
	EXPECT_EQ(ClearReason::InjectorDutyCycle, dut.allowInjection().reason);
}

TEST(limp, testFatalError) {
  EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LimpManager dut;

	// Everything should work by default
	ASSERT_TRUE(dut.allowElectronicThrottle());
	ASSERT_TRUE(dut.allowIgnition());
	ASSERT_TRUE(dut.allowInjection());
	ASSERT_TRUE(dut.allowTriggerInput());

	dut.fatalError();

	// Fatal error should kill everything
	EXPECT_FALSE(dut.allowElectronicThrottle());
	EXPECT_FALSE(dut.allowIgnition());
	EXPECT_FALSE(dut.allowInjection());
	EXPECT_FALSE(dut.allowTriggerInput());
}

TEST(limp, revLimit) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->rpmHardLimit = 2500;
	engineConfiguration->cutFuelOnHardLimit = true;
	engineConfiguration->cutSparkOnHardLimit = true;

	LimpManager dut;

	// Under rev limit, inj/ign allowed
	dut.updateState(2000, 0);
	EXPECT_TRUE(dut.allowIgnition());
	EXPECT_TRUE(dut.allowInjection());

	// Over rev limit, no injection
	dut.updateState(3000, 0);
	EXPECT_FALSE(dut.allowIgnition());
	EXPECT_FALSE(dut.allowInjection());

	// Now recover back to under limit
	dut.updateState(2000, 0);
	EXPECT_TRUE(dut.allowIgnition());
	EXPECT_TRUE(dut.allowInjection());
}

TEST(limp, revLimitCltBased) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->rpmHardLimit = 2500;
	engineConfiguration->cutFuelOnHardLimit = true;
	engineConfiguration->cutSparkOnHardLimit = true;

	// Configure CLT-based rev limit curve
	engineConfiguration->useCltBasedRpmLimit = true;
	copyArray(config->cltRevLimitRpmBins, { 10, 20, 30, 40 });
	copyArray(config->cltRevLimitRpm, { 1000, 2000, 3000, 4000 });

	LimpManager dut;

	// Check low temperature first
	Sensor::setMockValue(SensorType::Clt, 10);

	// Under rev limit, inj/ign allowed
	dut.updateState(900, 0);
	EXPECT_TRUE(dut.allowIgnition());
	EXPECT_TRUE(dut.allowInjection());

	// Over rev limit, no injection
	dut.updateState(1100, 0);
	EXPECT_FALSE(dut.allowIgnition());
	EXPECT_FALSE(dut.allowInjection());

	// Now recover back to under limit
	dut.updateState(900, 0);
	EXPECT_TRUE(dut.allowIgnition());
	EXPECT_TRUE(dut.allowInjection());


	// Check middle temperature
	Sensor::setMockValue(SensorType::Clt, 35);

	// Under rev limit, inj/ign allowed
	dut.updateState(3400, 0);
	EXPECT_TRUE(dut.allowIgnition());
	EXPECT_TRUE(dut.allowInjection());

	// Over rev limit, no injection
	dut.updateState(3600, 0);
	EXPECT_FALSE(dut.allowIgnition());
	EXPECT_FALSE(dut.allowInjection());

	// Now recover back to under limit
	dut.updateState(3400, 0);
	EXPECT_TRUE(dut.allowIgnition());
	EXPECT_TRUE(dut.allowInjection());
}

TEST(limp, revHardLimitHyst) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->rpmHardLimit = 2500;
	engineConfiguration->rpmHardLimitHyst = 200;
	engineConfiguration->cutSparkOnHardLimit = true;

	LimpManager dut;

	// Under rev limit, inj/ign allowed
	dut.updateState(2500, 0);
	EXPECT_TRUE(dut.allowIgnition());
	EXPECT_TRUE(dut.allowInjection());

	// Over rev limit, no injection or ignition
	dut.updateState(2501, 0);
	EXPECT_FALSE(dut.allowIgnition());
	EXPECT_FALSE(dut.allowInjection());

	// Now set back inside the limit window - still not allowed
	dut.updateState(2300, 0);
	EXPECT_FALSE(dut.allowIgnition());
	EXPECT_FALSE(dut.allowInjection());

	// Now recover back to under lower limit
	dut.updateState(2299, 0);
	EXPECT_TRUE(dut.allowIgnition());
	EXPECT_TRUE(dut.allowInjection());
}

TEST(limp, revSoftLimit) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->rpmHardLimit = 2500;
	engineConfiguration->rpmHardLimitHyst = 200;
	engineConfiguration->rpmSoftLimitTimingRetard = 10; // 10 deg
	engineConfiguration->rpmSoftLimitFuelAdded = 20;	// 20%
	engineConfiguration->cutSparkOnHardLimit = true;

	eth.engine.updateSlowSensors();
	Sensor::setMockValue(SensorType::Clt, 36.605f);
	Sensor::setMockValue(SensorType::Iat, 30.0f);

	// this is 5ms base fuel with some default CLT/IAT corrections
	static const float baseFuel = 5.0f;
	static const float normalRunningFuel = 5.3679f;

	// Under rev limit, no inj/ign corrections
	Sensor::setMockValue(SensorType::Rpm, 2300);
	eth.engine.periodicFastCallback();

	EXPECT_FLOAT_EQ(0, getLimpManager()->getLimitingTimingRetard());
	EXPECT_FLOAT_EQ((float)eth.engine.ignitionState.correctedIgnitionAdvance - (float)eth.engine.ignitionState.baseIgnitionAdvance, 0);
	EXPECT_FLOAT_EQ(1, getLimpManager()->getLimitingFuelCorrection());
	// this is normal injection mode, no limiting fuel corrections
	ASSERT_NEAR(normalRunningFuel, getRunningFuel(baseFuel), EPS4D) << "base fuel";

	// For upper rev limit, we expect maximum inj/ign corrections
	Sensor::setMockValue(SensorType::Rpm, 2500);
	eth.engine.periodicFastCallback();

	EXPECT_FLOAT_EQ(10, getLimpManager()->getLimitingTimingRetard());	// 10 deg
	EXPECT_FLOAT_EQ((float)eth.engine.ignitionState.correctedIgnitionAdvance - (float)eth.engine.ignitionState.baseIgnitionAdvance, -10.0f);
	EXPECT_FLOAT_EQ(1.2f, getLimpManager()->getLimitingFuelCorrection());	// 20%
	ASSERT_NEAR(normalRunningFuel * 1.2f, getRunningFuel(baseFuel), EPS4D) << "base fuel";	// 20%

	// In the middle of the limit window, we expect 50% interpolated inj/ign corrections
	Sensor::setMockValue(SensorType::Rpm, 2400);
	eth.engine.periodicFastCallback();
	EXPECT_FLOAT_EQ(5, getLimpManager()->getLimitingTimingRetard());	// 5 deg
	EXPECT_FLOAT_EQ((float)eth.engine.ignitionState.correctedIgnitionAdvance - (float)eth.engine.ignitionState.baseIgnitionAdvance, -5.0f);
	EXPECT_FLOAT_EQ(1.1f, getLimpManager()->getLimitingFuelCorrection());	// 10%
	ASSERT_NEAR(normalRunningFuel * 1.1f, getRunningFuel(baseFuel), EPS4D) << "base fuel";	// 10%
}

TEST(limp, boostCut) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Cut above 100kPa
	engineConfiguration->boostCutPressure = 100;
	engineConfiguration->boostCutPressureHyst = 20;

	LimpManager dut;

	// Below threshold, injection allowed
	Sensor::setMockValue(SensorType::Map, 80);
	dut.updateState(1000, 0);
	EXPECT_TRUE(dut.allowInjection());

	// Above rising threshold, injection cut
	Sensor::setMockValue(SensorType::Map, 105);
	dut.updateState(1000, 0);
	EXPECT_FALSE(dut.allowInjection());

	// Below rising threshold, but should have hysteresis, so not cut yet
	Sensor::setMockValue(SensorType::Map, 95);
	dut.updateState(1000, 0);
	EXPECT_FALSE(dut.allowInjection());

	// Below falling threshold, fuel restored
	Sensor::setMockValue(SensorType::Map, 79);
	dut.updateState(1000, 0);
	EXPECT_TRUE(dut.allowInjection());

	// SPECIAL CASE: threshold of 0 means never boost cut
	engineConfiguration->boostCutPressure = 0;
	Sensor::setMockValue(SensorType::Map, 500);
	dut.updateState(1000, 0);
	EXPECT_TRUE(dut.allowInjection());
}

TEST(limp, BoostFuelCutRemainsActiveWhenMapSensorFails) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->boostCutPressure = 180;
	engineConfiguration->boostCutPressureHyst = 20;
	Sensor::setMockValue(SensorType::Map, 200);
	engine->rpmCalculator.setRpmValue(2000);
	auto* limp = getLimpManager();
	limp->updateState(2000, getTimeNowNt());
	ASSERT_EQ(ClearReason::BoostCut, limp->allowInjection().reason);

	Sensor::setInvalidMockValue(SensorType::Map);
	ASSERT_FALSE(Sensor::get(SensorType::Map));
	limp->updateState(2000, getTimeNowNt());
	EXPECT_EQ(ClearReason::BoostCut, limp->allowInjection().reason);

	Sensor::setMockValue(SensorType::Map, 150);
	limp->updateState(2000, getTimeNowNt());
	EXPECT_TRUE(limp->allowInjection());
}

TEST(limp, boostCutUint8Overflow) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Cut above 1500kPa
	engineConfiguration->boostCutPressure = 1500;
	engineConfiguration->boostCutPressureHyst = 20;

	LimpManager dut;

	// Below threshold, injection allowed
	Sensor::setMockValue(SensorType::Map, 80);
	dut.updateState(1000, 0);
	EXPECT_TRUE(dut.allowInjection());

	// Above rising threshold, injection cut
	Sensor::setMockValue(SensorType::Map, 1600);
	dut.updateState(1000, 0);
	EXPECT_FALSE(dut.allowInjection());

	// Below rising threshold, but should have hysteresis, so not cut yet
	Sensor::setMockValue(SensorType::Map, 1495);
	dut.updateState(1000, 0);
	EXPECT_FALSE(dut.allowInjection());

	// Below falling threshold, fuel restored
	Sensor::setMockValue(SensorType::Map, 79);
	dut.updateState(1000, 0);
	EXPECT_TRUE(dut.allowInjection());
}


TEST(limp, oilPressureStartupFailureCase) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->minOilPressureAfterStart = 200;

	LimpManager dut;

	// Low oil pressure!
	Sensor::setMockValue(SensorType::OilPressure, 50);

	// Start the engine
	engine->rpmCalculator.setRpmValue(1000);

	// update & check: injection should be allowed
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// 4.5 seconds later, should still be allowed (even though pressure is low)
	advanceTimeUs(4.5e6);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// 1 second later (5.5 since start), injection should cut
	advanceTimeUs(1.0e6);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_FALSE(dut.allowInjection());

	// But then oil pressure arrives!
	// Injection still isn't allowed, since now we're late.
	Sensor::setMockValue(SensorType::OilPressure, 250);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_FALSE(dut.allowInjection());
}

TEST(limp, oilPressureStartupEnableProtect) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Configure "After Start" protection
	engineConfiguration->minOilPressureAfterStart = 200;

	// explicitly set it to false.
	engineConfiguration->enableOilPressureProtect = false;

	LimpManager dut;

	// Low oil pressure!
	Sensor::setMockValue(SensorType::OilPressure, 50);

	// Start the engine
	engine->rpmCalculator.setRpmValue(1000);

	// update & check: injection should be allowed
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	advanceTimeUs(5.5e6);
	dut.updateState(1000, getTimeNowNt());

	// It SHOULD stay TRUE because enableOilPressureProtect is false.
	EXPECT_FALSE(dut.allowInjection());
}

TEST(limp, oilPressureStartupSuccessCase) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->enableOilPressureProtect = true;
	engineConfiguration->minOilPressureAfterStart = 200;

	LimpManager dut;

	// Low oil pressure!
	Sensor::setMockValue(SensorType::OilPressure, 50);

	// Start the engine
	engine->rpmCalculator.setRpmValue(1000);

	// update & check: injection should be allowed
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// 4.5 seconds later, should still be allowed (even though pressure is low)
	advanceTimeUs(4.5e6);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// But then oil pressure arrives!
	Sensor::setMockValue(SensorType::OilPressure, 250);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());

	// 1 second later (5.5 since start), injection should be allowed since we saw pressure before the timeout
	advanceTimeUs(1.0e6);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());

	// Later, we lose oil pressure, but engine should stay running
	advanceTimeUs(10e6);
	Sensor::setMockValue(SensorType::OilPressure, 10);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());
}

TEST(limp, oilPressureRunning) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->enableOilPressureProtect = true;
	engineConfiguration->minimumOilPressureTimeout = 1.0f;
	setArrayValues(config->minimumOilPressureValues, 100);

	LimpManager dut;

	// Oil pressure starts OK
	Sensor::setMockValue(SensorType::OilPressure, 110);

	// Start the engine
	engine->rpmCalculator.setRpmValue(1000);

	// update & check: injection should be allowed
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// A long time later, everything should still be OK
	advanceTimeUs(60e6);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// Now oil pressure drops below threshold
	Sensor::setMockValue(SensorType::OilPressure, 90);

	// 0.9 second later, injection should continue as timeout isn't hit yet
	advanceTimeUs(0.9e6);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());

	// 0.2 second later (1.1s since low pressure starts), injection should cut
	advanceTimeUs(1.0e6);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_FALSE(dut.allowInjection());

	// Oil pressure is restored, and fuel should be restored too
	Sensor::setMockValue(SensorType::OilPressure, 110);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());
}

// https://github.com/rusefi/rusefi/issues/9879
// Cold start: oil pressure is above "Minimum oil pressure after start" but still
// below the running table. The running check waits for the 5 second
// after-start window to end, then applies its own timeout.
TEST(limp, oilPressureRunningCheckDuringAfterStartWindow) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->minOilPressureAfterStart = 50;
	engineConfiguration->enableOilPressureProtect = true;
	engineConfiguration->minimumOilPressureTimeout = 1.0f;
	setArrayValues(config->minimumOilPressureValues, 75);

	LimpManager dut;

	Sensor::setMockValue(SensorType::OilPressure, 60);

	// Engine stopped
	engine->rpmCalculator.setRpmValue(0);
	dut.updateState(0, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// Start the engine
	engine->rpmCalculator.setRpmValue(1000);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	advanceTimeUs(0.9e6);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// 2 seconds after start, still inside the after-start window
	advanceTimeUs(1.1e6);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// 4.9 seconds after start: last update inside the window
	advanceTimeUs(2.9e6);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// 5.8 seconds after start: window is over, running timeout not reached yet
	advanceTimeUs(0.9e6);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// 6.1 seconds after start: still below running table, fuel is cut
	advanceTimeUs(0.3e6);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_FALSE(dut.allowInjection());
	EXPECT_EQ(ClearReason::OilPressure, dut.allowInjection().reason);
}

TEST(limp, oilPressureMaxLimit) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	config->maximumOilPressureValues[0] = 200; // kPa
	config->maximumOilPressureValues[1] = 200;
	config->maximumOilPressureValues[2] = 800;
	config->maximumOilPressureValues[3] = 800;
	setRpmTableBin(config->maximumOilPressureBins); // 700 800 3800 7000

	engineConfiguration->maxOilPressureTimeout = 1.0f; // sec

	LimpManager dut;

	// Oil pressure starts OK
	Sensor::setMockValue(SensorType::OilPressure, 300);

	// Start the engine
	float rpm = 4000;
	engine->rpmCalculator.setRpmValue(rpm);

	// update & check: injection should be allowed
	dut.updateState(rpm, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// after 5 sec - still allowed
	advanceTimeUs(5.0e6);
	dut.updateState(rpm, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// On lower RPMs oil pressure goes above threshold
	rpm = 800;
	engine->rpmCalculator.setRpmValue(rpm);

	// 0.9 second later, injection should continue as timeout isn't hit yet
	advanceTimeUs(0.9e6);
	dut.updateState(rpm, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());

	// 0.2 second later (1.1s since non-valid pressure), injection should cut
	advanceTimeUs(1.0e6);
	dut.updateState(rpm, getTimeNowNt());
	ASSERT_FALSE(dut.allowInjection());

	// Oil pressure is restored, and fuel should be restored too
	Sensor::setMockValue(SensorType::OilPressure, 110);
	dut.updateState(rpm, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());
}

TEST(limp, highOilPressureTimeoutAlreadyExpiredAtEngineStart) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->minOilPressureAfterStart = 0;
	engineConfiguration->enableOilPressureProtect = false;
	engineConfiguration->maxOilPressureTimeout = 1;
	setArrayValues(config->maximumOilPressureValues, 500);
	LimpManager dut;
	Sensor::setMockValue(SensorType::OilPressure, 0);
	engine->rpmCalculator.setRpmValue(0);
	dut.updateState(0, getTimeNowNt());
	advanceTimeUs(10000000);
	dut.updateState(0, getTimeNowNt());

	// The first running sample is high, but no running timeout has elapsed yet.
	Sensor::setMockValue(SensorType::OilPressure, 600);
	engine->rpmCalculator.setRpmValue(1000);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// Once a healthy running sample resets the timer, its configured delay works.
	Sensor::setMockValue(SensorType::OilPressure, 400);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());
	Sensor::setMockValue(SensorType::OilPressure, 600);
	advanceTimeUs(900000);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());
	advanceTimeUs(200000);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_EQ(ClearReason::OilPressure, dut.allowInjection().reason);
}

TEST(limp, highOilPressureTimeoutStartsAtFirstRunningSample) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->minOilPressureAfterStart = 0;
	engineConfiguration->enableOilPressureProtect = false;
	engineConfiguration->maxOilPressureTimeout = 1;
	setArrayValues(config->maximumOilPressureValues, 500);
	Sensor::setMockValue(SensorType::OilPressure, 600);
	engine->rpmCalculator.setRpmValue(1000);
	LimpManager dut;

	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());
	advanceTimeUs(900000);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());
	advanceTimeUs(200000);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_EQ(ClearReason::OilPressure, dut.allowInjection().reason);
}


TEST(limp, gdiFuelCut) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// configure GDI engine for tests:
	engineConfiguration->hpfpCam = HPFP_CAM_NONE;
	engineConfiguration->hpfpCamLobes = 4;
	engineConfiguration->mc33_t_hold_tot = 3.0f;
	engine->rpmCalculator.setRpmValue(1000);

	// below limits:
	engine->engineState.injectionDuration = 1.8;

	LimpManager dut;

	// update & check: injection should be allowed
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	engine->engineState.injectionDuration = 3.1f;
	// update & check: injection should cut
	dut.updateState(1000, getTimeNowNt());
	ASSERT_EQ(ClearReason::GdiLimits, dut.allowInjection().reason);
}

static void assertNoFiringUntilCamSync(trigger_type_e triggerType, const char* context) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// TEST_ENGINE defaults to IM_ONE_COIL where a distributor makes crank phase ambiguity harmless
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	eth.setTriggerType(triggerType);

	// A symmetrical crank wheel repeats several times per engine cycle, so crank phase
	// is ambiguous (worse than the 360 degrees wasted spark can tolerate) until cam sync arrives
	ASSERT_FALSE(engine->triggerCentral.triggerState.hasSynchronizedPhase()) << context;

	LimpManager dut;
	dut.updateState(1000, getTimeNowNt());

	EXPECT_EQ(ClearReason::EnginePhase, dut.allowInjection().reason) << context;
	EXPECT_EQ(ClearReason::EnginePhase, dut.allowIgnition().reason) << context;
}

TEST(limp, noFiringUntilCamSyncOnSymmetricalCrank) {
	assertNoFiringUntilCamSync(trigger_type_e::TT_3_TOOTH_CRANK, "3 tooth crank");
	assertNoFiringUntilCamSync(trigger_type_e::TT_6_TOOTH_CRANK, "6 tooth crank");
	assertNoFiringUntilCamSync(trigger_type_e::TT_12_TOOTH_CRANK, "12 tooth crank");
}

struct Mockhpfp : public MockHpfpController {
	bool isHpfpActive;
	angle_t m_deadangle;
};

static void checkCutWarningInterval(EngineTestHelper& eth, LimpManager& dut, ObdCode code) {
	ASSERT_EQ(1, eth.getWarningCounter());
	auto* warning = eth.recentWarnings()->find(code);
	ASSERT_NE(nullptr, warning);
	const auto firstReport = warning->LastTriggered.get();
	EXPECT_EQ(getTimeNowNt(), firstReport);

	// warningCounter counts stored codes; timestamps track repeated reports.
	advanceTimeUs(1);
	dut.updateState(2000, getTimeNowNt());
	EXPECT_EQ(firstReport, warning->LastTriggered.get());
	advanceTimeUs(29999998);
	dut.updateState(2000, getTimeNowNt());
	EXPECT_EQ(firstReport, warning->LastTriggered.get());

	// The boundary is inclusive, with no early report from float rounding.
	advanceTimeUs(1);
	dut.updateState(2000, getTimeNowNt());
	const auto secondReport = getTimeNowNt();
	EXPECT_EQ(secondReport, warning->LastTriggered.get());
	EXPECT_EQ(code, engine->engineState.warnings.lastErrorCode);
	advanceTimeUs(1);
	dut.updateState(2000, getTimeNowNt());
	EXPECT_EQ(secondReport, warning->LastTriggered.get());

	// Still report after an interval longer than UINT32_MAX native ticks.
	advanceTimeUs(60000000);
	dut.updateState(2000, getTimeNowNt());
	EXPECT_EQ(getTimeNowNt(), warning->LastTriggered.get());
	EXPECT_EQ(1, eth.getWarningCounter());
}

TEST(limp, boostCutWarning) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LimpManager dut;
	engineConfiguration->boostCutPressure = 100;
	Sensor::setMockValue(SensorType::Map, 110);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_EQ(ClearReason::BoostCut, dut.allowInjection().reason);
	checkCutWarningInterval(eth, dut, ObdCode::OBD_Engine_Overboost);
}

TEST(limp, lambdaCutWarning) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LimpManager dut;
	engine->lambdaMonitor.lambdaMonitorCut = true;
	dut.updateState(1000, getTimeNowNt());
	ASSERT_EQ(ClearReason::LambdaProtection, dut.allowInjection().reason);
	checkCutWarningInterval(eth, dut, ObdCode::OBD_System_Too_Lean_Bank_1);
}

TEST(limp, etbCutWarning) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LimpManager dut;
	dut.reportEtbJammed();
	dut.updateState(2000, getTimeNowNt());
	ASSERT_FALSE(dut.allowElectronicThrottle());
	ASSERT_EQ(ClearReason::EtbJammedRevLimit, dut.allowInjection().reason);
	checkCutWarningInterval(eth, dut, ObdCode::OBD_Throttle_Actuator_Control_Range_Performance_Bank_1);
}

TEST(limp, injectorDutyCutWarning) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LimpManager dut;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->maxInjectorDutyInstant = 90;
	engine->engineState.injectionDuration = 120;
	dut.updateState(1000, getTimeNowNt());
	ASSERT_EQ(ClearReason::InjectorDutyCycle, dut.allowInjection().reason);
	checkCutWarningInterval(eth, dut, ObdCode::CUSTOM_TOO_LONG_FUEL_INJECTION);
}

TEST(limp, cutWarningsShareTimer) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LimpManager dut;
	engineConfiguration->boostCutPressure = 100;
	engineConfiguration->boostCutPressureHyst = 10;
	Sensor::setMockValue(SensorType::Map, 80);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());
	ASSERT_EQ(0, eth.getWarningCounter());

	Sensor::setMockValue(SensorType::Map, 110);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_EQ(1, eth.getWarningCounter());
	auto* boostWarning = eth.recentWarnings()->find(ObdCode::OBD_Engine_Overboost);
	ASSERT_NE(nullptr, boostWarning);
	const auto firstReport = boostWarning->LastTriggered.get();

	// Recovery and a different cut must not reset the reporting interval.
	advanceTimeUs(1000000);
	Sensor::setMockValue(SensorType::Map, 80);
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->maxInjectorDutyInstant = 90;
	engine->engineState.injectionDuration = 120;
	dut.updateState(1000, getTimeNowNt());
	EXPECT_EQ(ClearReason::InjectorDutyCycle, dut.allowInjection().reason);
	EXPECT_EQ(1, eth.getWarningCounter());

	advanceTimeUs(1000000);
	engine->engineState.injectionDuration = 0;
	engine->lambdaMonitor.lambdaMonitorCut = true;
	dut.updateState(1000, getTimeNowNt());
	EXPECT_EQ(ClearReason::LambdaProtection, dut.allowInjection().reason);
	EXPECT_EQ(1, eth.getWarningCounter());

	advanceTimeUs(1000000);
	engine->lambdaMonitor.lambdaMonitorCut = false;
	dut.reportEtbJammed();
	dut.updateState(2000, getTimeNowNt());
	EXPECT_FALSE(dut.allowElectronicThrottle());
	EXPECT_EQ(ClearReason::EtbJammedRevLimit, dut.allowInjection().reason);
	EXPECT_EQ(1, eth.getWarningCounter());

	// Even simultaneous faults share the same interval.
	Sensor::setMockValue(SensorType::Map, 110);
	engine->engineState.injectionDuration = 120;
	engine->lambdaMonitor.lambdaMonitorCut = true;

	// One jam report is enough: the latched fault gets reported when eligible.
	advanceTimeUs(26999999);
	dut.updateState(2000, getTimeNowNt());
	EXPECT_EQ(1, eth.getWarningCounter());
	advanceTimeUs(1);
	dut.updateState(2000, getTimeNowNt());
	EXPECT_EQ(2, eth.getWarningCounter());
	auto* etbWarning = eth.recentWarnings()->find(ObdCode::OBD_Throttle_Actuator_Control_Range_Performance_Bank_1);
	ASSERT_NE(nullptr, etbWarning);
	EXPECT_EQ(getTimeNowNt(), etbWarning->LastTriggered.get());
	EXPECT_EQ(firstReport, boostWarning->LastTriggered.get());
	EXPECT_FALSE(hasRecentWarningCode(ObdCode::OBD_System_Too_Lean_Bank_1));
	EXPECT_FALSE(hasRecentWarningCode(ObdCode::CUSTOM_TOO_LONG_FUEL_INJECTION));
}

TEST(limp, recoveredCutDoesNotReportAgain) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LimpManager dut;
	engineConfiguration->boostCutPressure = 100;
	engineConfiguration->boostCutPressureHyst = 10;
	Sensor::setMockValue(SensorType::Map, 110);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_EQ(1, eth.getWarningCounter());
	auto* warning = eth.recentWarnings()->find(ObdCode::OBD_Engine_Overboost);
	ASSERT_NE(nullptr, warning);
	const auto firstReport = warning->LastTriggered.get();

	Sensor::setMockValue(SensorType::Map, 80);
	advanceTimeUs(30000000);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());
	EXPECT_EQ(firstReport, warning->LastTriggered.get());

	// A healthy update does not consume the next interval.
	Sensor::setMockValue(SensorType::Map, 110);
	dut.updateState(1000, getTimeNowNt());
	EXPECT_EQ(ClearReason::BoostCut, dut.allowInjection().reason);
	EXPECT_EQ(getTimeNowNt(), warning->LastTriggered.get());
	EXPECT_EQ(1, eth.getWarningCounter());
}
