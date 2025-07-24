#include "pch.h"

#include "engine_csv_reader.h"

TEST(real, SubaruEj20gcranking_only_cam7) {
	EngineCsvReader reader(/*triggerCount*/ 2, /* vvtCount */ 0);

	/* 0 - cam
	 * 1 - crank */
	reader.open("tests/trigger/resources/subaru_6_7.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
//	setVerboseTrigger(true);
	eth.setTriggerType(trigger_type_e::TT_SUBARU_7_WITHOUT_6);

	while (reader.haveMore()) {
		reader.processLine(&eth);
		reader.assertFirstRpm(254, /*expectedFirstRpmAtIndex*/47);
	}

	ASSERT_TRUE(reader.gotRpm);
	ASSERT_FALSE(reader.gotSync);

	ASSERT_EQ(0, eth.recentWarnings()->getCount());
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

		reader.assertFirstRpm(228, /*expectedFirstRpmAtIndex*/47);

		//printf("%5d: RPM %f\n", n++, Sensor::getOrZero(SensorType::Rpm));
		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if ((rpm) && (!firstRpm)) {
			printf("Got first RPM %f, at %d\n", rpm, n);
			firstRpm = true;
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

		reader.assertFirstRpm(212, /*expectedFirstRpmAtIndex*/27);

		//printf("%5d: RPM %f\n", n++, Sensor::getOrZero(SensorType::Rpm));
		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if ((rpm) && (!firstRpm)) {
			printf("Got first RPM %f, at %d\n", rpm, n);
			firstRpm = true;
		}
		n++;
	}
}
