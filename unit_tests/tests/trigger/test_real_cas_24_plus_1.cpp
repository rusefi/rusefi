// Mitsubishi 4B11 trigger pattern
// Crank: 36-2-1
// Cam: Single tooth (half moon)

#include "pch.h"
#include "real_trigger_helper.h"

TEST(realCas24Plus1, spinningOnBench) {
	extern bool unitTestTaskPrecisionHack;
	unitTestTaskPrecisionHack = true;
	RealTriggerHelper helper;

	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	engineConfiguration->isPhaseSyncRequiredForIgnition = true;

	// 24 teeth at cam speed + 1 tooth
	// AKA 12 teeth at crank speed + 1 cam tooth
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;

	bool gotFullSync = false;

	helper.expectedFirstRpm = 808;
	helper.expectedFirstRpmLineIndex = 7;

	helper.runTest("tests/trigger/resources/cas_nissan_24_plus_1.csv", trigger_type_e::TT_12_TOOTH_CRANK, 1, 1, false, 0.0, [&](CsvReader& reader) {
		// Expect that all teeth are in the correct spot
		float angleError = getTriggerCentral()->triggerToothAngleError;
		EXPECT_TRUE(angleError < 3 && angleError > -3) << "tooth angle of " << angleError << " at timestamp " << (getTimeNowNt() / 1e8);

		auto rpm = Sensor::getOrZero(SensorType::Rpm);

		bool hasFullSync = getTriggerCentral()->triggerState.hasSynchronizedPhase();
		if (!gotFullSync && hasFullSync) {
			gotFullSync = true;

			// Should get full sync on the first cam tooth
			EXPECT_EQ(reader.lineIndex(), 40);
			EXPECT_NEAR(rpm, 915.08f, 0.1);
		}

		float vvt = engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0);
		if (vvt != 0) {
			// cam position should never be reported outside of correct range
			EXPECT_TRUE(vvt > -10 && vvt < -9);
		}
	});

	ASSERT_EQ(0u, helper.eth.recentWarnings()->getCount());
}
