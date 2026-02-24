#include "pch.h"

#include "logicdata_csv_reader.h"

static void prepare(EngineTestHelper *eth, trigger_type_e trigger) {

	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;
	engineConfiguration->invertPrimaryTriggerSignal = false;

	engineConfiguration->isPhaseSyncRequiredForIgnition = true;

    eth->setTriggerType(trigger);

    ASSERT_FALSE(engine->triggerCentral.triggerShape.shapeDefinitionError);

	engineConfiguration->vvtMode[0] = VVT_INACTIVE;
}

static void constructTriggerFromRecording(CsvReader *reader) {
	int magic = 20;

	if (reader->lineIndex() == magic) {

		int len = 8;
		double last = reader->history.get(magic - 1);
		printf("last=%f\n", last);

		double time360 = last - reader->history.get(magic - 1 - 8);

		float current = 0;

		for (int i=len - 1;i>=0;i--) {
			double tooth = last - reader->history.get(magic - 1 - i);
//				printf("index=%d width=%f\n", i, tooth);
			double angle = 360 - (360 * tooth / time360);
			//printf("index=%d, to=%f\n", i, angle);

			bool isRise = (i % 2) == 1;
			const char * front = isRise ? "RISE" : "FALL";

			printf("\ts->addEvent360(%f, TriggerValue::%s);\n", angle, front);
			current += tooth;
		}

//		printf("time360=%f\n", time360);
	}
}

static void runTriggerTest(const char *fileName, uint32_t totalErrors, int syncCounter, float firstRpm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	prepare(&eth, trigger_type_e::TT_VVT_MITSU_6G72);

	bool gotRpm = false;

	CsvReader reader(/*trigger channels count*/1, /* vvtCount */ 0);
	reader.flipOnRead = true;

	reader.open(fileName);
	while (reader.haveMore()) {
		reader.processLine(&eth);

		constructTriggerFromRecording(&reader);


		auto rpm = Sensor::getOrZero(SensorType::Rpm);
//		printf("rpm %f\n", rpm);

		if (!gotRpm && rpm) {
			gotRpm = true;

//			EXPECT_EQ(eventCount, 7);
			EXPECT_NEAR(rpm, firstRpm, 0.1);
		}
	}
	ASSERT_TRUE(gotRpm);
	ASSERT_EQ(totalErrors,  engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	ASSERT_EQ(syncCounter,  engine->triggerCentral.triggerState.getSynchronizationCounter());
}


/*
TEST(real6g72, data1) {
	runTriggerTest("tests/trigger/resources/3000gt.csv", 0, 15, 195.515f);
}

TEST(real6g72, data2) {
	runTriggerTest("tests/trigger/resources/3000gt_cranking_cam_first_crank_second_only_cam.csv", 2, 9, 157.843f);
}
*/

void generateLog(const char* filename) {
    CsvReader reader(/*triggerCount*/ 1, /* vvtCount */ 1);

    reader.open(filename, NORMAL_ORDER, NORMAL_ORDER);

    EngineTestHelper eth(engine_type_e::TEST_ENGINE);
//    setVerboseTrigger(true);

    engineConfiguration->vvtMode[0] = vvt_mode_e::VVT_MITSUBISHI_6G72;
    eth.setTriggerType(trigger_type_e::TT_3_TOOTH_CRANK);

    engineConfiguration->globalTriggerAngleOffset = 125;
    engineConfiguration->isFasterEngineSpinUpEnabled = true;
    engineConfiguration->alwaysInstantRpm = true;
    engineConfiguration->isPhaseSyncRequiredForIgnition = true;

    int n = 0;
    bool firstRpm = false;
    while (reader.haveMore()) {
        reader.processLine(&eth);
        auto rpm = Sensor::getOrZero(SensorType::Rpm);
        if ((rpm) && (!firstRpm)) {
            printf("Got first RPM %f, at %d\n", rpm, n);
            firstRpm = true;
        }
        n++;
    }
}

TEST(real6g72, sync_3000gt_cranking_rusefi) {
    generateLog("tests/trigger/resources/3000gt_cranking_rusefi.csv");
}

TEST(real6g72, sync_3000gt_cranking_rusefi_2) {
    generateLog("tests/trigger/resources/3000gt_cranking_rusefi_2.csv");
}

TEST(real6g72, sync_3000gt_crank_cam_cranking) {
    generateLog("tests/trigger/resources/3000gt_crank_cam_cranking.csv");
}

TEST(real6g72, sync_3000gt_crank_cam_cranking_2) {
    generateLog("tests/trigger/resources/3000gt_crank_cam_cranking_2.csv");
}

TEST(real6g72, sync_3000gt_crank_cam_cranking_idle) {
    generateLog("tests/trigger/resources/3000gt_crank_cam_cranking_idle.csv");
}
