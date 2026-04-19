#include "pch.h"
#include "engine_csv_reader.h"

TEST(realNeon, srt4_looks_like_cam) {
	EngineCsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/neonsrt4.csv");

	EngineTestHelper eth (engine_type_e::TEST_ENGINE);
	reader.setReadingOffset(0); // first column is cam signal

	engineConfiguration->vvtMode[0] = VVT_INACTIVE;
	eth.setTriggerType(trigger_type_e::TT_DODGE_NEON_2003_CAM);
	engineConfiguration->alwaysInstantRpm = true;

	while (reader.haveMore()) {
		reader.processLine(&eth);
		reader.assertFirstRpm(226, 124);
	}

	ASSERT_EQ(56, round(Sensor::getOrZero(SensorType::Rpm)));
	EXPECT_EQ(0, eth.recentWarnings()->getCount());
}

TEST(realNeon, srt4_crank) {

	EngineCsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 0);
	reader.setReadingOffset(1); // second channel is crank

	reader.open("tests/trigger/resources/neonsrt4.csv", NORMAL_ORDER, NORMAL_ORDER);

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->vvtMode[0] = VVT_INACTIVE;
	eth.setTriggerType(trigger_type_e::TT_DODGE_NEON_2003_CRANK);
	engineConfiguration->alwaysInstantRpm = true;

	while (reader.haveMore()) {
		reader.processLine(&eth);
		reader.assertFirstRpm(75, 168);
	}

	ASSERT_NEAR(67, Sensor::getOrZero(SensorType::Rpm), 1);
}
