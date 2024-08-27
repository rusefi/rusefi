#include "pch.h"

#include "logicdata_csv_reader.h"

TEST(real6g72, data) {

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;
	engineConfiguration->invertPrimaryTriggerSignal = false;

	engineConfiguration->isPhaseSyncRequiredForIgnition = true;

    eth.setTriggerType(trigger_type_e::TT_VVT_MITSU_6G72);

    ASSERT_FALSE(engine->triggerCentral.triggerShape.shapeDefinitionError);

	engineConfiguration->vvtMode[0] = VVT_INACTIVE;

	bool gotRpm = false;

	CsvReader reader(/*trigger channels count*/1, /* vvtCount */ 0);
	reader.flipOnRead = true;

	reader.open("tests/trigger/resources/3000gt.csv");
	while (reader.haveMore()) {
		reader.processLine(&eth);
		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		printf("rpm %f\n", rpm);

		if (!gotRpm && rpm) {
			gotRpm = true;

//			EXPECT_EQ(eventCount, 7);
			EXPECT_NEAR(rpm, 419.42f, 0.1);
		}
	}
	ASSERT_TRUE(gotRpm);
	ASSERT_EQ(0,  engine->triggerCentral.triggerState.totalTriggerErrorCounter);
	ASSERT_EQ(16,  engine->triggerCentral.triggerState.getCrankSynchronizationCounter());
}
