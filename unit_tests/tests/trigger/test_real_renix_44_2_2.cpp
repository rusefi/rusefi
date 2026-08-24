/*
 * @file test_real_renix_44_2_2.cpp
 *
 * @date Jun 27, 2026
 * @author Andrey Belomutskiy, (c) 2012-2026
 */

#include "pch.h"
#include "logicdata_csv_reader.h"
#include "unit_test_logger.h"

extern int tooManyTeethCounter;

TEST(triggerRenix44, renix44RealCrankingFromFile) {
	tooManyTeethCounter = 0;

	ScopedLogFileSizeLimit sz(32 * 1024 * 1024);
	// It's a long trace, let's not waste time and disk space on logs
	ScopedUnitTestCreateLogs sz2(false);
	CsvReader reader(1, /* vvtCount */ 0);
	// The recording is a rusEFI internal CSV log:
	// PriLevel,SecLevel,Trigger,Sync,Coil,Injector,Time,ToothTime
	reader.readingOffset = 0; // use PriLevel (column 0) as primary trigger
	reader.timestampColumnIndex = 6; // 'Time' column holds the timestamp
	reader.timestampScale = 0.001; // timestamps are in milliseconds
	// recording starts at ~36.7s of ECU uptime; rebase to zero to avoid 32-bit
	// mock-clock overflow during us->NT conversion
	reader.normalizeTimestamps = true;

	reader.open("tests/trigger/resources/renix_44_2_2.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->alwaysInstantRpm = true;
	eth.setTriggerType(trigger_type_e::TT_RENIX_44_2_2);

	// capture the RPM reported at the very first synchronization point, like
	// the other 'real cranking from file' tests do
	float firstSyncRpm = 0;
	while (reader.haveMore()) {
		reader.processLine(&eth);
		if (firstSyncRpm == 0) {
			firstSyncRpm = Sensor::getOrZero(SensorType::Rpm);
		}
	}

	// the first sync point reports the running RPM right away (~1390)
	ASSERT_NEAR(1389.81, firstSyncRpm, 0.5) << "renix44 first sync RPM";

	// The decoder is able to synchronize on the 44-2-2 pattern and tracks the
	// engine running at ~1389 RPM (22 teeth per symmetrical 180 deg half, ~0.967
	// ms per tooth). Note: the second half of this recording contains periodic
	// ~37 ms logging drop-outs which legitimately cost the decoder sync (and a
	// handful of trigger errors) until it re-synchronizes, so we do not assert a
	// zero error/warning count here.
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "renix44 shaft sync";
	ASSERT_NEAR(1389, Sensor::getOrZero(SensorType::Rpm), 5) << "renix44 RPM";
	ASSERT_EQ(84, tooManyTeethCounter);
}

TEST(triggerRenix44, renix44RealCrankingFromFileAnotherOne) {
	tooManyTeethCounter = 0;

	ScopedLogFileSizeLimit sz(32 * 1024 * 1024);
	// It's a long trace, let's not waste time and disk space on logs
	ScopedUnitTestCreateLogs sz2(false);
	CsvReader reader(1, /* vvtCount */ 0);
	// The recording is a rusEFI internal CSV log:
	// PriLevel,SecLevel,Trigger,Sync,Coil,Injector,Time,ToothTime
	reader.readingOffset = 0; // use PriLevel (column 0) as primary trigger
	reader.timestampColumnIndex = 6; // 'Time' column holds the timestamp
	reader.timestampScale = 0.001; // timestamps are in milliseconds
	// rebase to zero to avoid 32-bit mock-clock overflow during us->NT conversion
	reader.normalizeTimestamps = true;

	reader.open("tests/trigger/resources/renix_44_2_2.csv-another-one.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->alwaysInstantRpm = true;
	eth.setTriggerType(trigger_type_e::TT_RENIX_44_2_2);

	float firstSyncRpm = 0;
	while (reader.haveMore()) {
		reader.processLine(&eth);
		if (firstSyncRpm == 0) {
			firstSyncRpm = Sensor::getOrZero(SensorType::Rpm);
		}
	}

	// this is a different cranking capture: it syncs at a higher instant RPM and
	// settles to a low cranking RPM by the end of the trace
	ASSERT_NEAR(556.71, firstSyncRpm, 0.5) << "renix44 first sync RPM";
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized()) << "renix44 shaft sync";
	ASSERT_NEAR(219, Sensor::getOrZero(SensorType::Rpm), 5) << "renix44 RPM";

	ASSERT_EQ(3, tooManyTeethCounter);
}
