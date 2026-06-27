/*
 * @file test_real_renix_44_2_2.cpp
 *
 * @date Jun 27, 2026
 * @author Andrey Belomutskiy, (c) 2012-2026
 */

#include "pch.h"
#include "logicdata_csv_reader.h"
#include "unit_test_logger.h"

TEST(triggerRenix44, renix44RealCrankingFromFile) {
	ScopedLogFileSizeLimit sz(32 * 1024 * 1024);
	// It's a long trace, let's not waste time and disk space on logs
	ScopedUnitTestCreateLogs sz2(false);
	CsvReader reader(1, /* vvtCount */ 0);
	reader.readingOffset = 0; // use PriLevel as primary trigger

	reader.open("tests/trigger/resources/renix_44_2_2.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->alwaysInstantRpm = true;
	eth.setTriggerType(trigger_type_e::TT_RENIX_44_2_2);

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

// todo: actually figure out state of trigger
// 	ASSERT_EQ(0u, eth.recentWarnings()->getCount()) << "warningCounter#renix44RealCranking";
// 	ASSERT_NEAR(614.5, Sensor::getOrZero(SensorType::Rpm), 0.5);
}
