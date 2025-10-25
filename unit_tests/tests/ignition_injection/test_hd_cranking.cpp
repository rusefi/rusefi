// file test_hd_cranking.cpp

#include "pch.h"
#include "engine_csv_reader.h"

class HdCranking {
public:
	HdCranking(const char *fileName) :
			reader(1, /* vvtCount */1), eth(engine_type_e::HARLEY) {
		engineConfiguration->vvtMode[0] = VVT_BOSCH_QUICK_START;
		eth.applyTriggerWaveform();
		reader.open(fileName);
	}

	EngineCsvReader reader;
	EngineTestHelper eth;
};

static void runTest(const char *fileName) {
	HdCranking test(fileName);

	EngineCsvReader &reader = test.reader;

	while (reader.haveMore()) {
		reader.processLine(&test.eth);
	}
}

TEST(harley, hdCrankingWithCam1) {
	runTest("tests/ignition_injection/resources/hd-req-sync_1.csv");
}

TEST(harley, hdCrankingWithCam3) {
	HdCranking test("tests/ignition_injection/resources/hd-req-sync_3.csv");
	extern bool unitTestTaskNoFastCallWhileAdvancingTimeHack;
	unitTestTaskNoFastCallWhileAdvancingTimeHack = true;

	EngineCsvReader &reader = test.reader;

	while (reader.haveMore()) {
		reader.processLine(&test.eth);

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

TEST(harley, hdCrankingWithCam4) {
	runTest("tests/ignition_injection/resources/hd-req-sync_4.csv");
}

TEST(harley, hdCrankingWithCamAnother) {
	runTest("tests/ignition_injection/resources/hd-req-sync-another-one_1.csv");
}

TEST(harley, hdCrankingWithCamAnother2) {
	runTest("tests/ignition_injection/resources/hd-req-sync-another-one_2.csv");
}

TEST(harley, hdCrankingWithCamAnother3) {
	runTest("tests/ignition_injection/resources/hd-req-sync-another-one_3.csv");
}
