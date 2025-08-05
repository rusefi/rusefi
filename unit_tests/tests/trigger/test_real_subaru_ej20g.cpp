#include "pch.h"

#include "engine_csv_reader.h"

TEST(real, SubaruEj20gcranking_only_cam7) {
	EngineCsvReader reader(/*triggerCount*/ 2, /* vvtCount */ 0);

	/* 0 - cam
	 * 1 - crank */
	reader.open("tests/trigger/resources/subaru_6_7.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// See comment at SubaruEj20gDefaultCrankingSeparateTrigger about following three lines
	reader.setFlipOnRead(true);
	reader.setFlipVvtOnRead(true);
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

		reader.assertFirstRpm(183, /*expectedFirstRpmAtIndex*/36);

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

		reader.assertFirstRpm(179, /*expectedFirstRpmAtIndex*/30);

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
	EngineCsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 1);

	/* 1 - cam
	 * 0 - crank */
	reader.open("tests/trigger/resources/subaru_6_7_crank_first.csv");
	reader.setFlipOnRead(true);

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
	// invertPrimaryTriggerSignal and invertCamVVTSignal do invertion twice: see https://github.com/rusefi/rusefi/issues/8373
	// we keep both set to true to align with settings for real car.
	reader.setFlipOnRead(true);
	reader.setFlipVvtOnRead(true);

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
			EXPECT_EQ(reader.lineIndex(), 36);
			EXPECT_NEAR(rpm, 220.0f, 1.0f);
		}
	}
}
