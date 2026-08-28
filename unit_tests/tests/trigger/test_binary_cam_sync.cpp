#include "pch.h"
#include "trigger_central.h"
#include "engine_test_helper.h"
#include "logicdata_csv_reader.h"

extern bool printTriggerDebug;

static void runPolledBinaryLog(const char* filename) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	engineConfiguration->camInputs[0] = Gpio::A0;
	engineConfiguration->vvtMode[0] = VVT_POLLED_BINARY;
	engineConfiguration->engineSyncCam = static_cast<engineSyncCam_e>(0); // intake first bank
	engineConfiguration->binarySyncRemainderOffset = 3; // 360° offset for 6-divider (3-tooth crank)

	engineConfiguration->useNoiselessTriggerDecoder = false;
	engineConfiguration->triggerMinPulseWidthPercent = 5;
	engineConfiguration->triggerMinToothOnTimeUs = 2000; // 2ms minimum tooth high time below cranking rpm

	eth.setTriggerType(trigger_type_e::TT_3_TOOTH_CRANK);
	printTriggerDebug = true;

	CsvReader reader(/*triggerCount*/ 1, /*vvtCount*/ 1);
	reader.open(filename, NORMAL_ORDER, NORMAL_ORDER);

	bool gotRpm = false;
	bool firstCamSync = false;
	float firstRpm = 0;
	float firstRpmAt = 0;
	float firstCamSyncTime = 0;
	bool prevShaftSync = false;
	int syncLossCount = 0;
	int prevPhaseResync = 0;
	while (reader.haveMore()) {
		reader.processLine(&eth);

		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		if (!gotRpm && rpm) {
			firstRpm = rpm;
			firstRpmAt = getTimeNowUs() / 1'000'000.0f;
			gotRpm = true;
		}
		if (!firstCamSync && engine->triggerCentral.triggerState.hasSynchronizedPhase()) {
			firstCamSyncTime = getTimeNowUs() / 1'000'000.0f;
			firstCamSync = true;
		}
		bool curShaftSync = engine->triggerCentral.triggerState.getShaftSynchronized();
		if (prevShaftSync && !curShaftSync) {
			syncLossCount++;
			printf("CRANK SYNC LOSS #%d at %.6f s, rpm=%.1f, counter=%d\n",
				syncLossCount,
				getTimeNowUs() / 1'000'000.0f,
				rpm,
				engine->triggerCentral.triggerState.getSynchronizationCounter());
		}
		prevShaftSync = curShaftSync;
		int curPhaseResync = engine->triggerCentral.triggerState.phaseResyncCounter;
		if (curPhaseResync > prevPhaseResync) {
			printf("PHASE RESYNC #%d at %.6f s, rpm=%.1f, counter=%d\n",
				curPhaseResync,
				getTimeNowUs() / 1'000'000.0f,
				rpm,
				engine->triggerCentral.triggerState.getSynchronizationCounter());
		}
		prevPhaseResync = curPhaseResync;
	}

	printf("POLLED BINARY, %f at %f, %f s, %d, %d, %d, %s\n",
		firstRpm, firstRpmAt, firstCamSyncTime,
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
