// Mitsubishi 4B11 trigger pattern
// Crank: 36-2-1
// Cam: Single tooth (half moon)

#include "pch.h"

#include "engine_csv_reader.h"

TEST(real4b11, running) {
	extern bool unitTestTaskPrecisionHack;
	unitTestTaskPrecisionHack = true;
	EngineCsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/4b11-running.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	eth.setTriggerType(trigger_type_e::TT_36_2_1);

	while (reader.haveMore()) {
		reader.processLine(&eth);

		// Expect that all teeth are in the correct spot
		float angleError = getTriggerCentral()->triggerToothAngleError;
		EXPECT_TRUE(angleError < 3 && angleError > -3) << "tooth angle of " << angleError << " at timestamp " << (getTimeNowNt() / 1e8);

		reader.assertFirstRpm(1436, 30);
	}

	ASSERT_EQ(0u, eth.recentWarnings()->getCount());
}

TEST(real4b11, runningDoubledEdge) {
	extern bool unitTestTaskPrecisionHack;
	unitTestTaskPrecisionHack = true;
	EngineCsvReader reader(1, /* vvtCount */ 0);

	// This log has an extra duplicate edge at 5.393782 seconds (hand added)
	reader.open("tests/trigger/resources/4b11-running-doubled-edge.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	eth.setTriggerType(trigger_type_e::TT_36_2_1);

	while (reader.haveMore()) {
		reader.processLine(&eth);
		reader.assertFirstRpm(1436, 30);
	}

	// Should get a warning for the doubled edge, but NOT one for a trigger error!
	ASSERT_EQ(1u, eth.recentWarnings()->getCount());
	ASSERT_EQ(ObdCode::CUSTOM_PRIMARY_DOUBLED_EDGE, eth.recentWarnings()->get(0).Code);
}
