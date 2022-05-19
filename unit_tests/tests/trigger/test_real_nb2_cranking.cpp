/*
 * @file test_real_nb2_cranking.cpp
 *
 * @date July 13, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "logicdata_csv_reader.h"

TEST(realCrankingNB2, normalCranking) {
	CsvReader reader(1, /* vvtCount */ 1);
	int indeces[] = {0};

	reader.open("tests/trigger/resources/nb2-cranking-good.csv", indeces);
	EngineTestHelper eth (HELLEN_NB2);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	// VVT position nearly zero!
	EXPECT_NEAR(engine->triggerCentral.getVVTPosition(0, 0), 3.6569f, 1e-4);

	// Check the number of times VVT information was used to adjust crank phase
	// This should happen exactly once: once we sync, we shouldn't lose it.
	EXPECT_EQ(engine->triggerCentral.triggerState.vvtSyncCounter, 2);

	ASSERT_EQ(942, round(Sensor::getOrZero(SensorType::Rpm)));

	ASSERT_EQ(3, eth.recentWarnings()->getCount());
	ASSERT_EQ(CUSTOM_OUT_OF_ORDER_COIL, eth.recentWarnings()->get(0));
	ASSERT_EQ(CUSTOM_SYNC_ERROR, eth.recentWarnings()->get(1));
	ASSERT_EQ(CUSTOM_SYNC_COUNT_MISMATCH, eth.recentWarnings()->get(2));
}

TEST(realCrankingNB2, crankingMissingInjector) {
	CsvReader reader(1, /* vvtCount */ 1);
	int indeces[] = {0};

	reader.open("tests/trigger/resources/nb2-cranking-good-missing-injector-1.csv", indeces);
	EngineTestHelper eth (HELLEN_NB2);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	// VVT position nearly zero!
	EXPECT_NEAR(engine->triggerCentral.getVVTPosition(0, 0), -7.1926f, 1e-4);

	ASSERT_EQ(668, round(Sensor::getOrZero(SensorType::Rpm)));

	ASSERT_EQ(3, eth.recentWarnings()->getCount());
	ASSERT_EQ(CUSTOM_OUT_OF_ORDER_COIL, eth.recentWarnings()->get(0));
	ASSERT_EQ(CUSTOM_SYNC_ERROR, eth.recentWarnings()->get(1));
	ASSERT_EQ(CUSTOM_SYNC_COUNT_MISMATCH, eth.recentWarnings()->get(2));
}
