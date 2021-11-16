/*
 * @file	test_start_stop.cpp
 *
 * @date Nov 18, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
*/

#include "pch.h"

TEST(start, startStop) {
	std::unordered_map<SensorType, float> sensorVals = {{ SensorType::AcceleratorPedal, 0 }};
	EngineTestHelper eth(PROTEUS_BMW_M73, sensorVals);
	eth.moveTimeForwardAndInvokeEventsSec(1); // '0' time has special meaning for implementation so let's move forward

	// this is a pull-up, so 'true' on start-up
	setMockState(engineConfiguration->startStopButtonPin, true);

	ASSERT_FALSE(efiReadPin(engineConfiguration->starterControlPin));

	slowStartStopButtonCallback();
	ASSERT_FALSE(efiReadPin(engineConfiguration->starterControlPin));

	// startup 'timeout' duration of time is a special case so let's sleep a bit
	eth.moveTimeForwardAndInvokeEventsSec(10);
	slowStartStopButtonCallback();
	ASSERT_FALSE(efiReadPin(engineConfiguration->starterControlPin));



	eth.moveTimeForwardAndInvokeEventsSec(10);
	// hit 'start' button! inverted since pull-up
	setMockState(engineConfiguration->startStopButtonPin, false);
	slowStartStopButtonCallback();
	ASSERT_TRUE(efiReadPin(engineConfiguration->starterControlPin));

	eth.moveTimeForwardAndInvokeEventsSec(5);
	slowStartStopButtonCallback();

	eth.moveTimeForwardAndInvokeEventsSec(5);
	slowStartStopButtonCallback();

	eth.moveTimeForwardAndInvokeEventsSec(5);
	slowStartStopButtonCallback();
	// starter is now OFF due to timeout
	ASSERT_FALSE(efiReadPin(engineConfiguration->starterControlPin));
}
