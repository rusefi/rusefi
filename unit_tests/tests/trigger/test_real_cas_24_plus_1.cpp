// Mitsubishi 4B11 trigger pattern
// Crank: 36-2-1
// Cam: Single tooth (half moon)

#include "pch.h"

#include "logicdata_csv_reader.h"

void testTwelvePlusOne(
		const char* file,
		float firstRpm,
		int fullSyncEventCount,
		float fullSyncRpm,
		int expectPhaseAdjustment,
		float expectVvt) {
	CsvReader reader(1, /* vvtCount */ 1);

	reader.open(file);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	engineConfiguration->isPhaseSyncRequiredForIgnition = true;

	// 24 teeth at cam speed + 1 tooth
	// AKA 12 teeth at crank speed + 1 cam tooth
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	eth.setTriggerType(trigger_type_e::TT_12_TOOTH_CRANK);

	int eventCount = 0;
	bool gotRpm = false;
	bool gotFullSync = false;

	while (reader.haveMore()) {
		reader.processLine(&eth);
		eventCount++;
		engine->rpmCalculator.onSlowCallback();

		// Expect that all teeth are in the correct spot
		float angleError = getTriggerCentral()->triggerToothAngleError;
		EXPECT_TRUE(std::abs(angleError) < 5)
				<< "tooth angle of " << angleError << " at timestamp " << (getTimeNowNt() / 1e8);

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if (!gotRpm && rpm) {
			gotRpm = true;

			// We should get first RPM on exactly the first (primary) sync point - this means the instant RPM pre-sync
			// event copy all worked OK
			EXPECT_EQ(eventCount, 7);
			EXPECT_NEAR(rpm, firstRpm, 0.1);
		}

		bool hasFullSync = getTriggerCentral()->triggerState.hasSynchronizedPhase();
		if (hasFullSync) {
			// Adjustment should be by 270 degrees
			EXPECT_FLOAT_EQ(getTriggerCentral()->triggerState.m_phaseAdjustment, expectPhaseAdjustment);

			if (!gotFullSync) {
				gotFullSync = true;

				// Should get full sync on the first cam tooth
				EXPECT_EQ(eventCount, fullSyncEventCount);
				EXPECT_NEAR(rpm, fullSyncRpm, 0.1);
			}
		}

		float vvtLegacy = engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0);
		if (vvtLegacy != 0) {
			expected<angle_t> vvt = vvtLegacy;
			// cam position should never be reported outside of correct range
			EXPECT_TRUE(vvt.Value > (expectVvt - 2) && vvt.Value < (expectVvt + 2)) << "VVT angle: " << vvt.Value;
		}
	}

	ASSERT_EQ(1, eth.recentWarnings()->getCount());
}

TEST(realCas24Plus1, spinningOnBench) {
	testTwelvePlusOne("tests/trigger/resources/cas_nissan_24_plus_1.csv", 808.32f, 40, 915.08f, 270, -9.5);
}

TEST(realCas24Plus1, longTimeIdling) {
// todo: fix me https://github.com/rusefi/rusefi/issues/9244	testTwelvePlusOne("tests/trigger/resources/twelve_plus_one_long.csv", 920.69f, 35, 926.67f, 330, -15.5);
}
