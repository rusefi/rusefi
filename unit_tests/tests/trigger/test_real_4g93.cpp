// Mitsubishi 4G93 trigger pattern
// Crank: 2 teeth per rev (4 teeth per cycle)
// Cam: 4G92/93/94 pattern (big tooth - small tooth)

#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(real4g93, cranking) {
	CsvReader reader(1, /* vvtCount */ 1);

	reader.open("tests/trigger/resources/4g93-cranking.csv");
	EngineTestHelper eth(TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	engineConfiguration->isPhaseSyncRequiredForIgnition = true;

	engineConfiguration->vvtMode[0] = VVT_MITSUBISHI_4G63;

	engineConfiguration->trigger.customTotalToothCount = 2;
	engineConfiguration->trigger.customSkippedToothCount = 0;
	engineConfiguration->skippedWheelOnCam = false;
	eth.setTriggerType(TT_TOOTHED_WHEEL);

	bool gotRpm = false;
	bool gotSync = false;

	while (reader.haveMore()) {
		reader.processLine(&eth);

		// Expect that all teeth are in the correct spot

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if (!gotRpm && rpm) {
			gotRpm = true;

			// We should get first RPM on exactly the first sync point - this means the instant RPM pre-sync event copy all worked OK
			EXPECT_EQ(reader.lineIndex(), 6);
			EXPECT_NEAR(rpm, 132.77f, 0.1);
		}

		if (!gotSync && engine->triggerCentral.triggerState.hasSynchronizedPhase()) {
			gotSync = true;

			EXPECT_EQ(reader.lineIndex(), 17);
			EXPECT_NEAR(rpm, 204.01f, 0.1);
		}
	}

	ASSERT_TRUE(gotRpm);
	ASSERT_TRUE(gotSync);

	ASSERT_EQ(0, eth.recentWarnings()->getCount());
}

TEST(real4g93, crankingCamOnly) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/4g93-cranking-cam-only.csv");
	EngineTestHelper eth(TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	eth.setTriggerType(TT_MITSU_4G9x_CAM);

	bool gotRpm = false;
	bool gotSync = false;

	while (reader.haveMore()) {
		reader.processLine(&eth);

		// Expect that all teeth are in the correct spot
		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if (!gotRpm && rpm) {
			gotRpm = true;

			// We should get first RPM on exactly the first sync point - this means the instant RPM pre-sync event copy all worked OK
			EXPECT_EQ(reader.lineIndex(), 17);
			EXPECT_NEAR(rpm, 194.61f, 0.1);
		}

		if (!gotSync && engine->triggerCentral.triggerState.getShaftSynchronized() && engine->triggerCentral.triggerState.hasSynchronizedPhase()) {
			gotSync = true;

			EXPECT_EQ(reader.lineIndex(), 17);
		}
	}

	ASSERT_TRUE(gotRpm);
	ASSERT_TRUE(gotSync);

	ASSERT_EQ(0, eth.recentWarnings()->getCount());
}
