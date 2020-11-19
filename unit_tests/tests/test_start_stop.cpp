/*
 * @file	test_start_stop.cpp
 *
 * @date Nov 18, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
*/

#include "engine_test_helper.h"
#include "engine_controller.h"

TEST(start, startStop) {
	WITH_ENGINE_TEST_HELPER(BMW_M73_PROTEUS);
	// this is a pull-up, so 'true' on start-up
	setMockState(engineConfiguration->startStopButtonPin, true);

	ASSERT_EQ(efiReadPin(engineConfiguration->starterControlPin), false);

	slowStartStopButtonCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ(efiReadPin(engineConfiguration->starterControlPin), false);


	eth.smartMoveTimeForwardSeconds(10);
	// inverted since pull-up
	setMockState(engineConfiguration->startStopButtonPin, false);
	slowStartStopButtonCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ(efiReadPin(engineConfiguration->starterControlPin), true);

}
