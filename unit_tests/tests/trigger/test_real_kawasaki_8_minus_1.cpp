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
 * The "without spark plug" captures (no compression, smooth rotation) decode
 * fine. The "with spark plug" captures (full compression) currently NEVER
 * achieve shaft sync - that is the issue #10106 symptom this test documents.
 * Once gap detection handles heavy intra-revolution speed swings, the
 * expectSync=false expectations below should flip to true.
 *
 * See tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/readme.md
 */

#include "pch.h"

#include "logicdata_csv_reader.h"

static void runKawasakiTest(const char* fileName, bool expectSync,
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
		// ~300 RPM starter cranking; anything near 2x means the wheel got
		// decoded at cam speed, 0 means we lost RPM entirely
		EXPECT_GT(maxRpm, 250) << fileName;
		EXPECT_LT(maxRpm, 400) << fileName;
	}
}

// full compression: tooth-to-tooth intervals vary too much for gap detection,
// the decoder never syncs - see issue #10106
TEST(realKawasaki8minus1, withSparkPlug10) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/with spark plug/re_10.teeth",
			false, -1, -1);
}

TEST(realKawasaki8minus1, withSparkPlug11) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/with spark plug/re_11.teeth",
			false, -1, -1);
}

TEST(realKawasaki8minus1, withSparkPlug12) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/with spark plug/re_12.teeth",
			false, -1, -1);
}

// spark plugs removed, no compression: decoder syncs and reports correct RPM
TEST(realKawasaki8minus1, withoutSparkPlug18) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/without spark plug/re_18.teeth",
			true, 12, 285.71f);
}

TEST(realKawasaki8minus1, withoutSparkPlug19) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/without spark plug/re_19.teeth",
			true, 9, 310.54f);
}

TEST(realKawasaki8minus1, withoutSparkPlug20) {
	runKawasakiTest("tests/trigger/resources/kawasaki_8-1_trigger_teeth_logs/without spark plug/re_20.teeth",
			true, 22, 304.33f);
}
