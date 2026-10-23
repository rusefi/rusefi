/**
 * @file test_sensor_checker.cpp
 */

#include "pch.h"

// The settle window must be measured from the moment ignition is turned on:
// a key that was off for a long time used to get no settle time at all.
TEST(SensorChecker, waitsAfterIgnitionOn) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	auto& sensorChecker = engine->module<SensorChecker>();

	sensorChecker->onIgnitionStateChanged(false);
	advanceTimeUs(10'000'000);
	sensorChecker->onSlowCallback();
	EXPECT_FALSE(sensorChecker->analogSensorsShouldWork());

	sensorChecker->onIgnitionStateChanged(true);
	sensorChecker->onSlowCallback();
	EXPECT_FALSE(sensorChecker->analogSensorsShouldWork());

	advanceTimeUs(4'900'000);
	sensorChecker->onSlowCallback();
	EXPECT_FALSE(sensorChecker->analogSensorsShouldWork());

	advanceTimeUs(200'000);
	sensorChecker->onSlowCallback();
	EXPECT_TRUE(sensorChecker->analogSensorsShouldWork());
}
