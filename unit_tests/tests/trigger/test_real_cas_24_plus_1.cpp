// Mitsubishi 4B11 trigger pattern
// Crank: 36-2-1
// Cam: Single tooth (half moon)

#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(realCas24Plus1, spinningOnBench) {
	CsvReader reader(1, /* vvtCount */ 1);

	reader.open("tests/trigger/resources/cas_nissan_24_plus_1.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	engineConfiguration->isPhaseSyncRequiredForIgnition = true;

	// 24 teeth at cam speed + 1 tooth
	// AKA 12 teeth at crank speed + 1 cam tooth
	engineConfiguration->vvtMode[0] = VVT_SECOND_HALF;
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
		EXPECT_TRUE(angleError < 3 && angleError > -3) << "tooth angle of " << angleError << " at timestamp " << (getTimeNowNt() / 1e8);

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if (!gotRpm && rpm) {
			gotRpm = true;

			// We should get first RPM on exactly the first (primary) sync point - this means the instant RPM pre-sync event copy all worked OK
			EXPECT_EQ(eventCount, 7);
			EXPECT_NEAR(rpm, 808.32f, 0.1);
		}

		bool hasFullSync = getTriggerCentral()->triggerState.hasSynchronizedPhase();
		if (!gotFullSync && hasFullSync) {
			gotFullSync = true;

			// Should get full sync on the first cam tooth
			EXPECT_EQ(eventCount, 40);
			EXPECT_NEAR(rpm, 915.08f, 0.1);
		}

		float vvt = engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0);
		if (vvt != 0) {
			// cam position should never be reported outside of correct range
			EXPECT_TRUE(vvt > -10 && vvt < -9);
		}
	}

	ASSERT_EQ(0, eth.recentWarnings()->getCount());
}
