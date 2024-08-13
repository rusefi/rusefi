#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(arctic, realStartFromFile) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/arctic-cat.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	eth.setTriggerType(trigger_type_e::TT_ARCTIC_CAT);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ( 1, eth.recentWarnings()->getCount())<< "warningCounter#arcticRealCranking";
	ASSERT_EQ(2165, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}
