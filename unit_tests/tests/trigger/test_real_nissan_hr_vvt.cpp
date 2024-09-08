#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(nissan, realFromFileVVTIN) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/hr12-vvt-in-16s.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	eth.setTriggerType(trigger_type_e::TT_NISSAN_HR_CAM_IN);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ( 0, eth.recentWarnings()->getCount())<< "warningCounter#realFromFileVVTIN";
	ASSERT_EQ(0,  engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	ASSERT_EQ(202, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}
