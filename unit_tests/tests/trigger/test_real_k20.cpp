#include "pch.h"

#include "logicdata_csv_reader.h"

static int getExhaustIndex() {
	return getTriggerCentral()->vvtState[/*bankIndex*/0][/*camIndex*/1].currentCycle.current_index;
}

TEST(realk20, cranking) {
	CsvReader reader(/* triggerCount */ 1, /* vvtCount */ 2);

	reader.open("tests/trigger/resources/civic-K20-cranking.csv", NORMAL_ORDER, REVERSE_ORDER);
	reader.twoBanksSingleCamMode = false;

	EngineTestHelper eth(engine_type_e::PROTEUS_HONDA_K);

	while (reader.haveMore()) {
		reader.processLine(&eth);

		// TODO
		// float vvtI = engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0);
		// if (vvtI != 0) {
		// 	EXPECT_TRUE(vvtI > -10 && vvtI < 10);
		// }

		auto vvtE = engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/1);
		if (vvtE) {
			EXPECT_TRUE(vvtE.Value > -10 && vvtE.Value < 10);
		}

	}

	EXPECT_EQ(1182, round(Sensor::getOrZero(SensorType::Rpm)));
	EXPECT_TRUE(getTriggerCentral()->triggerState.hasSynchronizedPhase());
}
