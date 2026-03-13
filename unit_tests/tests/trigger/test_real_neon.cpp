#include "pch.h"
#include "logicdata_csv_reader.h"

TEST(realNeon, srt4_looks_like_cam) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/neonsrt4.csv");

	EngineTestHelper eth (engine_type_e::TEST_ENGINE);
	reader.readingOffset = 0; // first column is cam signal
	//    setVerboseTrigger(true);

	engineConfiguration->vvtMode[0] = VVT_INACTIVE;
	eth.setTriggerType(trigger_type_e::TT_DODGE_NEON_2003_CAM);
	engineConfiguration->alwaysInstantRpm = true;

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ(56, round(Sensor::getOrZero(SensorType::Rpm)));
	EXPECT_EQ(0, eth.recentWarnings()->getCount());
}

TEST(realNeon, srt4_crank) {

	CsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 0);
	reader.readingOffset = 1; // second channel is crank

	reader.open("tests/trigger/resources/neonsrt4.csv", NORMAL_ORDER, NORMAL_ORDER);

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	//    setVerboseTrigger(true);

	engineConfiguration->vvtMode[0] = VVT_INACTIVE;
	eth.setTriggerType(trigger_type_e::TT_DODGE_NEON_2003_CRANK);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

//	ASSERT_EQ(, eth.recentWarnings()->getCount());
//	ASSERT_NEAR(TODO, Sensor::getOrZero(SensorType::Rpm), 0.1);
}