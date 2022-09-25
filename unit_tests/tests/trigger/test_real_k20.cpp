#include "pch.h"

#include "logicdata_csv_reader.h"

static int getExhaustIndex() {
	return getTriggerCentral()->vvtState[/*bankIndex*/0][/*camIndex*/1].currentCycle.current_index;
}

TEST(realk20, cranking) {
	CsvReader reader(/* triggerCount */ 1, /* vvtCount */ 2);

	reader.open("tests/trigger/resources/civic-K20-cranking.csv", NORMAL_ORDER, REVERSE_ORDER);
	reader.twoBanksSingleCamMode = false;

	EngineTestHelper eth (PROTEUS_HONDA_K);

	while (reader.haveMore() && reader.lineIndex() < 101) {
		reader.processLine(&eth);
		ASSERT_EQ(getExhaustIndex(), 0) << "Skipping until first exhaust event " << reader.lineIndex();
	}

	reader.haveMore();
	reader.processLine(&eth);
	ASSERT_EQ(getExhaustIndex(), 1) << "First exhaust event " << reader.lineIndex();


	while (reader.haveMore()) {
		reader.processLine(&eth);
		ASSERT_TRUE(getExhaustIndex() != 0 ) << "At line " << reader.lineIndex();
	}

	ASSERT_EQ(173, getExhaustIndex()); // huh? not synching?

	ASSERT_EQ(1182, round(Sensor::getOrZero(SensorType::Rpm)));
}
