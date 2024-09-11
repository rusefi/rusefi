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

static void runTriggerTest(const char *fileName, int totalErrors, int syncCounter, float firstRpm) {
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
	ASSERT_EQ(syncCounter,  engine->triggerCentral.triggerState.getCrankSynchronizationCounter());
}


TEST(real6g72, data1) {
	runTriggerTest("tests/trigger/resources/3000gt.csv", 0, 15, 195.515f);
}

TEST(real6g72, data2) {
	runTriggerTest("tests/trigger/resources/3000gt_cranking_cam_first_crank_second_only_cam.csv", 2, 9, 157.843f);
}
