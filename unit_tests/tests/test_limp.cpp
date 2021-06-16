#include "limp_manager.h"
#include "engine_test_helper.h"
#include <gtest/gtest.h>

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
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	engineConfiguration->rpmHardLimit = 2500;

	LimpManager dut;
	INJECT_ENGINE_REFERENCE(&dut);

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
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// Cut above 100kPa
	engineConfiguration->boostCutPressure = 100;

	LimpManager dut;
	INJECT_ENGINE_REFERENCE(&dut);

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
