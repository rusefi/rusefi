// file test_hd_cranking.cpp

#include "pch.h"
#include "engine_csv_reader.h"

TEST(harley, hdCrankingWithCam) {
	EngineCsvReader reader(1, /* vvtCount */ 1);
	reader.open("tests/ignition_injection/resources/hd-req-sync_3.csv");

	EngineTestHelper eth(engine_type_e::HARLEY);
	engineConfiguration->vvtMode[0] = VVT_BOSCH_QUICK_START;
	eth.applyTriggerWaveform();

	while (reader.haveMore()) {
		reader.processLine(&eth);

    reader.assertFirstRpm(184, 60);
    auto rpm = Sensor::getOrZero(SensorType::Rpm);

		if (!reader.gotSync && engine->triggerCentral.triggerState.hasSynchronizedPhase()) {
		  EXPECT_EQ(reader.lineIndex(), 269);
		  reader.gotSync = true;
		}

		if (reader.gotSync) {
		// we have loss of sync, interesting!
//		  EXPECT_TRUE(engine->triggerCentral.triggerState.hasSynchronizedPhase()) << "Loss " << reader.lineIndex();
		}
	}

	ASSERT_EQ(2, engine->triggerCentral.triggerState.camResyncCounter); // interesting!
}
