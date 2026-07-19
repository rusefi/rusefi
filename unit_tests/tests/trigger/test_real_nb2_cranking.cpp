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

	reader.open("tests/trigger/resources/nb2-cranking-good.csv");
	EngineTestHelper eth (engine_type_e::MAZDA_MIATA_NB2);
	engineConfiguration->alwaysInstantRpm = true;

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	// VVT position nearly zero!
	EXPECT_NEAR(engine->triggerCentral.getVVTPosition(0, 0), 4.2627f, 1e-4);

	// Check the number of times VVT information was used to adjust crank phase
	// This should happen exactly once: once we sync, we shouldn't lose it.
	EXPECT_EQ(engine->triggerCentral.triggerState.phaseResyncCounter, 2);

	ASSERT_EQ(876, round(Sensor::getOrZero(SensorType::Rpm)));

	// todo: address later: +1 is a false positive CUSTOM_OBD_impossibly_short_INJECTION - injection events run with zero fuel mass in this test
	EXPECT_EQ(4u, eth.recentWarnings()->getCount());
	EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_PRIMARY_NOT_ENOUGH_TEETH));
	EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_CAM_TOO_MANY_TEETH));
	EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_OBD_impossibly_short_INJECTION));
}

TEST(realCrankingNB2, crankingMissingInjector) {
	CsvReader reader(1, /* vvtCount */ 1);

	reader.open("tests/trigger/resources/nb2-cranking-good-missing-injector-1.csv");
	EngineTestHelper eth (engine_type_e::MAZDA_MIATA_NB2);
	engineConfiguration->alwaysInstantRpm = true;

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	// VVT position nearly zero!
	EXPECT_NEAR(engine->triggerCentral.getVVTPosition(0, 0), -2.5231f, 1e-4);

	ASSERT_EQ(316, round(Sensor::getOrZero(SensorType::Rpm)));

	// todo: address later: +1 is a false positive CUSTOM_OBD_impossibly_short_INJECTION - injection events run with zero fuel mass in this test
	EXPECT_EQ(5u, eth.recentWarnings()->getCount());
	EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_PRIMARY_NOT_ENOUGH_TEETH));
	EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_CAM_TOO_MANY_TEETH));
	EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_PRIMARY_TOO_MANY_TEETH));
	EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_OBD_impossibly_short_INJECTION));
}
