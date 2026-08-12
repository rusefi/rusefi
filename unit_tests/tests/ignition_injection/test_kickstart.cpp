/**
 * @file test_kickstart.cpp
 *
 * Kick-start cranking mode for Ural bikes #4569:
 * "I see the trigger mark, after 3ms I plan to ignite in both cylinders"
 */

#include "pch.h"

#include "limp_manager.h"

TEST(kickStart, chargesAndFiresBothCoilsOffTriggerMark) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->kickStartCranking = true;
	engineConfiguration->ignitionDwellForCrankingMs = 3;
	engine->ignitionState.updateDwell(/*rpm*/0, /*isCranking*/true);

	// engine is stopped: RPM is zero, way below the kick-start threshold
	ASSERT_EQ(0, Sensor::getOrZero(SensorType::Rpm));

	eth.firePrimaryTriggerRise();

	// the trigger mark starts charging both coils
	EXPECT_TRUE(enginePins.coils[0].getLogicValue());
	EXPECT_TRUE(enginePins.coils[1].getLogicValue());

	// with the fire event scheduled a dwell-time later
	EXPECT_EQ(engine->scheduler.size(), 1);

	// 2ms into the 3ms dwell: still charging
	eth.moveTimeForwardAndInvokeEventsUs(MS2US(2));
	EXPECT_TRUE(enginePins.coils[0].getLogicValue());
	EXPECT_TRUE(enginePins.coils[1].getLogicValue());

	// past the end of dwell: both coils have fired
	eth.moveTimeForwardAndInvokeEventsUs(MS2US(2));
	EXPECT_FALSE(enginePins.coils[0].getLogicValue());
	EXPECT_FALSE(enginePins.coils[1].getLogicValue());
}

TEST(kickStart, inactiveWhenDisabled) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// kickStartCranking defaults to false
	engineConfiguration->ignitionDwellForCrankingMs = 3;
	engine->ignitionState.updateDwell(/*rpm*/0, /*isCranking*/true);

	eth.firePrimaryTriggerRise();

	EXPECT_FALSE(enginePins.coils[0].getLogicValue());
	EXPECT_FALSE(enginePins.coils[1].getLogicValue());
	EXPECT_EQ(engine->scheduler.size(), 0);
}

TEST(kickStart, inactiveAboveThresholdRpm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->kickStartCranking = true;
	engineConfiguration->ignitionDwellForCrankingMs = 3;
	engine->ignitionState.updateDwell(/*rpm*/0, /*isCranking*/true);

	// engine is running above the kick-start threshold: normal spark scheduling owns the coils
	Sensor::setMockValue(SensorType::Rpm, KICK_START_MODE_MAX_RPM);

	eth.firePrimaryTriggerRise();

	EXPECT_FALSE(enginePins.coils[0].getLogicValue());
	EXPECT_FALSE(enginePins.coils[1].getLogicValue());
}

TEST(kickStart, refusesToChargeWithoutValidDwell) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->kickStartCranking = true;
	// no valid dwell: charging a coil with no scheduled release would overheat it
	engine->ignitionState.sparkDwell = 0;

	eth.firePrimaryTriggerRise();

	EXPECT_FALSE(enginePins.coils[0].getLogicValue());
	EXPECT_FALSE(enginePins.coils[1].getLogicValue());
	EXPECT_EQ(engine->scheduler.size(), 0);
}

TEST(kickStart, limpManagerSuppressesNormalSpark) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	LimpManager dut;

	// without kick-start mode, low RPM does not suppress spark
	dut.updateState(500, 0);
	EXPECT_TRUE(dut.allowIgnition());

	engineConfiguration->kickStartCranking = true;

	// below the threshold, normal spark is suppressed - kick-start logic owns the coils
	dut.updateState(500, 0);
	EXPECT_FALSE(dut.allowIgnition());
	EXPECT_EQ(ClearReason::KickStart, dut.allowIgnition().reason);

	// above the threshold, normal spark scheduling resumes
	dut.updateState(KICK_START_MODE_MAX_RPM, 0);
	EXPECT_TRUE(dut.allowIgnition());
}
