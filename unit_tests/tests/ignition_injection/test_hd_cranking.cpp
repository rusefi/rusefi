// file test_hd_cranking.cpp

#include "pch.h"
#include "logicdata_csv_reader.h"

TEST(harley, crankingSomethingCam) {
	CsvReader reader(1, /* vvtCount */ 1);
	reader.open("tests/ignition_injection/resources/hd-req-sync_3.csv");

	EngineTestHelper eth(engine_type_e::HARLEY);

  bool gotRpm = false;
  bool gotSync = false;
  bool prevSync = false;

	while (reader.haveMore()) {
		reader.processLine(&eth);

		engine->periodicSlowCallback();

    // whole pattern is a copy-paste from test_real_4b11.cpp, is better API possible?
    auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if (!gotRpm && rpm) {
			gotRpm = true;

			// We should get first RPM on exactly the first sync point - this means the instant RPM pre-sync event copy all worked OK
			EXPECT_EQ(reader.lineIndex(), 56);
			EXPECT_NEAR(rpm, 87, 1);
		}

		if (!gotSync && engine->triggerCentral.triggerState.hasSynchronizedPhase()) {
		  EXPECT_EQ(reader.lineIndex(), 83);
		  gotSync = true;
		}

		if (gotSync) {
		// we have loss of sync, interesting!
//		  EXPECT_TRUE(engine->triggerCentral.triggerState.hasSynchronizedPhase()) << "Loss " << reader.lineIndex();
		}
	}

	ASSERT_EQ(6, engine->triggerCentral.triggerState.camResyncCounter); // interesting!
}
