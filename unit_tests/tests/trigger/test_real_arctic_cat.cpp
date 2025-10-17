#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(arctic, realStartFromFile) {
	extern bool unitTestTaskPrecisionHack;
	unitTestTaskPrecisionHack = true;
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/arctic-cat.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	eth.setTriggerType(trigger_type_e::TT_ARCTIC_CAT);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	// C9354 and C9351
	// C9007 and C9008
	ASSERT_EQ( 4u, eth.recentWarnings()->getCount())<< "warningCounter#arcticRealCranking";
	ASSERT_EQ(2165, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}
