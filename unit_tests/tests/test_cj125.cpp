/*
 * test_cj125.cpp
 *
 *  Created on: Jan 3, 2019
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "gtest/gtest.h"
#include "CJ125_logic.h"
#include "engine_test_helper.h"

TEST(testCJ125, testInitialState) {
	CJ125 cj;

	ASSERT_EQ(cj.state, CJ125_INIT);
	ASSERT_FALSE(cj.isWorkingState());

	WITH_ENGINE_TEST_HELPER(FORD_ASPIRE_1996);
	ASSERT_EQ(engine->sensors.vBatt, 0);

}


