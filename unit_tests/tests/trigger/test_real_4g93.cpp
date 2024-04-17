// Mitsubishi 4G93 trigger pattern
// Crank: 2 teeth per rev (4 teeth per cycle)
// Cam: 4G92/93/94 pattern (big tooth - small tooth)

#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(real4g93, cranking) {
	CsvReader reader(1, /* vvtCount */ 1);

	reader.open("tests/trigger/resources/4g93-cranking.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	engineConfiguration->isPhaseSyncRequiredForIgnition = true;

	engineConfiguration->vvtMode[0] = VVT_MITSUBISHI_4G63;

	eth.setTriggerType(trigger_type_e::TT_MITSU_4G63_CRANK);

	bool gotRpm = false;
	bool gotSync = false;

	static const float gapRatios[2][4] = {
		{ 0, NAN, INFINITY, 0.89f }, // no sync
		{ 0.4f, 3.788f, 0.62f, 1.02f }
	};
	static const int gapRatioIndices[12] = { 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3 };

	for (int idx = 0; reader.haveMore(); idx++) {
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
//		float instantRpm = engine->triggerCentral.instantRpm.getInstantRpm();
// not looking too bad horrible		printf("rpm=%f instant=%f\n", rpm, instantRpm);

		float vvtSyncGapRatio = engine->triggerCentral.triggerState.vvtSyncGapRatio;
		float gapRatio = gapRatios[idx < 12 ? 0 : 1][gapRatioIndices[idx % 12]];
		if (isnan(gapRatio)) {
			EXPECT_TRUE(isnan(vvtSyncGapRatio));
		} else if (isinf(gapRatio)) {
			EXPECT_TRUE(isinf(vvtSyncGapRatio));
		}else {
			EXPECT_NEAR(vvtSyncGapRatio, gapRatio, 0.1);
		}
	}

	ASSERT_TRUE(gotRpm);
	ASSERT_TRUE(gotSync);

	ASSERT_EQ(0, eth.recentWarnings()->getCount());
}

TEST(real4g93, crankingOn11) {
	CsvReader reader(2, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/4g93-cranking.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;

	engineConfiguration->isPhaseSyncRequiredForIgnition = true;

	eth.setTriggerType(trigger_type_e::TT_MAZDA_MIATA_NA);

	bool gotRpm = false;
	while (reader.haveMore()) {
		reader.processLine(&eth);

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if (!gotRpm && rpm) {
			gotRpm = true;

			// We should get first RPM on exactly the first sync point - this means the instant RPM pre-sync event copy all worked OK
			EXPECT_EQ(reader.lineIndex(), 7);
			EXPECT_NEAR(rpm, 168.43f, 0.1);
		}

//		float instantRpm = engine->triggerCentral.instantRpm.getInstantRpm();
//		printf("%d rpm=%f instant=%f\n", reader.lineIndex(), rpm, instantRpm);
	}
}

TEST(real4g93, crankingCamOnly) {
	CsvReader reader(1, /* vvtCount */ 0);

	reader.open("tests/trigger/resources/4g93-cranking-cam-only.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	eth.setTriggerType(trigger_type_e::TT_MITSU_4G9x_CAM);

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

	ASSERT_EQ(1, eth.recentWarnings()->getCount());
}
