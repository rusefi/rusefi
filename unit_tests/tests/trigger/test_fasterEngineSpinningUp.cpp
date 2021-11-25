/*
 * test_fasterEngineSpinningUp.cpp
 *
 *  Created on: Mar 6, 2018
 */

#include "pch.h"

TEST(cranking, testFasterEngineSpinningUp) {
	EngineTestHelper eth(TEST_ENGINE);
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

	ASSERT_EQ(IM_INDIVIDUAL_COILS, getCurrentIgnitionMode());

	eth.fireRise(1000 /*ms*/);

	// check if it's true
	ASSERT_EQ(IM_SEQUENTIAL, engine->getCurrentInjectionMode());
	ASSERT_EQ(IM_WASTED_SPARK, getCurrentIgnitionMode());
	// check if the engine has the right state
	ASSERT_EQ(SPINNING_UP, engine->rpmCalculator.getState());
	// check RPM
	ASSERT_EQ( 0,  GET_RPM()) << "RPM=0";
	// the queue should be empty, no trigger events yet
	ASSERT_EQ(0, engine->executor.size()) << "plain#1";

	// check all events starting from now
	// advance 1 revolution
	// because we have trivial TT_ONE trigger here synchronization would happen with just one rise front
	eth.fireRise(200);

	// check if the mode is changed
	ASSERT_EQ(SPINNING_UP, engine->rpmCalculator.getState());
	// due to isFasterEngineSpinUp=true, we should have already detected RPM!
	ASSERT_EQ( 300,  GET_RPM()) << "spinning-RPM#1";
	// two simultaneous injections
	ASSERT_EQ(4, engine->executor.size()) << "plain#2";
	// test if they are simultaneous
	ASSERT_EQ(IM_SIMULTANEOUS, engine->getCurrentInjectionMode());
	// test if ignition mode is temporary changed to wasted spark, if set to individual coils
	ASSERT_EQ(IM_WASTED_SPARK, getCurrentIgnitionMode());
	// check real events
	eth.assertEvent5("inj start#1", 0, (void*)startSimultaniousInjection, 97500);
	eth.assertEvent5("inj end#1", 1, (void*)endSimultaniousInjection, 100000);

	// skip the rest of the cycle
	eth.fireFall(200);

	// now clear and advance more
	eth.executeActions();

	eth.fireRise(200);

	// check if the mode is changed when fully synched
	ASSERT_EQ(CRANKING, engine->rpmCalculator.getState());
	// check RPM
	ASSERT_EQ( 200,  GET_RPM()) << "RPM#2";
	// test if they are simultaneous in cranking mode too
	ASSERT_EQ(IM_SIMULTANEOUS, engine->getCurrentInjectionMode());
	// test if ignition mode is restored to ind.coils
	ASSERT_EQ(IM_INDIVIDUAL_COILS, getCurrentIgnitionMode());
	// two simultaneous injections
	ASSERT_EQ( 4,  engine->executor.size()) << "plain#2";
	// check real events
	eth.assertEvent5("inj start#2", 0, (void*)startSimultaniousInjection, 148375);
	eth.assertEvent5("inj end#2", 1, (void*)endSimultaniousInjection, 149999);

	// skip, clear & advance 1 more revolution at higher RPM
	eth.fireFall(60);

	eth.clearQueue();
	eth.fireTriggerEventsWithDuration(60);

	// check if the mode is now changed to 'running' at higher RPM
	ASSERT_EQ(RUNNING, engine->rpmCalculator.getState());
	// check RPM
	ASSERT_EQ( 1000,  GET_RPM()) << "RPM#3";
	// check if the injection mode is back to sequential now
	ASSERT_EQ(IM_SEQUENTIAL, engine->getCurrentInjectionMode());
	// 4 sequential injections for the full cycle
	ASSERT_EQ( 8,  engine->executor.size()) << "plain#3";

	// check real events for sequential injection
	// Note: See addFuelEvents() fix inside setRpmValue()!
	eth.assertEvent5("inj start#3", 0, (void*)turnInjectionPinHigh, -31625);
	eth.assertEvent5("inj end#3", 1, (void*)turnInjectionPinLow, -30001);
}

static void doTestFasterEngineSpinningUp60_2(int startUpDelayMs, int rpm1, int expectedRpm) {
	EngineTestHelper eth(TEST_ENGINE);
	// turn on FasterEngineSpinUp mode
	engineConfiguration->isFasterEngineSpinUpEnabled = true;

	setupSimpleTestEngineWithMaf(&eth, IM_SEQUENTIAL, TT_TOOTHED_WHEEL_60_2);
	eth.moveTimeForwardMs(startUpDelayMs);

	// fire 30 tooth rise/fall signals
	eth.fireTriggerEvents2(30 /* count */, 1 /*ms*/);
	// now fire missed tooth rise/fall
	eth.fireRise(5 /*ms*/);
	EXPECT_EQ(rpm1, GET_RPM()) << "test RPM: After rise " << std::to_string(startUpDelayMs);
	eth.fireFall(1 /*ms*/);
	EXPECT_EQ(expectedRpm, GET_RPM()) << "test RPM: with " << std::to_string(startUpDelayMs) << " startUpDelayMs";
}

TEST(cranking, testFasterEngineSpinningUp60_2) {
	doTestFasterEngineSpinningUp60_2(0, 1000, 1000);
	doTestFasterEngineSpinningUp60_2(100, 1000, 1000);
	doTestFasterEngineSpinningUp60_2(1000, 1000, 1000);
}
