/*
 * @file test_real_cranking_miata_NA.cpp
 *
 * @date May 22, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

#include "logicdata_csv_reader.h"
#include <string>

TEST(cranking, realCrankingFromFile) {
	CsvReader reader(2, /* vvtCount */ 0);
	int indeces[2] = {1, 0}; // this logic data file has first trigger channel in second column and second trigger channel in first column
	reader.open("tests/trigger/resources/cranking_na_3.csv", indeces);

	EngineTestHelper eth (FRANKENSO_MIATA_NA6_MAP);
	engineConfiguration->alwaysInstantRpm = true;

	ssize_t read;

	for (int i = 0; i < 11; i++) {
		reader.readLine(&eth);
	}

	EXPECT_EQ(0, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
	EXPECT_EQ( 0, eth.recentWarnings()->getCount());

	// This tooth should be first sync point
	reader.readLine(&eth);

	EXPECT_EQ(228, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex() << " @ 1";

	for (int i = 0; i < 42; i++) {
		reader.readLine(&eth);
	}
	EXPECT_EQ(261, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();


	for (int i = 0; i < 30; i++) {
		reader.readLine(&eth);
	}
	EXPECT_EQ(738, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex() << " @ 2";

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	EXPECT_EQ(0, eth.recentWarnings()->getCount())<< "warningCounter#realCranking";
	EXPECT_EQ(191, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}
