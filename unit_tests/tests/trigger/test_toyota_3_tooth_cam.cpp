#include "pch.h"
#include "logicdata_csv_reader.h"

TEST(realToyota3ToothCam, running) {
	CsvReader reader(1, /* vvtCount */ 1);

	reader.open("tests/trigger/resources/toyota_3_tooth_cam.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	engineConfiguration->vvtMode[0] = VVT_TOYOTA_3_TOOTH;
	engineConfiguration->vvtOffsets[0] = 152.583f;

	engineConfiguration->trigger.customTotalToothCount = 36;
	engineConfiguration->trigger.customSkippedToothCount = 2;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);

	bool hasSeenFirstVvt = false;

	while (reader.haveMore()) {
		reader.processLine(&eth);
		float vvt1 = engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0);

		if (vvt1 != 0) {
			if (!hasSeenFirstVvt) {
				EXPECT_NEAR(vvt1, 0, /*precision*/1);
				hasSeenFirstVvt = true;
			}

			// cam position should never be reported outside of correct range
			EXPECT_TRUE(vvt1 > -3 && vvt1 < 3);
		}
	}

	EXPECT_NEAR(engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0), 0, 1);
	ASSERT_EQ(3078, round(Sensor::getOrZero(SensorType::Rpm)));

	// TODO: why warnings?
	ASSERT_EQ(1, eth.recentWarnings()->getCount());
	ASSERT_EQ(ObdCode::CUSTOM_PRIMARY_TOO_MANY_TEETH, eth.recentWarnings()->get(0).Code);
}
