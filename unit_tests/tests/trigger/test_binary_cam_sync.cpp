#include "pch.h"
#include "trigger_central.h"
#include "engine_test_helper.h"
#include "logicdata_csv_reader.h"

static void runPolledBinaryLog(const char* filename) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	engineConfiguration->camInputs[0] = Gpio::A0;
	engineConfiguration->vvtMode[0] = VVT_POLLED_BINARY;
	engineConfiguration->engineSyncCam = static_cast<engineSyncCam_e>(0); // intake first bank
	engineConfiguration->binarySyncRemainderOffset = 0;

	eth.setTriggerType(trigger_type_e::TT_3_TOOTH_CRANK);

	CsvReader reader(/*triggerCount*/ 1, /*vvtCount*/ 1);
	reader.open(filename, NORMAL_ORDER, NORMAL_ORDER);

	bool gotRpm = false;
	bool firstCamSync = false;
	int n = 0;
	while (reader.haveMore()) {
		reader.processLine(&eth);

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if (!gotRpm && rpm) {
			printf("Got first RPM %f at %d for %s\n", rpm, n, filename);
			gotRpm = true;
		}
		if (!firstCamSync && engine->triggerCentral.triggerState.hasSynchronizedPhase()) {
			float syncTime = getTimeNowUs() / 1'000'000.0f;
			printf("Got first cam sync at %f s, at %d for %s\n", syncTime, n, filename);
			firstCamSync = true;
		}
		n++;
	}

	printf("%s: shaftSynchronized=%d hasSynchronizedPhase=%d resyncs=%d\n",
		filename,
		engine->triggerCentral.triggerState.getShaftSynchronized(),
		engine->triggerCentral.triggerState.hasSynchronizedPhase(),
		engine->triggerCentral.triggerState.phaseResyncCounter);
}

TEST(trigger, polledBinaryCamSyncCsv3000gt_cranking_rusefi) {
	runPolledBinaryLog("tests/trigger/resources/3000gt_cranking_rusefi.csv");
}

TEST(trigger, polledBinaryCamSyncCsv3000gt_cranking_rusefi_2) {
	runPolledBinaryLog("tests/trigger/resources/3000gt_cranking_rusefi_2.csv");
}

TEST(trigger, polledBinaryCamSyncCsv3000gt_crank_cam_cranking) {
	runPolledBinaryLog("tests/trigger/resources/3000gt_crank_cam_cranking.csv");
}

TEST(trigger, polledBinaryCamSyncCsv3000gt_crank_cam_cranking_2) {
	runPolledBinaryLog("tests/trigger/resources/3000gt_crank_cam_cranking_2.csv");
}

TEST(trigger, polledBinaryCamSyncCsv3000gt_crank_cam_cranking_idle) {
	runPolledBinaryLog("tests/trigger/resources/3000gt_crank_cam_cranking_idle.csv");
}
