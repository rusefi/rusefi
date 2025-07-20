#include "pch.h"

#include "engine_csv_reader.h"

TEST(realSubaruEj20g, cranking) {
	CsvReader reader(/*triggerCount*/ 2, /* vvtCount */ 0);

	/* 0 - cam
	 * 1 - cranck */
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
		//printf("%5d: RPM %f\n", n++, Sensor::getOrZero(SensorType::Rpm));
		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if ((rpm) && (!firstRpm)) {
			printf("Got first RPM %f, at %d\n", rpm, n);
			firstRpm = true;
		}
		n++;
	}
}

TEST(realSubaruEj20g, crankingWot) {
	CsvReader reader(/*triggerCount*/ 2, /* vvtCount */ 0);

	/* 0 - cam
	 * 1 - cranck */
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
		//printf("%5d: RPM %f\n", n++, Sensor::getOrZero(SensorType::Rpm));
		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if ((rpm) && (!firstRpm)) {
			printf("Got first RPM %f, at %d\n", rpm, n);
			firstRpm = true;
		}
		n++;
	}
}
