//
// Created by kifir on 5/5/24.
//

#include "pch.h"

using ::testing::_;

// Magic below is the simplest way to schedule overdwell sparkDown that I found in
// testAssertWeAreNotMissingASpark299.
//
// eth.smartFireFall(20);
//
// after this method will schedule over fire spark.
// P.S. May be later we will be able to simplify this spell.
static void prepareToScheduleOverdwellSparkDown(EngineTestHelper& eth) {
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	engineConfiguration->isIgnitionEnabled = true;
	engineConfiguration->isInjectionEnabled = false;

	ASSERT_EQ( 0u,  getRecentWarnings()->getCount()) << "warningCounter#0";

	eth.smartFireRise(20);
	ASSERT_EQ( 0u,  engine->triggerCentral.triggerState.currentCycle.current_index) << "ci#0";

	eth.smartFireFall(20);
	ASSERT_EQ( 1u,  engine->triggerCentral.triggerState.currentCycle.current_index) << "ci#1";

	eth.smartFireRise(20);
	ASSERT_EQ( 0u,  engine->triggerCentral.triggerState.currentCycle.current_index) << "ci#2";

	eth.smartFireFall(20);
	ASSERT_EQ( 1u,  engine->triggerCentral.triggerState.currentCycle.current_index) << "ci#3";

	eth.smartFireRise(20);

	eth.smartFireFall(20);
	ASSERT_EQ( 1u,  eth.engine.triggerCentral.triggerState.currentCycle.current_index) << "ci#5";

	ASSERT_EQ(3000, Sensor::getOrZero(SensorType::Rpm));

	// negative advance is rarely used but worth testing considering all out angleWrap.
	setWholeTimingTable(-5);
	eth.engine.periodicFastCallback();

	eth.smartFireRise(20);
}

TEST(coil, testOverdwellProtection) {
	extern bool unitTestTaskPrecisionHack;
	printf("*************************************************** testOverdwellProtection\r\n");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	unitTestTaskPrecisionHack = true;
	engine->onScheduleOverFireSparkAndPrepareNextSchedule = [&](const IgnitionEvent&, efitick_t) -> void {
		FAIL() << "Unexpected scheduling of overFireSparkAndPrepareNextSchedule";
	};

	prepareToScheduleOverdwellSparkDown(eth);

	printf("*************************************************** testOverdwellProtection start\r\n");

	// Magic above is the simpliest way to schedule overdwell sparkDown that I found in
	// testAssertWeAreNotMissingASpark299. May be later we will be able to simplify this spell.

	const IgnitionOutputPin& testCoil = enginePins.coils[0];
	const std::string testOutputName(testCoil.getName());
	ASSERT_STREQ("Coil 1", testOutputName.c_str()) << "Unexpected test output name";

	std::optional<efitick_t> turnSparkPinHighStartChargingTimestamp;
	std::optional<uint32_t> testSparkCounter;
	engine->onScheduleTurnSparkPinHighStartCharging =
		[&](const IgnitionEvent& event, efitick_t, angle_t, efitick_t chargeTime) -> void {
			if (testOutputName == event.outputs[0]->getName()) {
				EXPECT_FALSE(turnSparkPinHighStartChargingTimestamp.has_value())
					<< "Extra scheduled overFireSparkAndPrepareNextSchedule";
				turnSparkPinHighStartChargingTimestamp = chargeTime;
				testSparkCounter = event.sparkCounter;
			}
	};

	std::optional<efitick_t> overFireSparkAndPrepareNextTimestamp;
	engine->onScheduleOverFireSparkAndPrepareNextSchedule =
		[&](const IgnitionEvent& event, efitick_t fireTime) -> void {
			EXPECT_STREQ(testOutputName.c_str(), event.outputs[0]->getName())
				<< "overFireSparkAndPrepareNextSchedule is scheduled for unexpected output";
			EXPECT_FALSE(overFireSparkAndPrepareNextTimestamp.has_value())
				<< "Extra scheduled overFireSparkAndPrepareNextSchedule";
			overFireSparkAndPrepareNextTimestamp = fireTime;
			ASSERT_TRUE(testSparkCounter.has_value()) << "Missed scheduled turnSparkPinHighStartCharging";
			EXPECT_EQ(testSparkCounter.value(), event.sparkCounter)
				<< "Scheduled overFireSparkAndPrepareNextSchedule doesn't match scheduled turnSparkPinHighStartCharging";
	};

	eth.smartFireFall(20); // this operation should schedule over fire spark

	ASSERT_TRUE(turnSparkPinHighStartChargingTimestamp.has_value())
		<< "Missed scheduled overFireSparkAndPrepareNextSchedule";
	const efitimeus_t expectedSparkUpTimestampUs = NT2US(turnSparkPinHighStartChargingTimestamp.value());

	const efitimeus_t sparkDownOverdwellDurationUs = MS2US(1.5f * engine->ignitionState.sparkDwell);
	EXPECT_EQ(4500, sparkDownOverdwellDurationUs) << "Unexpected sparkDown overdwell duration";

	ASSERT_TRUE(overFireSparkAndPrepareNextTimestamp.has_value())
		<< "Missed scheduled overFireSparkAndPrepareNextSchedule";
	const efitimeus_t expectedSparkDownOverdwellTimestampUs = NT2US(overFireSparkAndPrepareNextTimestamp.value());
	EXPECT_EQ(expectedSparkDownOverdwellTimestampUs, expectedSparkUpTimestampUs + sparkDownOverdwellDurationUs);

	EXPECT_FALSE(testCoil.getLogicValue()) << "Test coil should be off";

	engine->onScheduleTurnSparkPinHighStartCharging =
		[&](const IgnitionEvent&, efitick_t, angle_t, efitick_t) -> void {
			FAIL() << "Unexpected scheduling of turnSparkPinHighStartCharging";
	};

	engine->onScheduleOverFireSparkAndPrepareNextSchedule =
		[&](const IgnitionEvent&, efitick_t) -> void {
			FAIL() << "Unexpected scheduling of overFireSparkAndPrepareNextSchedule";
	};

	std::optional<int> testIgnitionEventState;
	engine->onIgnitionEvent = [&](IgnitionEvent* event, bool state) -> void {
		if (testOutputName == event->outputs[0]->getName()) {
			EXPECT_EQ(testSparkCounter.value(), event->sparkCounter) << "Unexpected spark counter in ignition event";
			testIgnitionEventState = state;
		}
	};

	eth.setTimeAndInvokeEventsUs(expectedSparkUpTimestampUs - 1);
	EXPECT_FALSE(testCoil.getLogicValue()) << "Test still coil should be off";
	EXPECT_FALSE(testIgnitionEventState.has_value()) << "Unexpected ignition event";

	eth.setTimeAndInvokeEventsUs(expectedSparkUpTimestampUs);
	EXPECT_TRUE(testCoil.getLogicValue()) << "Test coil should be on";
	ASSERT_TRUE(testIgnitionEventState.has_value()) << "Missed ignition event";
	EXPECT_TRUE(testIgnitionEventState.value()) << "Unexpected state in ignition event";
	testIgnitionEventState.reset();

	eth.setTimeAndInvokeEventsUs(expectedSparkDownOverdwellTimestampUs - 1);
	EXPECT_TRUE(testCoil.getLogicValue()) << "Test coil still should be on";
	EXPECT_FALSE(testIgnitionEventState.has_value()) << "Unexpected ignition event";

	eth.setTimeAndInvokeEventsUs(expectedSparkDownOverdwellTimestampUs);
	EXPECT_FALSE(testCoil.getLogicValue()) << "Test coil should be off";
	ASSERT_TRUE(testIgnitionEventState.has_value()) << "Missed ignition event";
	EXPECT_FALSE(testIgnitionEventState.value()) << "Unexpected state in ignition event";
}
