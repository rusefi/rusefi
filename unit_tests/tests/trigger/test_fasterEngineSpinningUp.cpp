/*
 * test_fasterEngineSpinningUp.cpp
 *
 *  Created on: Mar 6, 2018
 */

#include "pch.h"
#include "fuel_math.h"

TEST(cranking, testFasterEngineSpinningUp) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	extern bool unitTestBusyWaitHack;
	unitTestBusyWaitHack = true;
	float phase = 181;
	setTable(config->injectionPhase, -phase);
	engine->tdcMarkEnabled = false;
	// turn on FasterEngineSpinUp mode
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->cranking.baseFuel = 12;

	// set ignition mode
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	// set cranking threshold (used below)
	engineConfiguration->cranking.rpm = 999;
	// set sequential injection mode to test auto-change to simultaneous when spinning-up
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth, IM_SEQUENTIAL);
	// Lie that this trigger requires disambiguation
	engine->triggerCentral.triggerState.setNeedsDisambiguation(true);

	ASSERT_EQ(IM_WASTED_SPARK, getCurrentIgnitionMode());

	eth.fireRise(1000 /*ms*/);

	// check if it's true
	ASSERT_EQ(IM_SEQUENTIAL, getCurrentInjectionMode());
	ASSERT_EQ(IM_WASTED_SPARK, getCurrentIgnitionMode());
	// check if the engine has the right state
	ASSERT_EQ(SPINNING_UP, engine->rpmCalculator.getState());
	ASSERT_NEAR(0, getInjectionMass(200), EPS5D);
	// check RPM
	eth.assertRpm( 0, "RPM=0");
	// the queue should be empty, no trigger events yet
	ASSERT_EQ(0, engine->executor.size()) << "plain#1";

	// check all events starting from now
	// advance 1 revolution
	// because we have trivial TT_ONE trigger here synchronization would happen with just one rise front
	eth.fireRise(200);

	// check if the mode is changed
	ASSERT_EQ(SPINNING_UP, engine->rpmCalculator.getState());
	// due to isFasterEngineSpinUp=true, we should have already detected RPM!
	eth.assertRpm( 300, "spinning-RPM#1");
	// two simultaneous injections
	ASSERT_EQ(4, engine->executor.size()) << "plain#2";
	// test if they are simultaneous
	ASSERT_EQ(IM_SIMULTANEOUS, getCurrentInjectionMode());
	// test if ignition mode is temporary changed to wasted spark, if set to individual coils
	ASSERT_EQ(IM_WASTED_SPARK, getCurrentIgnitionMode());
	// check real events

	float expectedSimultaneousTimestamp = eth.angleToTimeUs(360 - phase);

	eth.assertEvent5("inj start#1", 0, (void*)startSimultaneousInjection, expectedSimultaneousTimestamp - MS2US(engine->engineState.injectionDuration));
	eth.assertEvent5("inj end#1", 1, (void*)endSimultaneousInjection, expectedSimultaneousTimestamp);

	// skip the rest of the cycle
	eth.moveTimeForwardUs(MS2US(200));

	// now clear and advance more
	eth.executeActions();

	eth.fireRise(200);

	// check if the mode is changed when fully synched
	ASSERT_EQ(CRANKING, engine->rpmCalculator.getState());
	// check RPM
	eth.assertRpm(200, "RPM#2");
	// test if they are simultaneous in cranking mode too
	ASSERT_EQ(IM_SIMULTANEOUS, getCurrentInjectionMode());
	// Should still be in wasted spark since we don't have cam sync yet
	ASSERT_EQ(IM_WASTED_SPARK, getCurrentIgnitionMode());
	// two simultaneous injections
	ASSERT_EQ( 4,  engine->executor.size()) << "plain#2";
	// check real events
	expectedSimultaneousTimestamp = eth.angleToTimeUs(360 - phase);

	eth.assertEvent5("inj start#2", 0, (void*)startSimultaneousInjection, expectedSimultaneousTimestamp - 1625);
	eth.assertEvent5("inj end#2", 1, (void*)endSimultaneousInjection, expectedSimultaneousTimestamp);

	// Now perform a fake VVT sync and check that ignition mode changes to sequential
	engine->triggerCentral.syncAndReport(2, 0);
	ASSERT_EQ(IM_SEQUENTIAL, getCurrentIgnitionMode());
	// still cranking fuel
	ASSERT_NEAR(0.0039, getInjectionMass(200), EPS3D);

	// skip, clear & advance 1 more revolution at higher RPM
	eth.fireFall(60);

	eth.clearQueue();
	eth.fireTriggerEventsWithDuration(60);

	// check if the mode is now changed to 'running' at higher RPM
	ASSERT_EQ(RUNNING, engine->rpmCalculator.getState());
	// check RPM
	eth.assertRpm( 1000, "RPM#3");
	// check if the injection mode is back to sequential now
	ASSERT_EQ(IM_SEQUENTIAL, getCurrentInjectionMode());
	ASSERT_NEAR(0.0, getInjectionMass(200), EPS3D); // in this test fuel calculation is not active in running mode
	// 4 sequential injections for the full cycle
	ASSERT_EQ( 8,  engine->executor.size()) << "plain#3";

	// check real events for sequential injection
	// Note: See addFuelEvents() fix inside setRpmValue()!
	expectedSimultaneousTimestamp = eth.angleToTimeUs(phase);
	ASSERT_EQ(180, eth.timeToAngle(30.000));
	eth.assertEvent5("inj start#3", 0, (void*)turnInjectionPinHigh, -expectedSimultaneousTimestamp - 1625);
	eth.assertEvent5("inj end#3", 1, (void*)turnInjectionPinLow, -expectedSimultaneousTimestamp);
}

static void doTestFasterEngineSpinningUp60_2(int startUpDelayMs, int rpm1, int expectedRpm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	// turn on FasterEngineSpinUp mode
	engineConfiguration->isFasterEngineSpinUpEnabled = true;

	setupSimpleTestEngineWithMaf(&eth, IM_SEQUENTIAL, trigger_type_e::TT_TOOTHED_WHEEL_60_2);
	eth.moveTimeForwardMs(startUpDelayMs);

	// fire 30 tooth rise/fall signals
	eth.fireTriggerEvents2(30 /* count */, 1 /*ms*/);
	// now fire missed tooth rise/fall
	eth.fireRise(5 /*ms*/);
	EXPECT_EQ(rpm1, round(Sensor::getOrZero(SensorType::Rpm)));

	eth.fireFall(1);
	eth.fireTriggerEvents2(30, 1);

	// After some more regular teeth, instant RPM is still correct
	EXPECT_EQ(rpm1, round(Sensor::getOrZero(SensorType::Rpm)));
}

TEST(cranking, testFasterEngineSpinningUp60_2) {
	doTestFasterEngineSpinningUp60_2(0, 1000, 1000);
	doTestFasterEngineSpinningUp60_2(100, 1000, 1000);
	doTestFasterEngineSpinningUp60_2(1000, 1000, 1000);
}
