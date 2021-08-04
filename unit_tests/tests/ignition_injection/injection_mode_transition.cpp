/*
 * @file injection_mode_transition.cpp
 *
 *  Created on: Jul 19, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

static void doRevolution(EngineTestHelper& eth, int periodMs) {
	float halfToothTime = (periodMs / 6.0f) / 2;

	eth.smartFireRise(halfToothTime);
	eth.fireFall(halfToothTime);
	eth.smartFireRise(halfToothTime);
	eth.fireFall(halfToothTime);
	eth.smartFireRise(halfToothTime);
	eth.fireFall(halfToothTime);

	// now missing tooth
	eth.smartFireRise(halfToothTime);
	eth.fireFall(3 * halfToothTime);

	// This tooth is the sync point!
	eth.smartFireRise(halfToothTime);
	eth.fireFall(halfToothTime);
}

// https://github.com/rusefi/rusefi/issues/1592
TEST(fuelControl, transitionIssue1592) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	ENGINE(tdcMarkEnabled) = false;
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth, IM_SEQUENTIAL);

	EXPECT_CALL(eth.mockAirmass, getAirmass(500))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));

	// This is easiest to trip on a wheel that requires sync
	engineConfiguration->trigger.customTotalToothCount = 6;
	engineConfiguration->trigger.customSkippedToothCount = 1;
	eth.setTriggerType(TT_TOOTHED_WHEEL PASS_ENGINE_PARAMETER_SUFFIX);
	engineConfiguration->ambiguousOperationMode = FOUR_STROKE_CAM_SENSOR;
	engineConfiguration->isFasterEngineSpinUpEnabled = true;

	setTable(config->injectionPhase, 0.0f);
	setArrayValues(config->crankingFuelCoef, 1.0f);
	setArrayValues(config->crankingCycleCoef, 1.0f);

	engineConfiguration->globalTriggerAngleOffset = 20;

	// Yes, this is a ton of fuel but it makes the repro easier
	engineConfiguration->cranking.baseFuel = 213.6;
	engineConfiguration->cranking.rpm = 501;

	// Test the transition from batch cranking to sequential running
	engineConfiguration->crankingInjectionMode = IM_BATCH;
	engineConfiguration->twoWireBatchInjection = true;


	// First sync point will schedule cranking pulse since we're in "faster spin up" mode
	doRevolution(eth, 240);

	{
		// Injector 2 should be scheduled to open then close
		void* inj2 = reinterpret_cast<void*>(&engine->injectionEvents.elements[1]);

		ASSERT_EQ(engine->executor.size(), 2);

		// Check that the action is correct - we don't care about the timing necessarily
		auto sched_open = engine->executor.getForUnitTest(0);
		ASSERT_EQ(sched_open->action.getArgument(), inj2);
		ASSERT_EQ(sched_open->action.getCallback(), (void(*)(void*))turnInjectionPinHigh);

		auto sched_close = engine->executor.getForUnitTest(1);
		// Next action should be closing the same injector
		ASSERT_EQ(sched_close->action.getArgument(), inj2);
		ASSERT_EQ(sched_close->action.getCallback(), (void(*)(void*))turnInjectionPinLow);
	}

	// Run the engine for some revs
	for (size_t i = 0; i < 10; i++) {
		doRevolution(eth, 150);
	}

	// Check that no injectors are stuck open
	// Injectors 1/3 should be open
	EXPECT_EQ(enginePins.injectors[0].getOverlappingCounter(), 1);
	EXPECT_EQ(enginePins.injectors[1].getOverlappingCounter(), 0);
	EXPECT_EQ(enginePins.injectors[2].getOverlappingCounter(), 1);
	EXPECT_EQ(enginePins.injectors[3].getOverlappingCounter(), 0);
}
