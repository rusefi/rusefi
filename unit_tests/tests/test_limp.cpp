#include "pch.h"

#include "fuel_math.h"
#include "limp_manager.h"

TEST(limp, testFatalError) {
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
	EngineTestHelper eth(engine_type_e::FORD_ASPIRE_1996);

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

TEST(limp, oilPressureStartupSuccessCase) {
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
