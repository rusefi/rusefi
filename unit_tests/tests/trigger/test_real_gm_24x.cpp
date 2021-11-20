#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(crankingGm24x, gmRealCrankingFromFile) {
	CsvReader reader(1, /* vvtCount */ 0);
	int indeces[1] = {0};

	reader.open("tests/trigger/resources/nick_1.csv", indeces);
	EngineTestHelper eth (TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;

	eth.setTriggerType(TT_GM_LS_24);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	//ASSERT_EQ( 0, eth.recentWarnings()->getCount())<< "warningCounter#vwRealCranking";
	//ASSERT_EQ( 1687, GET_RPM())<< reader.lineIndex();
}