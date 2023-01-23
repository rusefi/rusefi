// Mitsubishi 4B11 trigger pattern
// Crank: 36-2-1
// Cam: Single tooth (half moon)

#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(real4g63, cranking) {
	CsvReader reader(1, /* vvtCount */ 1);

	reader.open("tests/trigger/resources/4g63-cranking.csv");
	EngineTestHelper eth(TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	engineConfiguration->isPhaseSyncRequiredForIgnition = true;

	engineConfiguration->vvtMode[0] = VVT_MITSUBISHI_4G63;

	engineConfiguration->trigger.customTotalToothCount = 2;
	engineConfiguration->trigger.customSkippedToothCount = 0;
	engineConfiguration->skippedWheelOnCam = false;
	eth.setTriggerType(TT_TOOTHED_WHEEL);

	int eventCount = 0;
	bool gotRpm = false;
	bool gotSync = false;

	while (reader.haveMore()) {
		reader.processLine(&eth);
		eventCount++;
		engine->rpmCalculator.onSlowCallback();

		// Expect that all teeth are in the correct spot

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if (!gotRpm && rpm) {
			gotRpm = true;

			// We should get first RPM on exactly the first sync point - this means the instant RPM pre-sync event copy all worked OK
			EXPECT_EQ(eventCount, 6);
			EXPECT_NEAR(rpm, 132.77f, 0.1);
		}

		if (!gotSync && engine->triggerCentral.triggerState.hasSynchronizedPhase()) {
			gotSync = true;

			EXPECT_EQ(eventCount, 13);
			EXPECT_NEAR(rpm, 191.45f, 0.1);
		}
	}

	ASSERT_EQ(0, eth.recentWarnings()->getCount());
}
