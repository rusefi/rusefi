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

static void runTriggerTest(const char *fileName, int totalErrors, int syncCounter, float firstRpm) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	prepare(&eth, trigger_type_e::TT_VVT_MITSU_6G72);

	bool gotRpm = false;

	CsvReader reader(/*trigger channels count*/1, /* vvtCount */ 0);
	reader.flipOnRead = true;

	reader.open(fileName);
	while (reader.haveMore()) {
		reader.processLine(&eth);
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


TEST(real6g72, data) {
	runTriggerTest("tests/trigger/resources/3000gt.csv", 0, 16, 419.42f);
}

TEST(real6g72, data2) {
	runTriggerTest("tests/trigger/resources/3000gt_cranking_cam_first_crank_second_only_cam.csv", 4, 0, 376.20f);
}
