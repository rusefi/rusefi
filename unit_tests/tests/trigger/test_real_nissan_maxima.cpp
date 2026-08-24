/*
 * @file test_real_nissan_maxima.cpp
 *
 * Replays real trigger captures from a 2010 Nissan Maxima (VQ35DE) and
 * verifies that the trigger decoder synchronizes and reports RPM.
 */

#include "pch.h"

#include "logicdata_csv_reader.h"

static void runMaximaTest(const char* fileName, double startTimeStamp, int expectedRpm) {
	// The raw captures start dozens of seconds into the log; shift the
	// timestamps back close to zero so the CsvReader gap-filler does not
	// synthesize tens of thousands of idle events.
	CsvReader reader(1, /* vvtCount */ 1, -startTimeStamp);

	reader.open(fileName);
	EngineTestHelper eth(engine_type_e::PROTEUS_NISSAN_VQ35);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	while (reader.haveMore()) {
		reader.processLine(&eth);
	}

	// The decoder should synchronize and report a plausible cranking RPM.
	ASSERT_EQ(expectedRpm, round(Sensor::getOrZero(SensorType::Rpm))) << reader.lineIndex();
}

TEST(realNissanMaxima, file1) {
	runMaximaTest("tests/trigger/resources/gen2-VQ35DE-2010-Nissan-Maxima.teeth", 38.837777, 127);
}

TEST(realNissanMaxima, file2) {
	runMaximaTest("tests/trigger/resources/gen2-VQ35DE-2010-Nissan-Maxima-2.teeth", 45.321360, 122);
}

/*
 * VVT (cam) synchronization for these captures.
 *
 * The Cam 1 signal in these recordings is a genuine VVT_NISSAN_VQ pattern and
 * it is NOT inverted - the sync pattern lives on the physical rising edges, so
 * invertCamVVTSignal must be false here.
 *
 * With the stock VQ VVT sync gaps ([4..6] primary, [0.35..0.7] second) the cam
 * never locks: these are cranking captures and the engine accelerates within a
 * single cam revolution, which inflates the primary sync-gap ratio (nominally
 * ~5) up to ~8 - outside the tolerance window. We use triggerVVTGapOverride to
 * widen the acceptable gap ranges so the decoder can synchronize despite the
 * unstable cranking speed.
 */
static void runMaximaVvtTest(const char* fileName, double startTimeStamp) {
	CsvReader reader(1, /* vvtCount */ 1, -startTimeStamp);

	reader.open(fileName);
	EngineTestHelper eth(engine_type_e::PROTEUS_NISSAN_VQ35);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	// this signal is not inverted
	engineConfiguration->invertCamVVTSignal = false;

	// Widen the VVT sync gaps so the cam can lock during unstable cranking.
	// gap[0] is the primary sync gap (stock [4..6]); gap[1] is the second one
	// (stock [0.35..0.7]).
	engineConfiguration->overrideVvtTriggerGaps = true;
	engineConfiguration->gapVvtTrackingLengthOverride = 2;
	engineConfiguration->triggerVVTGapOverrideFrom[0] = 2;
	engineConfiguration->triggerVVTGapOverrideTo[0] = 20;
	engineConfiguration->triggerVVTGapOverrideFrom[1] = 0.2;
	engineConfiguration->triggerVVTGapOverrideTo[1] = 1.5;
	// re-apply the trigger/cam shapes so the gap override takes effect
	engine->triggerCentral.applyShapesConfiguration();

	TriggerCentral *tc = &engine->triggerCentral;
	bool everSynchronized = false;

	while (reader.haveMore()) {
		reader.processLine(&eth);
		if (tc->vvtState[0][0].getShaftSynchronized()) {
			everSynchronized = true;
		}
	}

	// Without the gap override the cam never synchronizes on these cranking
	// captures; with it the decoder locks onto the VVT_NISSAN_VQ pattern.
	ASSERT_TRUE(everSynchronized) << reader.lineIndex();
	// Once synchronized a cam position gets reported (0 would mean "never
	// decoded").
	ASSERT_NE(0.0f, tc->vvtPosition[0][0]) << reader.lineIndex();
}

TEST(realNissanMaxima, vvtFile1) {
	runMaximaVvtTest("tests/trigger/resources/gen2-VQ35DE-2010-Nissan-Maxima.teeth", 38.837777);
}

TEST(realNissanMaxima, vvtFile2) {
	runMaximaVvtTest("tests/trigger/resources/gen2-VQ35DE-2010-Nissan-Maxima-2.teeth", 45.321360);
}
