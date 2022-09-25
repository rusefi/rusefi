#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(realk20, cranking) {
	CsvReader reader(/* triggerCount */ 1, /* vvtCount */ 2);

	reader.open("tests/trigger/resources/civic-K20-cranking.csv");

	EngineTestHelper eth (PROTEUS_HONDA_ELEMENT_2003);

	while (reader.haveMore()) {
// todo		reader.processLine(&eth);
	}


}


