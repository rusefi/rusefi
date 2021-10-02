/*
 * @file test_ignition_scheduling.cpp
 *
 * @date Nov 17, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "spark_logic.h"

using ::testing::_;

TEST(ignition, twoCoils) {
	WITH_ENGINE_TEST_HELPER(BMW_M73_F);

	// first one to fire uses first coil
	ASSERT_EQ(ENGINE(ignitionPin[ID2INDEX(1)]), 0);
	ASSERT_EQ(ENGINE(ignitionPin[ID2INDEX(2)]), 1);
	ASSERT_EQ(ENGINE(ignitionPin[ID2INDEX(3)]), 0);
	ASSERT_EQ(ENGINE(ignitionPin[ID2INDEX(4)]), 1);

	ASSERT_EQ(ENGINE(ignitionPin[ID2INDEX(11)]), 0);
	ASSERT_EQ(ENGINE(ignitionPin[ID2INDEX(12)]), 1);

	// let's recalculate with zero timing so that we can focus on relation advance between cylinders
	engine->engineState.timingAdvance = 0;
	initializeIgnitionActions(PASS_ENGINE_PARAMETER_SIGNATURE);

	ASSERT_EQ(engine->ignitionEvents.elements[0].sparkAngle, 0);
	ASSERT_EQ((void*)engine->ignitionEvents.elements[0].outputs[0], (void*)&enginePins.coils[0]);


	ASSERT_EQ(engine->ignitionEvents.elements[1].sparkAngle, 720 / 12);
	ASSERT_EQ((void*)engine->ignitionEvents.elements[1].outputs[0], (void*)&enginePins.coils[6]);

	ASSERT_EQ(engine->ignitionEvents.elements[3].sparkAngle, 3 * 720 / 12);
	ASSERT_EQ((void*)engine->ignitionEvents.elements[3].outputs[0], (void*)&enginePins.coils[6]);
}

TEST(ignition, trailingSpark) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	EXPECT_CALL(eth.mockAirmass, getAirmass(_))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	CONFIG(specs.cylindersCount) = 1;
	CONFIG(specs.firingOrder) = FO_1;
	CONFIG(isInjectionEnabled) = false;
	CONFIG(isIgnitionEnabled) = true;

	// Fire trailing spark 10 degrees after main spark
	ENGINE(engineState.trailingSparkAngle) = 10;

	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	eth.fireTriggerEventsWithDuration(20);
	// still no RPM since need to cycles measure cycle duration
	eth.fireTriggerEventsWithDuration(20);
	ASSERT_EQ( 3000,  GET_RPM()) << "RPM#0";
	eth.clearQueue();

	/**
	 * Trigger up - scheduling fuel for full engine cycle
	 */
	eth.fireRise(20);

	// Primary coil should be high
	EXPECT_EQ(enginePins.coils[0].getLogicValue(), true);
	EXPECT_EQ(enginePins.trailingCoils[0].getLogicValue(), false);

	// Should be a TDC callback + spark firing
	EXPECT_EQ(engine->executor.size(), 2);

	// execute all actions
	eth.executeActions();

	// Primary and secondary coils should be low - primary just fired
	EXPECT_EQ(enginePins.coils[0].getLogicValue(), false);
	EXPECT_EQ(enginePins.trailingCoils[0].getLogicValue(), false);

	// Now enable trailing sparks
	CONFIG(enableTrailingSparks) = true;

	// Fire trigger fall - should schedule ignition chargings (rising edges)
	eth.fireFall(20);
	eth.moveTimeForwardMs(18);
	eth.executeActions();

	// Primary low, scheduling trailing
	EXPECT_EQ(enginePins.coils[0].getLogicValue(), true);
	EXPECT_EQ(enginePins.trailingCoils[0].getLogicValue(), false);

	eth.moveTimeForwardMs(2);
	eth.executeActions();

	// and secondary coils should be low
	EXPECT_EQ(enginePins.trailingCoils[0].getLogicValue(), true);

	// Fire trigger rise - should schedule ignition firings
	eth.fireRise(0);
	eth.moveTimeForwardMs(1);
	eth.executeActions();

	// Primary goes low, scheduling trailing
	EXPECT_EQ(enginePins.coils[0].getLogicValue(), false);
	EXPECT_EQ(enginePins.trailingCoils[0].getLogicValue(), true);

	eth.moveTimeForwardMs(1);
	eth.executeActions();
	// secondary coils should be low
	EXPECT_EQ(enginePins.trailingCoils[0].getLogicValue(), false);

}
