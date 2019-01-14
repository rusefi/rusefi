/*
 * test_fasterEngineSpinningUp.cpp
 *
 *  Created on: Mar 6, 2018
 */

#include "engine_math.h"
#include "test_trigger_decoder.h"

extern int timeNowUs;

TEST(sensors, testFasterEngineSpinningUp) {
	printf("*************************************************** testFasterEngineSpinningUp\r\n");

	EngineTestHelper eth(TEST_ENGINE);
	EXPAND_EngineTestHelper

	// turn on FasterEngineSpinUp mode
	engineConfiguration->bc.isFasterEngineSpinUpEnabled = true;
	// set ignition mode
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	// set cranking threshold (used below)
	engineConfiguration->cranking.rpm = 999;
	// set sequential injection mode to test auto-change to simultaneous when spinning-up
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth, IM_SEQUENTIAL);

	// check if it's true
	ASSERT_EQ(IM_SEQUENTIAL, engine->getCurrentInjectionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	ASSERT_EQ(IM_INDIVIDUAL_COILS, getIgnitionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	// check if the engine has the right state
	ASSERT_EQ(STOPPED, engine->rpmCalculator.getState());
	// check RPM
	assertEqualsM("RPM=0", 0, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));
	// the queue should be empty, no trigger events yet
	ASSERT_EQ(0, engine->executor.size()) << "plain#1";

	// check all events starting from now
	int timeStartUs = eth.getTimeNowUs();
	// advance 1 revolution
	// because we have trivial TT_ONE trigger here synchronization would happen with just one rise front
	eth.fireRise(200);

	// check if the mode is changed
	ASSERT_EQ(SPINNING_UP, engine->rpmCalculator.getState());
	// due to isFasterEngineSpinUp=true, we should have already detected RPM!
	assertEqualsM("spinning-RPM#1", 300, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));
	// two simultaneous injections
	ASSERT_EQ(4, engine->executor.size()) << "plain#2";
	// test if they are simultaneous
	ASSERT_EQ(IM_SIMULTANEOUS, engine->getCurrentInjectionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	// test if ignition mode is temporary changed to wasted spark, if set to ind.coils
	ASSERT_EQ(IM_WASTED_SPARK, getIgnitionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	// check real events
	eth.assertEvent5(&engine->executor, "inj start#1", 0, (void*)startSimultaniousInjection, timeStartUs, MS2US(200) + 97975);
	eth.assertEvent5(&engine->executor, "inj end#1", 1, (void*)endSimultaniousInjection, timeStartUs, MS2US(200) + 100000);

	// skip the rest of the cycle
	eth.fireFall(200);

	// now clear and advance more
	eth.clearQueue();

	eth.fireRise(200);

	// check if the mode is changed when fully synched
	ASSERT_EQ(CRANKING, engine->rpmCalculator.getState());
	// check RPM
	assertEqualsM("RPM#2", 300, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));
	// test if they are simultaneous in cranking mode too
	ASSERT_EQ(IM_SIMULTANEOUS, engine->getCurrentInjectionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	// test if ignition mode is restored to ind.coils
	ASSERT_EQ(IM_INDIVIDUAL_COILS, getIgnitionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	// two simultaneous injections
	assertEqualsM("plain#2", 4, engine->executor.size());
	// check real events
	eth.assertEvent5(&engine->executor, "inj start#2", 0, (void*)startSimultaniousInjection, eth.getTimeNowUs(), 97975);
	eth.assertEvent5(&engine->executor, "inj end#2", 1, (void*)endSimultaniousInjection, eth.getTimeNowUs(), 100000);

	// skip, clear & advance 1 more revolution at higher RPM
	eth.fireFall(60);

	eth.clearQueue();
	timeStartUs = timeNowUs;
	eth.fireTriggerEventsWithDuration(60);

	// check if the mode is now changed to 'running' at higher RPM
	ASSERT_EQ(RUNNING, engine->rpmCalculator.getState());
	// check RPM
	assertEqualsM("RPM#3", 1000, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));
	// check if the injection mode is back to sequential now
	ASSERT_EQ(IM_SEQUENTIAL, engine->getCurrentInjectionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	// 4 sequential injections for the full cycle
	assertEqualsM("plain#3", 8, engine->executor.size());

	// check real events for sequential injection
	// Note: See addFuelEvents() fix inside setRpmValue()!
	eth.assertEvent5(&engine->executor, "inj start#3", 0, (void*)seTurnPinHigh, timeStartUs, MS2US(60) + 27974);
	eth.assertEvent5(&engine->executor, "inj end#3", 1, (void*)seTurnPinLow, timeStartUs, MS2US(60) + 27974 + 3000);
}
