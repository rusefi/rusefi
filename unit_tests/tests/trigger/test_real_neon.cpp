#include "pch.h"
#include "logicdata_csv_reader.h"

TEST(realNeon, srt4_only_crank) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/neonsrt4.csv");
	EngineTestHelper eth (engine_type_e::ET_DODGE_NEON_2003);
	engineConfiguration->alwaysInstantRpm = true;

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ(56, round(Sensor::getOrZero(SensorType::Rpm)));
	EXPECT_EQ(ObdCode::CUSTOM_OUT_OF_ORDER_COIL, eth.recentWarnings()->get(0).Code);
	EXPECT_EQ(1u, eth.recentWarnings()->getCount());
}

