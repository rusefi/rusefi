#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(nissan, realFromFile) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/nissan-HR12DDR-with-spark-plugs.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
//	engineConfiguration->isFasterEngineSpinUpEnabled = true;
//	engineConfiguration->alwaysInstantRpm = true;

	eth.setTriggerType(trigger_type_e::TT_NISSAN_HR);


	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ( 1, eth.recentWarnings()->getCount())<< "warningCounter#nissanRealCranking";
	ASSERT_EQ(179, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();

}

TEST(nissan, realNoSparkPlugsFromFile) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/nissan-HR12DDR-no-spark-plugs.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
//	engineConfiguration->isFasterEngineSpinUpEnabled = true;
//	engineConfiguration->alwaysInstantRpm = true;

	eth.setTriggerType(trigger_type_e::TT_NISSAN_HR);


	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ( 1, eth.recentWarnings()->getCount())<< "warningCounter#nissanRealCranking";
	ASSERT_EQ(215, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();

}
