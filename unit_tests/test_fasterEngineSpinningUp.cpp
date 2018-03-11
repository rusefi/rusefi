/*
 * test_fasterEngineSpinningUp.cpp
 *
 *  Created on: Mar 6, 2018
 */

#include "engine_math.h"
#include "test_fasterEngineSpinningUp.h"
#include "test_trigger_decoder.h"
#include "event_queue.h"
#include "unit_test_framework.h"

extern EventQueue schedulingQueue;
extern int timeNowUs;
extern EnginePins enginePins;

void testFasterEngineSpinningUp() {
	// this is just a reference unit test implementation
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
	assertEquals(IM_SEQUENTIAL, engine->getCurrentInjectionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	assertEquals(IM_INDIVIDUAL_COILS, getIgnitionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	// check if the engine has the right state
	assertEquals(STOPPED, engine->rpmCalculator.getState());
	// check RPM
	assertEqualsM("RPM=0", 0, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));
	// the queue should be empty, no trigger events yet
	assertEqualsM("plain#1", 0, schedulingQueue.size());

	// check all events starting from now
	int timeStartUs = timeNowUs;
	// advance 1 revolution
	timeNowUs += MS2US(200);
	eth.firePrimaryTriggerRise();

	// check if the mode is changed
	assertEquals(SPINNING_UP, engine->rpmCalculator.getState());
	// due to isFasterEngineSpinUp=true, we should have already detected RPM!
	assertEqualsM("RPM#1", 300, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));
	// two simultaneous injections
	assertEqualsM("plain#1", 4, schedulingQueue.size());
	// test if they are simultaneous
	assertEquals(IM_SIMULTANEOUS, engine->getCurrentInjectionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	// test if ignition mode is temporary changed to wasted spark, if set to ind.coils
	assertEquals(IM_WASTED_SPARK, getIgnitionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	// check real events
	assertEvent5("inj start#1", 0, (void*)startSimultaniousInjection, timeStartUs, MS2US(200) + 97975);
	assertEvent5("inj end#1", 1, (void*)endSimultaniousInjection, timeStartUs, MS2US(200) + 100000);

	// skip the rest of the cycle
	timeNowUs += MS2US(200);
	eth.firePrimaryTriggerFall();

	// now clear and advance more
	eth.clearQueue();
	timeNowUs += MS2US(200);
	eth.firePrimaryTriggerRise();

	// check if the mode is changed when fully synched
	assertEquals(CRANKING, engine->rpmCalculator.getState());
	// check RPM
	assertEqualsM("RPM#2", 300, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));
	// test if they are simultaneous in cranking mode too
	assertEquals(IM_SIMULTANEOUS, engine->getCurrentInjectionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	// test if ignition mode is restored to ind.coils
	assertEquals(IM_INDIVIDUAL_COILS, getIgnitionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	// two simultaneous injections
	assertEqualsM("plain#2", 4, schedulingQueue.size());
	// check real events
	assertEvent5("inj start#2", 0, (void*)startSimultaniousInjection, timeNowUs, 97975);
	assertEvent5("inj end#2", 1, (void*)endSimultaniousInjection, timeNowUs, 100000);

	// skip, clear & advance 1 more revolution at higher RPM
	timeNowUs += MS2US(60);
	eth.firePrimaryTriggerFall();

	eth.clearQueue();
	timeStartUs = timeNowUs;
	eth.fireTriggerEvents2(1, MS2US(60));

	// check if the mode is now changed to 'running' at higher RPM
	assertEquals(RUNNING, engine->rpmCalculator.getState());
	// check RPM
	assertEqualsM("RPM#3", 1000, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));
	// check if the injection mode is back to sequential now
	assertEquals(IM_SEQUENTIAL, engine->getCurrentInjectionMode(PASS_ENGINE_PARAMETER_SIGNATURE));
	// 4 sequential injections for the full cycle
	assertEqualsM("plain#3", 8, schedulingQueue.size());

	// check real events for sequential injection
	// Note: See addFuelEvents() fix inside setRpmValue()!
	assertEvent5("inj start#3", 0, (void*)seTurnPinHigh, timeStartUs, MS2US(60) + 27974);
	assertEvent5("inj end#3", 1, (void*)seTurnPinLow, timeStartUs, MS2US(60) + 27974 + 3000);
}
