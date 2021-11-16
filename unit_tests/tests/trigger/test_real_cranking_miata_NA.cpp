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

	ssize_t read;

	for (int i = 0; i < 23; i++) {
		reader.readLine(&eth);
	}

	EXPECT_EQ(0, GET_RPM())<< reader.lineIndex();
	EXPECT_EQ( 0, eth.recentWarnings()->getCount());

	// This tooth should be first sync point
	reader.readLine(&eth);

	EXPECT_EQ(232, GET_RPM())<< reader.lineIndex() << " @ 1";

	for (int i = 0; i < 30; i++) {
		reader.readLine(&eth);
	}
	ASSERT_EQ(224, GET_RPM())<< reader.lineIndex();


	for (int i = 0; i < 30; i++) {
		reader.readLine(&eth);
	}
	ASSERT_EQ(456, GET_RPM())<< reader.lineIndex() << " @ 2";

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ(0, eth.recentWarnings()->getCount())<< "warningCounter#realCranking";
	ASSERT_EQ(407, GET_RPM())<< reader.lineIndex();
}
