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
