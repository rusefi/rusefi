/*
 * @file injection_mode_transition.cpp
 *
 *  Created on: Jul 19, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_test_helper.h"

static void doRevolution(EngineTestHelper& eth, int periodMs) {
	float halfToothTime = (periodMs / 6.0f) / 2;

	eth.fireRise(halfToothTime);
	eth.fireFall(halfToothTime);
	eth.fireRise(halfToothTime);
	eth.fireFall(halfToothTime);
	eth.fireRise(halfToothTime);
	eth.fireFall(halfToothTime);

	// now missing tooth
	eth.fireRise(halfToothTime);
	eth.fireFall(3 * halfToothTime);

	// This tooth is the sync point!
	eth.fireRise(halfToothTime);
	eth.fireFall(halfToothTime);
}

// https://github.com/rusefi/rusefi/issues/1592
TEST(fuelControl, transitionIssue1592) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth, IM_SEQUENTIAL);

	// This is easiest to trip on a wheel that requires sync
	engineConfiguration->trigger.customTotalToothCount = 6;
	engineConfiguration->trigger.customSkippedToothCount = 1;
	eth.setTriggerType(TT_TOOTHED_WHEEL PASS_ENGINE_PARAMETER_SUFFIX);
	engineConfiguration->ambiguousOperationMode = FOUR_STROKE_CAM_SENSOR;
	engineConfiguration->isFasterEngineSpinUpEnabled = true;

	engineConfiguration->fuelAlgorithm = LM_ALPHA_N;

	extern fuel_Map3D_t fuelMap;
	fuelMap.setAll(13);
	extern fuel_Map3D_t fuelPhaseMap;
	fuelPhaseMap.setAll(0);

	engineConfiguration->globalTriggerAngleOffset = 20;

	// Yes, this is a ton of fuel but it makes the repro easier
	engineConfiguration->cranking.baseFuel = 89;
	engineConfiguration->cranking.rpm = 500;

	// Test the transition from batch cranking to sequential running
	engineConfiguration->crankingInjectionMode = IM_BATCH;
	engineConfiguration->twoWireBatchInjection = true;


	// First sync point will schedule cranking pulse since we're in "faster spin up" mode
	doRevolution(eth, 150);

	{
		// Injector 2 should be scheduled to open then close
		void* inj2 = reinterpret_cast<void*>(&engine->injectionEvents.elements[1]);

		ASSERT_EQ(engine->executor.size(), 2);

		// Check that the action is correct - we don't care about the timing necessarily
		auto sched_open = engine->executor.getForUnitTest(0);
		ASSERT_EQ(sched_open->action.getArgument(), inj2);
		ASSERT_EQ(sched_open->action.getCallback(), &turnInjectionPinHigh);

		auto sched_close = engine->executor.getForUnitTest(1);
		// Next action should be closing the same injector
		ASSERT_EQ(sched_close->action.getArgument(), inj2);
		ASSERT_EQ(sched_close->action.getCallback(), &turnInjectionPinLow);
	}

	// Execute the first of those two events - the injector opens, but doesn't yet close.
	engine->executor.executeAll(getTimeNowUs() + MS2US(35));

	// Check that queue got shorter, and overlap counters were incremented on injectors 2/3 (batch mode, remember?)
	{
		// Check that it was exec'd
		ASSERT_EQ(engine->executor.size(), 1);

		// Injectors 2/3 should currently be open
		EXPECT_EQ(enginePins.injectors[0].getOverlappingCounter(), 0);
		EXPECT_EQ(enginePins.injectors[1].getOverlappingCounter(), 1);
		EXPECT_EQ(enginePins.injectors[2].getOverlappingCounter(), 1);
		EXPECT_EQ(enginePins.injectors[3].getOverlappingCounter(), 0);
	}

	// Second sync point will transition to running
	// This needs to reset overlapping state as it may reschedule injector openings
	doRevolution(eth, 150);

	// Injectors should all be closed immediately after mode change
	EXPECT_EQ(enginePins.injectors[0].getOverlappingCounter(), 0);

	// !!!!!!!!! BUG !!!!!!!!!!!!!!!
	// These next two should be equal to 0, not 1
	EXPECT_EQ(enginePins.injectors[1].getOverlappingCounter(), 1);
	EXPECT_EQ(enginePins.injectors[2].getOverlappingCounter(), 1);
	// !!!!!!!!! BUG !!!!!!!!!!!!!!!

	EXPECT_EQ(enginePins.injectors[3].getOverlappingCounter(), 0);
}

