/*
 * @file test_ignition_scheduling.cpp
 *
 * @date Nov 17, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "defaults.h"
#include "spark_logic.h"

using ::testing::_;
using ::testing::InSequence;
using ::testing::StrictMock;

TEST(ignition, twoCoils) {
	EngineTestHelper eth(engine_type_e::FRANKENSO_BMW_M73_F);

	// let's recalculate with zero timing so that we can focus on relation advance between cylinders
	setArrayValues(engine->engineState.timingAdvance, 0.0f);
	initializeIgnitionActions();

	// first one to fire uses first coil
	EXPECT_EQ(engine->ignitionEvents.elements[0].coilIndex, 0);
	EXPECT_EQ(engine->ignitionEvents.elements[1].coilIndex, 6);
	EXPECT_EQ(engine->ignitionEvents.elements[2].coilIndex, 0);
	EXPECT_EQ(engine->ignitionEvents.elements[3].coilIndex, 6);
	EXPECT_EQ(engine->ignitionEvents.elements[4].coilIndex, 0);
	EXPECT_EQ(engine->ignitionEvents.elements[5].coilIndex, 6);
	EXPECT_EQ(engine->ignitionEvents.elements[6].coilIndex, 0);
	EXPECT_EQ(engine->ignitionEvents.elements[7].coilIndex, 6);
	EXPECT_EQ(engine->ignitionEvents.elements[8].coilIndex, 0);
	EXPECT_EQ(engine->ignitionEvents.elements[9].coilIndex, 6);
	EXPECT_EQ(engine->ignitionEvents.elements[10].coilIndex, 0);
	EXPECT_EQ(engine->ignitionEvents.elements[11].coilIndex, 6);

	ASSERT_EQ(engine->ignitionEvents.elements[0].sparkAngle, 0);
	ASSERT_EQ((void*)engine->ignitionEvents.elements[0].outputs[0], (void*)&enginePins.coils[0]);


	ASSERT_EQ(engine->ignitionEvents.elements[1].sparkAngle, 720 / 12);
	ASSERT_EQ((void*)engine->ignitionEvents.elements[1].outputs[0], (void*)&enginePins.coils[6]);

	ASSERT_EQ(engine->ignitionEvents.elements[3].sparkAngle, 3 * 720 / 12);
	ASSERT_EQ((void*)engine->ignitionEvents.elements[3].outputs[0], (void*)&enginePins.coils[6]);
}

TEST(ignition, trailingSpark) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = false;

	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	engineConfiguration->cylindersCount = 1;
	engineConfiguration->firingOrder = FO_1;
	engineConfiguration->isInjectionEnabled = false;
	engineConfiguration->isIgnitionEnabled = true;

	// Fire trailing spark 10 degrees after main spark
	engine->engineState.trailingSparkAngle = 10;

	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	setWholeTimingTable(0);

	eth.fireTriggerEventsWithDuration(20);
	// still no RPM since need to cycles measure cycle duration
	eth.fireTriggerEventsWithDuration(20);
	ASSERT_EQ( 3000,  Sensor::getOrZero(SensorType::Rpm)) << "RPM#0";

	/**
	 * Trigger up - scheduling fuel for full engine cycle
	 */
	eth.smartFireRise(20);

	// Primary coil should be high
	EXPECT_EQ(enginePins.coils[0].getLogicValue(), true);
	EXPECT_EQ(enginePins.trailingCoils[0].getLogicValue(), false);

	// Should be a TDC callback + spark firing
	EXPECT_EQ(engine->scheduler.size(), 2);

	// execute all actions
	eth.executeActions();

	// Primary and secondary coils should be low - primary just fired
	EXPECT_EQ(enginePins.coils[0].getLogicValue(), false);
	EXPECT_EQ(enginePins.trailingCoils[0].getLogicValue(), false);

	// Now enable trailing sparks
	engineConfiguration->enableTrailingSparks = true;

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

TEST(ignition, CylinderTimingTrim) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// Base timing 15 degrees
	setTable(config->ignitionTable, 15);

	// negative numbers retard timing, positive advance
	setTable(config->ignTrims[0].table, -4);
	setTable(config->ignTrims[1].table, -2);
	setTable(config->ignTrims[2].table,  2);
	setTable(config->ignTrims[3].table,  4);

	// run the ignition math
	engine->periodicFastCallback();

	// Check that each cylinder gets the expected timing
	float unadjusted = 15;
	EXPECT_NEAR(engine->engineState.timingAdvance[0], unadjusted - 4, EPS4D);
	EXPECT_NEAR(engine->engineState.timingAdvance[1], unadjusted - 2, EPS4D);
	EXPECT_NEAR(engine->engineState.timingAdvance[2], unadjusted + 2, EPS4D);
	EXPECT_NEAR(engine->engineState.timingAdvance[3], unadjusted + 4, EPS4D);
}

TEST(ignition, negativeAdvance) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

  int rpm = 0;
  float load = 50;

	engineConfiguration->fixedTiming = -13;
	engineConfiguration->timingMode = TM_FIXED;
	// run the ignition math
	engine->periodicFastCallback();

	ASSERT_EQ(0, Sensor::getOrZero(SensorType::Rpm));

	ASSERT_EQ(DEFAULT_CRANKING_ANGLE, getCrankingAdvance(rpm, load));
	ASSERT_EQ(0, getAdvanceCorrections(load));
	ASSERT_EQ(707, engine->ignitionState.getWrappedAdvance(rpm, load));

	ASSERT_NEAR(-13, engine->ignitionState.baseIgnitionAdvance, EPS4D);
	ASSERT_NEAR(-13, engine->ignitionState.correctedIgnitionAdvance, EPS4D);
}

TEST(ignition, negativeAdvance2stroke) {
	EngineTestHelper eth(engine_type_e::SACHS);

	int rpm = 0;
	float load = 50;

	ASSERT_EQ(360, getEngineState()->engineCycle);

	engineConfiguration->fixedTiming = -13;
	engineConfiguration->timingMode = TM_FIXED;
	// run the ignition math
	engine->periodicFastCallback();

	ASSERT_EQ(0, Sensor::getOrZero(SensorType::Rpm));
	ASSERT_EQ(347, engine->ignitionState.getWrappedAdvance(rpm, load));

	ASSERT_NEAR(-13, engine->ignitionState.correctedIgnitionAdvance, EPS4D);
}

TEST(ignition, oddCylinderWastedSpark) {
	StrictMock<MockExecutor> mockExec;

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->scheduler.setMockExecutor(&mockExec);
	engineConfiguration->cylindersCount = 1;
	engineConfiguration->firingOrder = FO_1;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

	efitick_t nowNt1 = 1000000;
	efitick_t nowNt2 = 2222222;


	engine->rpmCalculator.oneDegreeUs = 100;

	{
		InSequence is;

		// Should schedule one dwell+fire pair:
		// Dwell 5 deg from now
		float nt1deg = USF2NT(engine->rpmCalculator.oneDegreeUs);
		efitick_t startTime = nowNt1 + nt1deg * 5;
		EXPECT_CALL(mockExec, schedule(testing::NotNull(), _, startTime, _));
		// Spark 15 deg from now
		efitick_t endTime = startTime + nt1deg * 10;
		EXPECT_CALL(mockExec, schedule(testing::NotNull(), _, endTime, _));


		// Should schedule second dwell+fire pair, the out of phase copy
		// Dwell 5 deg from now
		startTime = nowNt2 + nt1deg * 5;
		EXPECT_CALL(mockExec, schedule(testing::NotNull(), _, startTime, _));
		// Spark 15 deg from now
		endTime = startTime + nt1deg * 10;
		EXPECT_CALL(mockExec, schedule(testing::NotNull(), _, endTime, _));
	}

	engine->ignitionState.sparkDwell = 1;

	// dwell should start at 15 degrees ATDC and firing at 25 deg ATDC
	engine->ignitionState.dwellDurationAngle = 10;
	engine->engineState.timingAdvance[0] = -25;
	engine->engineState.useOddFireWastedSpark = true;
	engineConfiguration->minimumIgnitionTiming = -25;

	// expect to schedule the on-phase dwell and spark (not the wasted spark copy)
	onTriggerEventSparkLogic(1200, nowNt1, 10, 30);

	// expect to schedule second events, the out-of-phase dwell and spark (the wasted spark copy)
	onTriggerEventSparkLogic(1200, nowNt2, 360 + 10, 360 + 30);
}
