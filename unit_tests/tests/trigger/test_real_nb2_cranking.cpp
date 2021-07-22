/*
 * @file test_real_nb2_cranking.cpp
 *
 * @date July 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */


#include "engine_test_helper.h"
#include "logicdata_csv_reader.h"

TEST(realCrankingNB2, normalCranking) {
	CsvReader reader(1, /* vvtCount */ 1);
	int indeces[] = {0};

	reader.open("tests/trigger/resources/nb2-cranking-good.csv", indeces);
	WITH_ENGINE_TEST_HELPER (HELLEN_NB2);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	// VVT position nearly zero!
	EXPECT_NEAR(engine->triggerCentral.getVVTPosition(0, 0), 3.6569f, 1e-4);

	ASSERT_EQ(942, GET_RPM());

	// TODO: why warnings?
	ASSERT_EQ(3, eth.recentWarnings()->getCount());
}

TEST(realCrankingNB2, crankingMissingInjector) {
	CsvReader reader(1, /* vvtCount */ 1);
	int indeces[] = {0};

	reader.open("tests/trigger/resources/nb2-cranking-good-missing-injector-1.csv", indeces);
	WITH_ENGINE_TEST_HELPER (HELLEN_NB2);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	// VVT position nearly zero!
	EXPECT_NEAR(engine->triggerCentral.getVVTPosition(0, 0), -7.1926f, 1e-4);

	ASSERT_EQ(668, GET_RPM());

	// TODO: why warnings?
	ASSERT_EQ(2, eth.recentWarnings()->getCount());
}
