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
	setMockState(engineConfiguration->starterControlPin, true PASS_ENGINE_PARAMETER_SUFFIX);
}
