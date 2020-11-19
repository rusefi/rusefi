#include "engine_test_helper.h"
#include "launch_control.h"

#include <gtest/gtest.h>

TEST(LaunchControl, TpsCondition) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	LaunchControlBase dut;
	INJECT_ENGINE_REFERENCE(&dut);

	engineConfiguration->launchTpsTreshold = 10;

	// Should return false with failed sensor
	Sensor::resetMockValue(SensorType::DriverThrottleIntent);
	EXPECT_FALSE(dut.isInsideTpsCondition());

	// Should return false when throttle is closed
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 5.0f);
	EXPECT_FALSE(dut.isInsideTpsCondition());
	
	// Should return true when throttle is opened past the threshold
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 20.0f);
	EXPECT_TRUE(dut.isInsideTpsCondition());
}
