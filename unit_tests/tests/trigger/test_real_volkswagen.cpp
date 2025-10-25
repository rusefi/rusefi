/*
 * @file test_real_volkswagen.cpp
 *
 * @date Jun 26, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(crankingVW, vwRealCrankingFromFile) {
	extern bool unitTestTaskPrecisionHack;
	unitTestTaskPrecisionHack = true;
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/nick_1.csv");
	EngineTestHelper eth (engine_type_e::VW_ABA);
	engineConfiguration->alwaysInstantRpm = true;
	eth.setTriggerType(trigger_type_e::TT_60_2_WRONG_POLARITY);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ( 0u, eth.recentWarnings()->getCount())<< "warningCounter#vwRealCranking";
	ASSERT_EQ( 1695, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}

TEST(crankingVW, crankingTwiceWithGap) {
	extern bool unitTestTaskPrecisionHack;
	unitTestTaskPrecisionHack = true;
	EngineTestHelper eth (engine_type_e::VW_ABA);
	extern bool unitTestTaskNoFastCallWhileAdvancingTimeHack;
	unitTestTaskNoFastCallWhileAdvancingTimeHack = true;
	engineConfiguration->alwaysInstantRpm = true;
	eth.setTriggerType(trigger_type_e::TT_60_2_WRONG_POLARITY);

	{
		CsvReader reader(1, /* vvtCount */ 0);

		reader.open("tests/trigger/resources/nick_1.csv");

		while (reader.haveMore()) {
			reader.processLine(&eth);
		}

		ASSERT_EQ(0u, eth.recentWarnings()->getCount())<< "warningCounter#vwRealCranking";
		ASSERT_EQ(1695, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
	}

	auto now = getTimeNowNt();

	{
		// Offset by a short time offset, 10 seconds
		CsvReader reader(1, /* vvtCount */ 0, 10);

		reader.open("tests/trigger/resources/nick_1.csv");

		while (reader.haveMore()) {
			reader.processLine(&eth);
		}

		ASSERT_EQ(0u, eth.recentWarnings()->getCount());
		ASSERT_EQ(1695, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
	}

	{
		// Offset by long time offset, 5m14.15s
		CsvReader reader(1, /* vvtCount */ 0, 314.159);

		reader.open("tests/trigger/resources/nick_1.csv");

		while (reader.haveMore()) {
			reader.processLine(&eth);
		}

		ASSERT_EQ(0u, eth.recentWarnings()->getCount());
		ASSERT_EQ(1695, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
	}
}
