/*
 * @file test_real_volkswagen.cpp
 *
 * @date Jun 26, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */


#include "engine_test_helper.h"
#include "logicdata_csv_reader.h"

TEST(crankingVW, vwRealCrankingFromFile) {
	CsvReader reader;
	reader.triggerCount = 1;
	int indeces[1] = {0};
//	setVerboseTrigger(true);

	reader.open("tests/trigger/recourses/nick_1.csv", indeces);
	WITH_ENGINE_TEST_HELPER (VW_ABA);
	eth.setTriggerType(TT_60_2_VW PASS_ENGINE_PARAMETER_SUFFIX);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ( 1, eth.recentWarnings()->getCount())<< "warningCounter#vwRealCranking";
	ASSERT_EQ( 1683, GET_RPM())<< reader.lineIndex;

}
