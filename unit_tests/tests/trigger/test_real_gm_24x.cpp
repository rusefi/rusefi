#include "pch.h"

#include "engine_csv_reader.h"

TEST(crankingGm24x_5, gmRealCrankingFromFile) {
	EngineCsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/gm_24x_cranking.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	extern bool unitTestTaskNoFastCallWhileAdvancingTimeHack;
	unitTestTaskNoFastCallWhileAdvancingTimeHack = true;
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	eth.setTriggerType(trigger_type_e::TT_GM_24x_5);

	while (reader.haveMore()) {
		reader.processLine(&eth);

		// Expect that all teeth are in the correct spot
		float angleError = getTriggerCentral()->triggerToothAngleError;
		EXPECT_TRUE(angleError < 3 && angleError > -3) << "tooth angle of " << angleError << " at timestamp " << (getTimeNowNt() / 1e8);

		reader.assertFirstRpm(77, 23);
	}

	ASSERT_EQ( 0u, eth.recentWarnings()->getCount())<< "warningCounter#vwRealCranking";
	ASSERT_EQ( 139, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();
}
