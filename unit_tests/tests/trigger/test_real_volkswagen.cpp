/*
 * @file test_real_volkswagen.cpp
 *
 * @date Jun 26, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */


#include "engine_test_helper.h"
#include "logicdata_csv_reader.h"

TEST(crankingVW, vwRealCrankingFromFile) {
	CsvReader reader(1, /* vvtCount */ 0);
	int indeces[1] = {0};

	reader.open("tests/trigger/resources/nick_1.csv", indeces);
	WITH_ENGINE_TEST_HELPER (VW_ABA);
	eth.setTriggerType(TT_60_2_VW PASS_ENGINE_PARAMETER_SUFFIX);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ( 0, eth.recentWarnings()->getCount())<< "warningCounter#vwRealCranking";
	ASSERT_EQ( 1687, GET_RPM())<< reader.lineIndex();
}

TEST(crankingVW, crankingTwiceWithGap) {
	WITH_ENGINE_TEST_HELPER (VW_ABA);
	eth.setTriggerType(TT_60_2_VW PASS_ENGINE_PARAMETER_SUFFIX);

	{
		CsvReader reader(1, /* vvtCount */ 0);
		int indeces[1] = {0};

		reader.open("tests/trigger/resources/nick_1.csv", indeces);
		
		while (reader.haveMore()) {
			reader.processLine(&eth);
		}

		ASSERT_EQ(0, eth.recentWarnings()->getCount())<< "warningCounter#vwRealCranking";
		ASSERT_EQ(1687, GET_RPM())<< reader.lineIndex();
	}

	auto now = getTimeNowNt();

	{
		// Offset by a short time offset, 10 seconds
		CsvReader reader(1, /* vvtCount */ 0, 10);
		int indeces[1] = {0};

		reader.open("tests/trigger/resources/nick_1.csv", indeces);
		
		while (reader.haveMore()) {
			reader.processLine(&eth);
		}

		ASSERT_EQ(0, eth.recentWarnings()->getCount());
		ASSERT_EQ(1687, GET_RPM())<< reader.lineIndex();
	}

	{
		// Offset by long time offset, 5m14.15s
		CsvReader reader(1, /* vvtCount */ 0, 314.159);
		int indeces[1] = {0};

		reader.open("tests/trigger/resources/nick_1.csv", indeces);
		
		while (reader.haveMore()) {
			reader.processLine(&eth);
		}

		ASSERT_EQ(0, eth.recentWarnings()->getCount());
		ASSERT_EQ(1688, GET_RPM())<< reader.lineIndex();
	}
}
