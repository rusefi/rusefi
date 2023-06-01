/*
 * @file test_rpm_multiplier.cpp
 *
 * Four different cases of operation_mode_e and getRpmMultiplier
 *
 * @date Jun 26, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

static void runRpmTest(bool isTwoStroke, bool isCam, int expected) {
	EngineTestHelper eth(TEST_ENGINE);
	engineConfiguration->twoStroke = isTwoStroke;
	engineConfiguration->skippedWheelOnCam = isCam;
	eth.setTriggerType(trigger_type_e::TT_ONE);

	eth.smartFireTriggerEvents2(/*count*/200, /*delay*/ 40);
	ASSERT_EQ(expected, Sensor::getOrZero(SensorType::Rpm));
}

// todo: google test profiles one day?

TEST(engine, testRpmOfCamSensor) {
	runRpmTest(false, true, 1500);
}

TEST(engine, testRpmOfTwoStroke) {
	runRpmTest(true, false, 750);
}

TEST(engine, testRpmOfCrankOnly) {
	runRpmTest(false, false, 750);
}

