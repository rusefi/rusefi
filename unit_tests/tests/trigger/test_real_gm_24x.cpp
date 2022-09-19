#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(crankingGm24x, gmRealCrankingFromFile) {
	CsvReader reader(1, /* vvtCount */ 0);
	int indeces[1] = {0};

	reader.open("tests/trigger/resources/gm_24x_cranking.csv", indeces);
	EngineTestHelper eth(TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	eth.setTriggerType(TT_GM_24x);

	while (reader.haveMore()) {
		reader.processLine(&eth);

		engine->rpmCalculator.onSlowCallback();
	}

	ASSERT_EQ( 0, eth.recentWarnings()->getCount())<< "warningCounter#vwRealCranking";
	ASSERT_EQ( 139, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}
