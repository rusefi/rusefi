#include "pch.h"

#include "engine_csv_reader.h"

TEST(real, SubaruEj20gcranking_only_cam7) {
	EngineCsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 0);

	/* 0 - cam
	 * 1 - crank */
	reader.open("tests/trigger/resources/subaru_6_7.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->invertPrimaryTriggerSignal = true;

//	setVerboseTrigger(true);
	eth.setTriggerType(trigger_type_e::TT_VVT_SUBARU_7_WITHOUT_6);

	while (reader.haveMore()) {
		reader.processLine(&eth);
		reader.assertFirstRpm(240, /*expectedFirstRpmAtIndex*/38);
	}

	ASSERT_TRUE(reader.gotRpm);
	ASSERT_FALSE(reader.gotSync);

	// this currently fails
	//ASSERT_EQ(0u, eth.recentWarnings()->getCount());
}

TEST(real, SubaruEj20gDefaultCranking) {
	EngineCsvReader reader(/*triggerCount*/ 2, /* vvtCount */ 0);

	/* 0 - cam
	 * 1 - crank */
	reader.open("tests/trigger/resources/subaru_6_7.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	//setVerboseTrigger(true);
	eth.setTriggerType(trigger_type_e::TT_SUBARU_7_6);
	engineConfiguration->invertPrimaryTriggerSignal = true;
	engineConfiguration->invertSecondaryTriggerSignal = true;

	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	int n = 0;
	bool firstRpm = false;
	while (reader.haveMore()) {
		reader.processLine(&eth);

		reader.assertFirstRpm(193, /*expectedFirstRpmAtIndex*/38);

		//printf("%5d: RPM %f\n", n++, Sensor::getOrZero(SensorType::Rpm));
		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if ((rpm) && (!firstRpm)) {
			printf("Got first RPM %f, at %d\n", rpm, n);
			firstRpm = true;
		}
		if ((rpm == 0) && (firstRpm)) {
			printf("Lost RPM at %d\n", n);
			firstRpm = false;
		}
		n++;
	}
}

TEST(real, SubaruEj20gCrankingWot) {
	EngineCsvReader reader(/*triggerCount*/ 2, /* vvtCount */ 0);

	/* 0 - cam
	 * 1 - crank */
	reader.open("tests/trigger/resources/subaru_6_7_wot.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	//setVerboseTrigger(true);
	eth.setTriggerType(trigger_type_e::TT_SUBARU_7_6);
	engineConfiguration->invertPrimaryTriggerSignal = true;
	engineConfiguration->invertSecondaryTriggerSignal = true;

	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	int n = 0;
	bool firstRpm = false;
	while (reader.haveMore()) {
		reader.processLine(&eth);

		reader.assertFirstRpm(188, /*expectedFirstRpmAtIndex*/32);

		//printf("%5d: RPM %f\n", n++, Sensor::getOrZero(SensorType::Rpm));
		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if ((rpm) && (!firstRpm)) {
			printf("Got first RPM %f, at %d\n", rpm, n);
			firstRpm = true;
		}
		if ((rpm == 0) && (firstRpm)) {
			printf("Lost RPM at %d\n", n);
			firstRpm = false;
		}
		n++;
	}
}

TEST(real, SubaruEj20gDefaultCranking_only_crank) {
	EngineCsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 0);

	/* 1 - cam
	 * 0 - crank */
	reader.open("tests/trigger/resources/subaru_6_7_crank_first.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	//setVerboseTrigger(true);
	eth.setTriggerType(trigger_type_e::TT_SUBARU_7_6_CRANK);
	engineConfiguration->invertPrimaryTriggerSignal = true;

	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	while (reader.haveMore()) {
		reader.processLine(&eth);

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		bool hasFullSync = getTriggerCentral()->triggerState.hasSynchronizedPhase();

		reader.assertFirstRpm(182, /*expectedFirstRpmAtIndex*/13);
	}
}

TEST(real, SubaruEj20gDefaultCrankingSeparateTrigger) {
	EngineCsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 1);

	/* 1 - cam
	 * 0 - crank */
	reader.open("tests/trigger/resources/subaru_6_7_crank_first.csv");
	// test CSV file was captured without inversion of cam and crank signals.
	// triggers are defined with SyncEdge::RiseOnly, while with real VR sensor we should rely on falling edges only.
	// FallOnly is not supported (yet?). So trigger is defined as SyncEdge::RiseOnly and with 1 degree tooth width.
	// setting both crank and cam inversion allows us to feed trigger decoder with correct (falling) edges.

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	//setVerboseTrigger(true);
	engineConfiguration->vvtMode[0] = VVT_SUBARU_7TOOTH;
	eth.setTriggerType(trigger_type_e::TT_SUBARU_7_6_CRANK);
	engineConfiguration->invertPrimaryTriggerSignal = true;
	engineConfiguration->invertCamVVTSignal = true;

	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	bool gotFullSync = false;
	while (reader.haveMore()) {
		reader.processLine(&eth);

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		bool hasFullSync = getTriggerCentral()->triggerState.hasSynchronizedPhase();

		reader.assertFirstRpm(182, /*expectedFirstRpmAtIndex*/13);
		if (0) {
			float vvt = engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0);
			printf("%5d: RPM %f, hasFullSync %d, vvt = %f\n", reader.lineIndex(), Sensor::getOrZero(SensorType::Rpm), hasFullSync, vvt);
		}

		if (!gotFullSync && hasFullSync && rpm) {
			gotFullSync = true;

			// Should get full sync and non-zero RPM
			EXPECT_EQ(reader.lineIndex(), 38);
			EXPECT_NEAR(rpm, 220.0f, 1.0f);
		}
	}
}

// Following test loses hasFullSync twice
// Test also has RPM falling to zero twice
// While real car was runnig fine while capturing this CSV
// TODO: investigate
// TODO: this test also triggers a watchdog warning after clearing the EngineTestHelper!
TEST(real, SubaruEj20gDefaultRev) {
	extern bool unitTestTaskNoFastCallWhileAdvancingTimeHack;
	unitTestTaskNoFastCallWhileAdvancingTimeHack = true;

	EngineCsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 1);

	/* 1 - cam
	 * 0 - crank */
	reader.open("tests/trigger/resources/subaru_6_7_rev.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	//setVerboseTrigger(true);
	engineConfiguration->vvtMode[0] = VVT_SUBARU_7TOOTH;
	eth.setTriggerType(trigger_type_e::TT_SUBARU_7_6_CRANK);
	engineConfiguration->invertPrimaryTriggerSignal = false;
	engineConfiguration->invertCamVVTSignal = false;

	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	bool gotFullSync = false;
	while (reader.haveMore()) {
		reader.processLine(&eth);

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		bool hasFullSync = getTriggerCentral()->triggerState.hasSynchronizedPhase();

		//reader.assertFirstRpm(182, /*expectedFirstRpmAtIndex*/13);
		if (0) {
			float vvt = engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0);
			printf("%5d: RPM %f, hasFullSync %d, vvt = %f\n", reader.lineIndex(), Sensor::getOrZero(SensorType::Rpm), hasFullSync, vvt);
		}

		if (!gotFullSync && hasFullSync && rpm) {
			gotFullSync = true;

			// Should get full sync and non-zero RPM
			EXPECT_EQ(reader.lineIndex(), 37);
			EXPECT_NEAR(rpm, 2174.0f, 1.0f);
		}
	}
}
