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

	engineConfiguration->useNoiselessTriggerDecoder = false;
	engineConfiguration->triggerMinPulseWidthPercent = 5;

	eth.setTriggerType(trigger_type_e::TT_3_TOOTH_CRANK);

	CsvReader reader(/*triggerCount*/ 1, /*vvtCount*/ 1);
	reader.open(filename, NORMAL_ORDER, NORMAL_ORDER);

	bool gotRpm = false;
	bool firstCamSync = false;
	float firstRpm = 0;
	int firstRpmAt = 0;
	float firstCamSyncTime = 0;
	int firstCamSyncAt = 0;
	int n = 0;
	while (reader.haveMore()) {
		reader.processLine(&eth);

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if (!gotRpm && rpm) {
			firstRpm = rpm;
			firstRpmAt = n;
			gotRpm = true;
		}
		if (!firstCamSync && engine->triggerCentral.triggerState.hasSynchronizedPhase()) {
			firstCamSyncTime = getTimeNowUs() / 1'000'000.0f;
			firstCamSyncAt = n;
			firstCamSync = true;
		}
		n++;
	}

	printf("POLLED BINARY, %f at %d, %f s at %d, %d, %d, %d, %s\n",
		firstRpm, firstRpmAt, firstCamSyncTime, firstCamSyncAt,
		engine->triggerCentral.triggerState.getShaftSynchronized(),
		engine->triggerCentral.triggerState.hasSynchronizedPhase(),
		engine->triggerCentral.triggerState.phaseResyncCounter,
		filename);
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
