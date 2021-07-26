/*
 * @file test_rpm_multiplier.cpp
 *
 * Four different cases of operation_mode_e and getRpmMultiplier
 *
 * @date Jun 26, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

static void runRpmTest(operation_mode_e mode, int expected) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	engineConfiguration->ambiguousOperationMode = mode;
	eth.setTriggerType(TT_ONE PASS_ENGINE_PARAMETER_SUFFIX);

	eth.smartFireTriggerEvents2(/*count*/200, /*delay*/ 40);
	ASSERT_EQ(expected, GET_RPM());
}

// todo: google test profiles one day?

TEST(engine, testRpmOfCamSensor) {
	runRpmTest(FOUR_STROKE_CAM_SENSOR, 1500);
}

TEST(engine, testRpmOfSymmetricalCrank) {
	runRpmTest(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR, 375);
}

TEST(engine, testRpmOfTwoStroke) {
	runRpmTest(TWO_STROKE, 750);
}

TEST(engine, testRpmOfCrankOnly) {
	runRpmTest(FOUR_STROKE_CRANK_SENSOR, 750);
}

TEST(engine, testRpmOfThreeTimesCrank) {
	runRpmTest(FOUR_STROKE_THREE_TIMES_CRANK_SENSOR, 250);
}
