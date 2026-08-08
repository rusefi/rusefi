/*
 * @file test_real_bmw_e90_cam.cpp
 *
 * Replays a real BMW E90 (N52) capture: 60-2 crank plus both cam sensors,
 * cranking through idle. The N52 preset decodes both cams as
 * VVT_BMW_VANOS_RELUCTOR.
 */

#include "pch.h"

#include "logicdata_csv_reader.h"

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

		// VVT_BOSCH_QUICK_START never completed a single cam cycle on this capture;
		// VVT_BMW_VANOS_RELUCTOR holds sync over consecutive cycles during the stable idle stretch
		EXPECT_TRUE(everSynced[cam]) << "cam " << cam;
		EXPECT_GE(maxSyncCounter[cam], 5) << "cam " << cam;
		EXPECT_GE(positionUpdates[cam], 20) << "cam " << cam;
		EXPECT_LE(tc->vvtState[0][cam].totalTriggerErrorCounter, 20u) << "cam " << cam;
	}
}
