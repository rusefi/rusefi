/*
 * test_startOfCrankingPrimingPulse.cpp
 *
 *  Created on: Mar 4, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "engine_test_helper.h"

TEST(engine, testPlainCrankingWithoutAdvancedFeatures) {
	printf("*************************************************** testPlainCrankingWithoutAdvancedFeatures\r\n");

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	ASSERT_EQ( 0,  GET_RPM()) << "RPM=0";

	eth.fireTriggerEventsWithDuration(/* durationMs */ 200);
	// still no RPM since need to cycles measure cycle duration
	ASSERT_EQ( 0,  GET_RPM()) << "start-RPM#1";


	eth.fireRise(/* delayMs */ 200);
	ASSERT_EQ( 300,  GET_RPM()) << "RPM#2";
	// two simultaneous injections
	ASSERT_EQ( 4,  engine->executor.size()) << "plain#2";

	eth.assertEvent5(&engine->executor, "sim start", 0, (void*)startSimultaniousInjection, eth.getTimeNowUs(), 100000 - 1875);
	eth.assertEvent5(&engine->executor, "sim end", 1, (void*)endSimultaniousInjection, eth.getTimeNowUs(), 100000);
}


TEST(engine, testStartOfCrankingPrimingPulse) {
	printf("*************************************************** testStartOfCrankingPrimingPulse\r\n");

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	engineConfiguration->startOfCrankingPrimingPulse = 4;

	ASSERT_EQ( 0,  GET_RPM()) << "RPM=0";

	// this -70 value comes from CLT error handling code
	ASSERT_NEAR( 70,  engine->sensors.clt, EPS4D) << "CLT#1";

	// we need below freezing temperature to get prime fuel
	// todo: less cruel CLT value assignment which would survive 'updateSlowSensors'
	engine->sensors.clt = -10;

	// prod code invokes this on ECU start, here we have to mimic this behavior
	startPrimeInjectionPulse(PASS_ENGINE_PARAMETER_SIGNATURE);


	ASSERT_EQ( 1,  engine->executor.size()) << "prime fuel";
}

