/*
 * test_startOfCrankingPrimingPulse.cpp
 *
 *  Created on: Mar 4, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

TEST(engine, testPlainCrankingWithoutAdvancedFeatures) {
	EngineTestHelper eth(TEST_ENGINE);

	engine->tdcMarkEnabled = false;
	engineConfiguration->cranking.baseFuel = 12;

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	ASSERT_EQ( 0,  GET_RPM()) << "RPM=0";

	eth.fireTriggerEventsWithDuration(/* durationMs */ 200);
	// still no RPM since need to cycles measure cycle duration
	ASSERT_EQ( 0,  GET_RPM()) << "start-RPM#1";


	eth.fireRise(/* delayMs */ 200);
	ASSERT_EQ( 300,  GET_RPM()) << "RPM#2";
	// two simultaneous injections
	ASSERT_EQ( 4,  engine->executor.size()) << "plain#2";

	eth.assertEvent5("sim start", 0, (void*)startSimultaniousInjection, 100000 - 1625);
	// -1 because ugh floating point math
	eth.assertEvent5("sim end", 1, (void*)endSimultaniousInjection, 100000 - 1);
}


TEST(engine, testStartOfCrankingPrimingPulse) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->startOfCrankingPrimingPulse = 4;

	ASSERT_EQ( 0,  GET_RPM()) << "RPM=0";

	// we need below freezing temperature to get prime fuel
	Sensor::setMockValue(SensorType::Clt, -10);

	// prod code invokes this on ECU start, here we have to mimic this behavior
	startPrimeInjectionPulse();


	ASSERT_EQ( 1,  engine->executor.size()) << "prime fuel";
}

