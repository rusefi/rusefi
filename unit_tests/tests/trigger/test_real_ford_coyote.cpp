#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(fordCoyote, intakeCam) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/ford-coyote-intake-cam.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_VVT_FORD_COYOTE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ( 1, eth.recentWarnings()->getCount())<< "warningCounter#intakeCam";
	ASSERT_EQ( 1093, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}

TEST(fordCoyote, exhaustCam) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/ford-coyote-exhaust-cam.csv");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	eth.setTriggerType(trigger_type_e::TT_VVT_FORD_COYOTE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ( 1, eth.recentWarnings()->getCount())<< "warningCounter#exhaustCam";
	ASSERT_EQ( 1093, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}
