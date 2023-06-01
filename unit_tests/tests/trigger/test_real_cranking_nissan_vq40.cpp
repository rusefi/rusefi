/*
 * @file test_real_cranking_nissan_vq40.cpp
 *
 * @date Jul 21, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"
#include "logicdata_csv_reader.h"

static void test(int engineSyncCam, float camOffsetAdd) {
	CsvReader reader(1, /* vvtCount */ 2);

	reader.open("tests/trigger/resources/nissan_vq40_cranking-1.csv");
	EngineTestHelper eth(engine_type_e::HELLEN_121_NISSAN_6_CYL);
	engineConfiguration->isFasterEngineSpinUpEnabled = false;
	engineConfiguration->alwaysInstantRpm = true;

	// Different sync cam may result in different TDC point, so we might need different cam offsets.
	engineConfiguration->vvtOffsets[0] = engineConfiguration->vvtOffsets[0] + camOffsetAdd;
	engineConfiguration->vvtOffsets[2] = engineConfiguration->vvtOffsets[2] + camOffsetAdd;
	engineConfiguration->engineSyncCam = engineSyncCam;

	bool hasSeenFirstVvt = false;

	while (reader.haveMore()) {
		reader.processLine(&eth);
		float vvt1 = engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0);
		float vvt2 = engine->triggerCentral.getVVTPosition(/*bankIndex*/1, /*camIndex*/0);

		if (vvt1 != 0) {
			if (!hasSeenFirstVvt) {
				EXPECT_NEAR(vvt1, 1.4, /*precision*/1);
				hasSeenFirstVvt = true;
			}

			// cam position should never be reported outside of correct range
			EXPECT_TRUE(vvt1 > -3 && vvt1 < 3);
		}

		if (vvt2 != 0) {
			// cam position should never be reported outside of correct range
			EXPECT_TRUE(vvt2 > -3 && vvt2 < 3);
		}
	}

	EXPECT_NEAR(engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0), 1.351, 1e-2);
	EXPECT_NEAR(engine->triggerCentral.getVVTPosition(/*bankIndex*/1, /*camIndex*/0), 1.548, 1e-2);
	ASSERT_EQ(102, round(Sensor::getOrZero(SensorType::Rpm)))<< reader.lineIndex();

	// TODO: why warnings?
	ASSERT_EQ(2, eth.recentWarnings()->getCount());
	ASSERT_EQ(ObdCode::CUSTOM_OUT_OF_ORDER_COIL, eth.recentWarnings()->get(0).Code);	// this is from a coil being protected by overdwell protection
	ASSERT_EQ(ObdCode::CUSTOM_PRIMARY_TOO_MANY_TEETH, eth.recentWarnings()->get(1).Code);
}

// On Nissan VQ, all cams have the same pattern, so all should be equally good for engine sync. Check them all!

TEST(realCrankingVQ40, normalCrankingSyncCam1) {
	test(0, 0);
}

TEST(realCrankingVQ40, normalCrankingSyncCam2) {
	test(2, -360);
}
