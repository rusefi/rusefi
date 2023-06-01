#include "pch.h"

#include "logicdata_csv_reader.h"

static void doTest(const char* testFile, int expectedRpm) {
	CsvReader reader(/* triggerCount */ 1, /* vvtCount */ 0);

	reader.open(testFile);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	eth.setTriggerType(trigger_type_e::TT_HONDA_K_CRANK_12_1);

	while (reader.haveMore()) {
		reader.processLine(&eth);

		engine->rpmCalculator.onSlowCallback();
	}

	ASSERT_EQ(0, eth.recentWarnings()->getCount())<< "warningCounter#vwRealCranking";
	ASSERT_EQ(expectedRpm, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}

TEST(realk24, crankingNoPlugs1) {
	doTest("tests/trigger/resources/cranking_honda_k24a2_no_plugs.csv", 188);
}

TEST(realk24, crankingNoPlugs2) {
	doTest("tests/trigger/resources/cranking_honda_k24a2_no_plugs_2.csv", 186);
}
