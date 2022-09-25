#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(realk20, cranking) {
	CsvReader reader(/* triggerCount */ 1, /* vvtCount */ 2);

	reader.open("tests/trigger/resources/civic-K20-cranking.csv", NORMAL_ORDER, REVERSE_ORDER);

	EngineTestHelper eth (PROTEUS_HONDA_K);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ(1182, round(Sensor::getOrZero(SensorType::Rpm)));


}


