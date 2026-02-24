// Mitsubishi 4B11 trigger pattern
// Crank: 36-2-1
// Cam: Single tooth (half moon)

#include "pch.h"
#include "real_trigger_helper.h"

TEST(real4b11, running) {
	extern bool unitTestTaskPrecisionHack;
	unitTestTaskPrecisionHack = true;

	RealTriggerHelper helper;
	helper.expectedFirstRpm = 1436;
	helper.expectedFirstRpmLineIndex = 30;

	helper.runTest("tests/trigger/resources/4b11-running.csv", trigger_type_e::TT_36_2_1, 1, 0, false, 0.0, [&](CsvReader& reader) {
		// Expect that all teeth are in the correct spot
		float angleError = getTriggerCentral()->triggerToothAngleError;
		EXPECT_TRUE(angleError < 3 && angleError > -3) << "tooth angle of " << angleError << " at timestamp " << (getTimeNowNt() / 1e8);
	});

	ASSERT_EQ(0u, helper.eth.recentWarnings()->getCount());
}

TEST(real4b11, runningDoubledEdge) {
	extern bool unitTestTaskPrecisionHack;
	unitTestTaskPrecisionHack = true;

	RealTriggerHelper helper;
	helper.expectedFirstRpm = 1436;
	helper.expectedFirstRpmLineIndex = 30;

	// This log has an extra duplicate edge at 5.393782 seconds (hand added)
	helper.runTest("tests/trigger/resources/4b11-running-doubled-edge.csv", trigger_type_e::TT_36_2_1, 1, 0, false, 0.0);

	// Should get a warning for the doubled edge, but NOT one for a trigger error!
	ASSERT_EQ(1u, helper.eth.recentWarnings()->getCount());
	ASSERT_EQ(ObdCode::CUSTOM_PRIMARY_DOUBLED_EDGE, helper.eth.recentWarnings()->get(0).Code);
}
