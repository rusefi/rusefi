#include "pch.h"

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
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->rpmHardLimit = 2500;

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

TEST(limp, boostCut) {
	EngineTestHelper eth(TEST_ENGINE);

	// Cut above 100kPa
	engineConfiguration->boostCutPressure = 100;

	LimpManager dut;

	// Below threshold, injection allowed
	Sensor::setMockValue(SensorType::Map, 80);
	dut.updateState(1000, 0);
	EXPECT_TRUE(dut.allowInjection());

	// Above threshold, injection cut
	Sensor::setMockValue(SensorType::Map, 120);
	dut.updateState(1000, 0);
	EXPECT_FALSE(dut.allowInjection());

	// Below threshold, should recover
	Sensor::setMockValue(SensorType::Map, 80);
	dut.updateState(1000, 0);
	EXPECT_TRUE(dut.allowInjection());

	// SPECIAL CASE: threshold of 0 means never boost cut
	engineConfiguration->boostCutPressure = 0;
	Sensor::setMockValue(SensorType::Map, 500);
	dut.updateState(1000, 0);
	EXPECT_TRUE(dut.allowInjection());
}

extern int timeNowUs;

TEST(limp, oilPressureFailureCase) {
	EngineTestHelper eth(TEST_ENGINE);
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
	timeNowUs += 4.5e6;
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// 1 second later (5.5 since start), injection should cut
	timeNowUs += 1.0e6;
	dut.updateState(1000, getTimeNowNt());
	ASSERT_FALSE(dut.allowInjection());

	// But then oil pressure arrives!
	// Injection still isn't allowed, since now we're late.
	Sensor::setMockValue(SensorType::OilPressure, 250);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_FALSE(dut.allowInjection());
}

TEST(limp, oilPressureSuccessCase) {
	EngineTestHelper eth(TEST_ENGINE);
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
	timeNowUs += 4.5e6;
	dut.updateState(1000, getTimeNowNt());
	EXPECT_TRUE(dut.allowInjection());

	// But then oil pressure arrives!
	Sensor::setMockValue(SensorType::OilPressure, 250);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());

	// 1 second later (5.5 since start), injection should be allowed since we saw pressure before the timeout
	timeNowUs += 1.0e6;
	dut.updateState(1000, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());

	// Later, we lose oil pressure, but engine should stay running
	timeNowUs += 10e6;
	Sensor::setMockValue(SensorType::OilPressure, 10);
	dut.updateState(1000, getTimeNowNt());
	ASSERT_TRUE(dut.allowInjection());
}
