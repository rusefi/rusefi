/*
 * test_startOfCrankingPrimingPulse.cpp
 *
 *  Created on: Mar 4, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "test_trigger_decoder.h"

extern int timeNowUs;
extern EnginePins enginePins;

TEST(engine, testPlainCrankingWithoutAdvancedFeatures) {
	// this is just a reference unit test implementation
	printf("*************************************************** testPlainCrankingWithoutAdvancedFeatures\r\n");

	EngineTestHelper eth(TEST_ENGINE);
	EXPAND_EngineTestHelper

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	assertEqualsM("RPM=0", 0, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));

	eth.fireTriggerEventsWithDuration(/* durationMs */ 200);
	// still no RPM since need to cycles measure cycle duration
	assertEqualsM("RPM#1", 0, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));


	eth.fireRise(/* delayMs */ 200);
	assertEqualsM("RPM#2", 300, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));
	// two simultaneous injections
	assertEqualsM("plain#2", 4, engine->executor.size());

	eth.assertEvent5(&engine->executor, "sim start", 0, (void*)startSimultaniousInjection, timeNowUs, 97975);
	eth.assertEvent5(&engine->executor, "sim end", 1, (void*)endSimultaniousInjection, timeNowUs, 100000);
}


TEST(engine, testStartOfCrankingPrimingPulse) {
	printf("*************************************************** testStartOfCrankingPrimingPulse\r\n");

	EngineTestHelper eth(TEST_ENGINE);
	EXPAND_EngineTestHelper

	engineConfiguration->startOfCrankingPrimingPulse = 4;

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	assertEqualsM("RPM=0", 0, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));

	// this -70 value comes from CLT error handling code
	assertEqualsM("CLT#1", 70, engine->sensors.clt);

	// we need below freezing temperature to get prime fuel
	// todo: less cruel CLT value assignment which would survive 'updateSlowSensors'
	engine->sensors.clt = -10;

	// prod code invokes this on ECU start, here we have to mimic this behavior
	startPrimeInjectionPulse(PASS_ENGINE_PARAMETER_SIGNATURE);


	assertEqualsM("prime fuel", 1, engine->executor.size());
}

