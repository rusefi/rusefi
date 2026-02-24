#include "pch.h"
#include "real_trigger_helper.h"

TEST(crankingGm24x_5, gmRealCrankingFromFile) {
	extern bool unitTestTaskNoFastCallWhileAdvancingTimeHack;
	unitTestTaskNoFastCallWhileAdvancingTimeHack = true;

	RealTriggerHelper helper;
	helper.expectedFirstRpm = 77;
	helper.expectedFirstRpmLineIndex = 23;

	helper.runTest("tests/trigger/resources/gm_24x_cranking.csv", trigger_type_e::TT_GM_24x_5, 1, 0, false, 0.0, [&](CsvReader& reader) {
		// Expect that all teeth are in the correct spot
		float angleError = getTriggerCentral()->triggerToothAngleError;
		EXPECT_TRUE(angleError < 3 && angleError > -3) << "tooth angle of " << angleError << " at timestamp " << (getTimeNowNt() / 1e8);
	});

	ASSERT_EQ(0u, helper.eth.recentWarnings()->getCount()) << "warningCounter#vwRealCranking";
	ASSERT_EQ(139, round(Sensor::getOrZero(SensorType::Rpm)));
}
