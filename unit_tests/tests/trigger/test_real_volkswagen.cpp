/*
 * @file test_real_volkswagen.cpp
 *
 * @date Jun 26, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(crankingVW, vwRealCrankingFromFile) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/nick_1.csv");
	EngineTestHelper eth(engine_type_e::VW_ABA);
	engineConfiguration->alwaysInstantRpm = true;
	eth.setTriggerType(trigger_type_e::TT_60_2_VW);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	ASSERT_EQ( 0, eth.recentWarnings()->getCount())<< "warningCounter#vwRealCranking";
	ASSERT_EQ( 1695, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}

TEST(crankingVW, crankingTwiceWithGap) {
	EngineTestHelper eth(engine_type_e::VW_ABA);
	engineConfiguration->alwaysInstantRpm = true;
	eth.setTriggerType(trigger_type_e::TT_60_2_VW);

	{
		CsvReader reader(1, /* vvtCount */ 0);

		reader.open("tests/trigger/resources/nick_1.csv");
		
		while (reader.haveMore()) {
			reader.processLine(&eth);
		}

		ASSERT_EQ(0, eth.recentWarnings()->getCount())<< "warningCounter#vwRealCranking";
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

		ASSERT_EQ(0, eth.recentWarnings()->getCount());
		ASSERT_EQ(1695, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
	}

	{
		// Offset by long time offset, 5m14.15s
		CsvReader reader(1, /* vvtCount */ 0, 314.159);

		reader.open("tests/trigger/resources/nick_1.csv");
		
		while (reader.haveMore()) {
			reader.processLine(&eth);
		}

		ASSERT_EQ(0, eth.recentWarnings()->getCount());
		ASSERT_EQ(1695, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
	}
}
