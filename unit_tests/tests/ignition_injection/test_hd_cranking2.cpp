// file test_hd_cranking2.cpp
//

#include "pch.h"
#include "engine_csv_reader.h"
//#include "msl_to_csv.h"
#include "unit_test_logger.h"

static void runCrankGapCrankingSyncData2(const char *fileName,
		int expectedSyncAtIndex,
		int expectedCamResyncCounter,
		int expectedFirstPhaseSyncAtIndex) {
	EngineTestHelper eth(engine_type_e::HARLEY);

	engineConfiguration->vvtMode[0] = VVT_MAP_V_TWIN;

	engine->triggerCentral.applyShapesConfiguration();

	// HARLEY uses a crank-only wheel, so the trigger waveform itself cannot
	// disambiguate the engine phase - we must wait until cam-driven sync
	// fires. Right after applyShapesConfiguration() the bit must be cleared.
	EXPECT_FALSE(engine->triggerCentral.triggerState.hasSynchronizedPhase());

	EngineCsvReader reader(1, /* vvtCount */ 0);
	reader.open(fileName);

	while (reader.haveMore()) {
		reader.processLine(&eth);

		reader.assertSyncAtIndex(&eth, expectedSyncAtIndex);

		// Synthetic phase-sync invocation: at the prescribed CSV line index,
		// force the engine-phase divider so subsequent logic sees a
		// phase-synced state. Mirrors the cam-driven sync that the fw-iws
		// crank-gap mode would produce at this point.
		if (!reader.gotPhaseSync
				&& reader.lineIndex() == expectedFirstPhaseSyncAtIndex) {
			auto shift = engine->triggerCentral.syncEnginePhaseAndReport(2, 0);

			// After phase sync, instant RPM is NO LONGER reset to 0. Instead, the history buffer is rotated
			// to match the new engine phase, preserving RPM calculation and avoiding a gap in engine control.
			if (shift != 0) {
				// instantRpm is used for immediate event scheduling and should remain valid
				EXPECT_NE(0, engine->triggerCentral.instantRpm.getInstantRpm());

				// rpmCalculator is the main RPM source; it is NOT reset by phase sync.
				// It keeps the last known good cycle-average RPM until the next TDC.
				EXPECT_NE(0, engine->rpmCalculator.getCachedRpm());
				EXPECT_EQ(SPINNING_UP, engine->rpmCalculator.getState());
			}
		}

		reader.assertPhaseSyncAtIndex(&eth, expectedFirstPhaseSyncAtIndex);
	}
	EXPECT_TRUE(reader.gotSync);
	EXPECT_TRUE(reader.gotPhaseSync);
	EXPECT_EQ(expectedCamResyncCounter,
			eth.engine.triggerCentral.triggerState.camResyncCounter);

	// Total injection / spark counts observed at end of CSV playback.
	// Captured from the data points "Fuel: Injection counter"
	// (engineState.fuelInjectionCounter) and "Ign: Spark counter"
	// (engineState.globalSparkCounter) visible in the TS log screenshot.
	EXPECT_EQ(8, (int)engine->engineState.fuelInjectionCounter);
	EXPECT_EQ(8, (int)engine->engineState.globalSparkCounter);
}

TEST(harleyCrank, orange2truncated) {
//	setUnitTestCreateLogs(true);
	{
		runCrankGapCrankingSyncData2(
				"tests/ignition_injection/resources/orange-2-first-9-seconds.csv",
				41, 1, 139);
	}
//	setUnitTestCreateLogs(false);

	// EngineTestHelper destructor above already closed the .msl file via
	// closeUnitTestLog(). Convert it to a sibling CSV so the log can be
	// inspected without TunerStudio.
// 	const char* mslPath = "test_results/unittest_harleyCrank_orange2truncated.msl";
// 	const char* csvPath = "test_results/unittest_harleyCrank_orange2truncated.csv";
// 	EXPECT_TRUE(convertMslToCsv(mslPath, csvPath));
}
