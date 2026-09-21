/*
 * @file test_real_bmw_e90_cam.cpp
 *
 * Replays a real BMW E90 (N52) capture: 60-2 crank plus both cam sensors,
 * cranking through idle. The N52 preset decodes both cams as
 * VVT_BMW_VANOS_RELUCTOR.
 */

#include "pch.h"

#include "logicdata_csv_reader.h"

static void checkColdStartCamGap(int cam) {
	EngineTestHelper eth(engine_type_e::BMW_N52);
	Sensor::setMockValue(SensorType::Rpm, 250);
	auto& tc = engine->triggerCentral;
	auto& decoder = tc.vvtState[0][cam];
	const auto& shape = tc.vvtShape[cam];
	const auto& configuration = tc.vvtTriggerConfiguration[cam];
	ASSERT_FALSE(engineConfiguration->overrideVvtTriggerGaps);
	ASSERT_EQ(6u, shape.getSize());
	decoder.resetState();

	// Falling-edge intervals from 2026-09-16_18.42.09.mlg, ~14.308..15.649 s.
	// These are the ECU's microsecond measurements, not differences between
	// the MLG's ~10 ms sample timestamps. No private log file is needed to run.
	constexpr int gapsUs[2][11] = {
		{193731, 139354, 193431, 282899, 108226, 137350, 134050, 77509, 94824, 107126, 66942},
		{196068, 139251, 193918, 283018, 107643, 137448, 134071, 77354, 94921, 107117, 66889},
	};
	auto edge = [&](trigger_event_e signal, int delayUs) {
		eth.moveTimeForwardUs(delayUs);
		decoder.decodeTriggerEvent("cold start", shape, nullptr, configuration, signal, getTimeNowNt());
	};
	auto gap = [&](int durationUs) {
		// Rising-edge positions are synthetic: this decoder synchronizes on falls.
		// Always supply exactly one rise and one fall, with no extra/noisy edges.
		edge(SHAFT_PRIMARY_RISING, durationUs / 2);
		edge(SHAFT_PRIMARY_FALLING, durationUs - durationUs / 2);
	};
	gap(1000);
	gap(gapsUs[cam][0]);
	gap(gapsUs[cam][1]);
	ASSERT_TRUE(decoder.getShaftSynchronized());
	ASSERT_EQ(0, eth.getWarningCounter());

	gap(gapsUs[cam][2]);
	gap(gapsUs[cam][3]);
	ASSERT_TRUE(decoder.getShaftSynchronized());
	ASSERT_EQ(4, decoder.getCurrentIndex());

	// Current behavior: the short-after-long ratio drops just below 0.4.
	// A complete six-edge cycle is rejected even though no edges were added.
	gap(gapsUs[cam][4]);
	EXPECT_NEAR(static_cast<float>(gapsUs[cam][4]) / gapsUs[cam][3], decoder.triggerSyncGapRatio, 1e-6);
	EXPECT_FALSE(decoder.getShaftSynchronized());
	EXPECT_EQ(6, decoder.getCurrentIndex());
	EXPECT_EQ(1, eth.getWarningCounter());
	EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_CAM_TOO_MANY_TEETH));
	EXPECT_EQ(1u, decoder.totalTriggerErrorCounter);
	EXPECT_EQ(0u, tc.triggerState.totalTriggerErrorCounter);

	gap(gapsUs[cam][5]);
	gap(gapsUs[cam][6]);
	gap(gapsUs[cam][7]);
	EXPECT_TRUE(decoder.getShaftSynchronized());
	EXPECT_EQ(6, decoder.triggerCountersError) << "One missed cycle, not six additional warnings";
	EXPECT_EQ(1, eth.getWarningCounter());

	gap(gapsUs[cam][8]);
	gap(gapsUs[cam][9]);
	gap(gapsUs[cam][10]);
	EXPECT_TRUE(decoder.getShaftSynchronized());
	EXPECT_EQ(0, decoder.triggerCountersError);
	EXPECT_EQ(0u, decoder.orderingErrorCounter);
	EXPECT_EQ(1, decoder.getSynchronizationCounter());
	EXPECT_EQ(1, eth.getWarningCounter());
}

TEST(realBmwE90, ColdStartIntakeGapWarns9004AndRecovers) {
	checkColdStartCamGap(0);
}

TEST(realBmwE90, ColdStartExhaustGapWarns9004AndRecovers) {
	checkColdStartCamGap(1);
}

TEST(realBmwE90, realCams) {
	// the 8.6 second capture overflows the default 16MB per-test log cap
	ScopedUnitTestCreateLogs logDisabler(false);

	CsvReader reader(1, /* vvtCount */ 2, -38.290885);
	// single bank, intake + exhaust cams
	reader.twoBanksSingleCamMode = false;

	reader.open("tests/trigger/resources/real-bmw-e90.teeth");
	EngineTestHelper eth(engine_type_e::BMW_N52);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	TriggerCentral *tc = &engine->triggerCentral;

	bool everSynced[2] = {false, false};
	int positionUpdates[2] = {0, 0};
	float lastSeenPosition[2] = {0, 0};

	int maxSyncCounter[2] = {0, 0};
	while (reader.haveMore()) {
		reader.processLine(&eth);
		for (int cam = 0; cam < 2; cam++) {
			if (tc->vvtState[0][cam].getShaftSynchronized()) {
				everSynced[cam] = true;
			}
			maxSyncCounter[cam] = std::max(maxSyncCounter[cam], tc->vvtState[0][cam].getSynchronizationCounter());
			float pos = tc->vvtPosition[0][cam];
			if (pos != 0 && pos != lastSeenPosition[cam]) {
				lastSeenPosition[cam] = pos;
				positionUpdates[cam]++;
			}
		}
	}

	// note: the crank signal itself is rough in this capture (unstable early cranking, engine
	// dies at the end); crank re-sync resets the cam decoders, so cam sync counters stay modest
	printf("E90DIAG rpm=%.1f crank syncCounter=%d errors=%u\n",
			Sensor::getOrZero(SensorType::Rpm),
			tc->triggerState.getSynchronizationCounter(),
			(unsigned)tc->triggerState.totalTriggerErrorCounter);
	for (int cam = 0; cam < 2; cam++) {
		printf("E90DIAG cam%d everSynced=%d maxSyncCounter=%d errors=%u posUpdates=%d lastPos=%.2f\n",
				cam,
				everSynced[cam],
				maxSyncCounter[cam],
				(unsigned)tc->vvtState[0][cam].totalTriggerErrorCounter,
				positionUpdates[cam],
				lastSeenPosition[cam]);

		EXPECT_TRUE(everSynced[cam]) << "cam " << cam;
		EXPECT_GE(maxSyncCounter[cam], 5) << "cam " << cam;
		EXPECT_GE(positionUpdates[cam], 20) << "cam " << cam;
		EXPECT_LE(tc->vvtState[0][cam].totalTriggerErrorCounter, 20u) << "cam " << cam;
	}
}
