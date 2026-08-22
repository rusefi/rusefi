/*
 * @file test_real_kawasaki_8_minus_1.cpp
 *
 * Replays real cranking captures from a Kawasaki engine with an 8-1 crank
 * trigger wheel (7 physical teeth, one missing-tooth gap per revolution).
 *
 * Captured for https://github.com/rusefi/rusefi/issues/10106 "crank by edge
 * for low count triggers on high comp engines": with so few teeth on a
 * high-compression engine, crank speed fluctuates heavily within a single
 * revolution during cranking (tooth-to-tooth intervals vary by more than 3x),
 * which challenges missing-tooth gap detection.
 *
 * Measured gap ratios in these captures:
 * - "without spark plug" (no compression, smooth rotation): missing-tooth
 *   ratio ~1.9..2.15, close to the nominal 2.0 -> stock gaps decode fine.
 * - "with spark plug" (full compression): the crank nearly stalls into the
 *   compression stroke right at the gap and accelerates out of it, so the
 *   missing-tooth ratio measures ~3.45 and the tooth-before-gap ratio ~0.56.
 *   Both are outside the stock 8-1 windows ([1.5..2.5] and [0.75..1.25]),
 *   so the decoder NEVER syncs - that is the issue #10106 symptom the
 *   stock-gap tests below document. Once gap detection handles heavy
 *   intra-revolution speed swings, flip those expectSync=false to true.
 *
 * The customGaps* tests demonstrate the tune-level workaround: widened
 * triggerGapOverride windows make every capture sync. The margins are thin
 * but real: largest non-gap ratio observed is ~1.64 (mid-revolution
 * acceleration tooth) vs the 1.7 lower bound; a real tune needs the window
 * to span both smooth running (~2.0) and compression cranking (~3.45).
 *
 * See tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/readme.md
 */

#include "pch.h"

#include "logicdata_csv_reader.h"

static void runKawasakiTest(const char* fileName, bool useCustomGaps, bool expectSync,
		int expectedSyncAtLine, float expectedRpmAtSync) {
	CsvReader reader(1, /* vvtCount */ 0);
	// captures start dozens of seconds into ECU uptime
	reader.normalizeTimestamps = true;

	reader.open(fileName);
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	// 8-1 wheel on the crank; TEST_ENGINE inherits skippedWheelOnCam=true from
	// setDefaultBaseEngine, so an explicit crank-speed override is required -
	// without it every RPM below doubles
	engineConfiguration->skippedWheelOnCam = false;
	engineConfiguration->trigger.customTotalToothCount = 8;
	engineConfiguration->trigger.customSkippedToothCount = 1;

	if (useCustomGaps) {
		// widened windows covering both the smooth ~2.0 ratio and the
		// compression-cranking ~3.45 ratio, see file header comment
		engineConfiguration->overrideTriggerGaps = true;
		engineConfiguration->gapTrackingLengthOverride = 2;
		engineConfiguration->triggerGapOverrideFrom[0] = 1.7f;
		engineConfiguration->triggerGapOverrideTo[0] = 4.2f;
		engineConfiguration->triggerGapOverrideFrom[1] = 0.4f;
		engineConfiguration->triggerGapOverrideTo[1] = 1.4f;
	}
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);

	bool gotSync = false;
	float maxRpm = 0;

	while (reader.haveMore()) {
		reader.processLine(&eth);

		if (!gotSync && engine->triggerCentral.triggerState.getShaftSynchronized()) {
			gotSync = true;
			EXPECT_EQ(reader.lineIndex(), expectedSyncAtLine);
			EXPECT_NEAR(Sensor::getOrZero(SensorType::Rpm), expectedRpmAtSync, 1);
		}
		maxRpm = std::max(maxRpm, Sensor::getOrZero(SensorType::Rpm));
	}

	EXPECT_EQ(gotSync, expectSync) << fileName;

	if (expectSync) {
		// ~300 RPM starter cranking; under full compression the instant RPM
		// ripples up to ~430 within a revolution, which is legitimate - but
		// anything near 2x average (~600) means the wheel got decoded at cam
		// speed, and 0 means we lost RPM entirely
		EXPECT_GT(maxRpm, 250) << fileName;
		EXPECT_LT(maxRpm, 500) << fileName;

		// no mid-crank sync losses: over the ~30 recorded revolutions the only
		// acceptable loss is the hard deceleration when the starter disengages
		// at the very end of a cranking attempt. This also proves the wide
		// custom windows still lock onto the true missing tooth - a wrong sync
		// point would fail the gap check on every following revolution.
		EXPECT_LE(engine->triggerCentral.triggerState.totalTriggerErrorCounter, 1u) << fileName;
	}
}

// full compression with stock 8-1 gap windows: the decoder never syncs - this
// documents issue #10106
TEST(realKawasaki8minus1, withSparkPlug10) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/with spark plug/re_10.teeth",
			false, false, -1, -1);
}

TEST(realKawasaki8minus1, withSparkPlug11) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/with spark plug/re_11.teeth",
			false, false, -1, -1);
}

TEST(realKawasaki8minus1, withSparkPlug12) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/with spark plug/re_12.teeth",
			false, false, -1, -1);
}

// spark plugs removed, no compression: stock gaps sync and report correct RPM
TEST(realKawasaki8minus1, withoutSparkPlug18) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/without spark plug/re_18.teeth",
			false, true, 12, 285.71f);
}

TEST(realKawasaki8minus1, withoutSparkPlug19) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/without spark plug/re_19.teeth",
			false, true, 9, 310.54f);
}

TEST(realKawasaki8minus1, withoutSparkPlug20) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/without spark plug/re_20.teeth",
			false, true, 22, 304.33f);
}

// same captures with widened custom gap windows (overrideTriggerGaps) - the
// tune-level workaround for issue #10106: every capture syncs, including full
// compression
TEST(realKawasaki8minus1, customGapsWithSparkPlug10) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/with spark plug/re_10.teeth",
			true, true, 12, 252.09f);
}

TEST(realKawasaki8minus1, customGapsWithSparkPlug11) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/with spark plug/re_11.teeth",
			true, true, 18, 285.60f);
}

TEST(realKawasaki8minus1, customGapsWithSparkPlug12) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/with spark plug/re_12.teeth",
			true, true, 18, 281.80f);
}

// custom gaps do not disturb the no-compression captures: same sync point and
// RPM as with stock gaps
TEST(realKawasaki8minus1, customGapsWithoutSparkPlug18) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/without spark plug/re_18.teeth",
			true, true, 12, 285.71f);
}

TEST(realKawasaki8minus1, customGapsWithoutSparkPlug19) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/without spark plug/re_19.teeth",
			true, true, 9, 310.54f);
}

TEST(realKawasaki8minus1, customGapsWithoutSparkPlug20) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/without spark plug/re_20.teeth",
			true, true, 22, 304.33f);
}
