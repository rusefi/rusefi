#include "pch.h"
#include "logicdata_csv_reader.h"

TEST(Toyota3ToothCam, RealEngineRunning) {
	CsvReader reader(1, /* vvtCount */ 1);

	reader.open("tests/trigger/resources/toyota_3_tooth_cam.csv");
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->isFasterEngineSpinUpEnabled = true;
	engineConfiguration->alwaysInstantRpm = true;

	engineConfiguration->vvtMode[0] = VVT_TOYOTA_3_TOOTH;
	engineConfiguration->vvtOffsets[0] = 152.58;

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

	EXPECT_EQ(getTriggerCentral()->triggerState.camResyncCounter, 0);

	EXPECT_NEAR(engine->triggerCentral.getVVTPosition(/*bankIndex*/0, /*camIndex*/0), 0, 1);
	ASSERT_EQ(3078, round(Sensor::getOrZero(SensorType::Rpm)));

	// TODO: why warnings?
	ASSERT_EQ(1, eth.recentWarnings()->getCount());
	ASSERT_EQ(ObdCode::CUSTOM_PRIMARY_TOO_MANY_TEETH, eth.recentWarnings()->get(0).Code);
}

static void test3tooth(size_t revsBeforeVvt, size_t teethBeforeVvt, bool expectSync, int expectCamResyncCounter) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->vvtMode[0] = VVT_TOYOTA_3_TOOTH;
	engineConfiguration->skippedWheelOnCam = false;
	engineConfiguration->trigger.customTotalToothCount = 12;
	engineConfiguration->trigger.customSkippedToothCount = 1;
	eth.setTriggerType(trigger_type_e::TT_TOOTHED_WHEEL);

	engineConfiguration->vvtOffsets[0] = 35;

	ASSERT_EQ(0, round(Sensor::getOrZero(SensorType::Rpm)));

	// Get the crank sync'd
	for (int i = 0; i < 4; i++) {
		eth.fireFall(1);
		eth.fireRise(9);
	}

	ASSERT_EQ(0, round(Sensor::getOrZero(SensorType::Rpm)));

	// skipped tooth - should sync here
	eth.fireFall(1);
	eth.fireRise(19);

	ASSERT_EQ(500, round(Sensor::getOrZero(SensorType::Rpm)));

	// We should be at TDC
	ASSERT_EQ(0, getTriggerCentral()->currentEngineDecodedPhase);

	// Do some number of revolutions before firing the cam tooth
	for (size_t i = 0; i < revsBeforeVvt; i++) {
		for (size_t j = 0; i < 10; i++)
		{
			eth.fireFall(1);
			eth.fireRise(9);
		}

		eth.fireFall(1);
		eth.fireRise(19);
	}

	// Bump ahead by some more teeth
	for (size_t i = 0; i < teethBeforeVvt; i++) {
		eth.fireFall(1);
		eth.fireRise(9);
	}
	auto expectedPhase = 360 * revsBeforeVvt + 30 * teethBeforeVvt;
	ASSERT_EQ(expectedPhase, getTriggerCentral()->currentEngineDecodedPhase);

	// No full sync yet, no VVT has arrived
	EXPECT_EQ(false, getTriggerCentral()->triggerState.hasSynchronizedPhase());

	// Now fire a cam tooth - this shouldn't change engine phase,
	// should set VVT position
	hwHandleVvtCamSignal(true, getTimeNowNt(), 0);

	EXPECT_EQ(expectCamResyncCounter, getTriggerCentral()->triggerState.camResyncCounter);
	EXPECT_EQ(expectSync, getTriggerCentral()->triggerState.hasSynchronizedPhase());

	if (expectSync) {
		EXPECT_EQ(5, getTriggerCentral()->getVVTPosition(0, 0));

		// Bump ahead by one tooth
		eth.fireFall(1);
		eth.fireRise(9);
		ASSERT_EQ(60, getTriggerCentral()->currentEngineDecodedPhase);
	} else {
		EXPECT_EQ(0, getTriggerCentral()->getVVTPosition(0, 0));
	}
}

// The cam has 3 teeth, which could arrive at one of 6 possible crank phases.
// 0 revs or 1 revs
// +0, +120, or +240 deg
// Test all 6 cases!

TEST(Toyota3ToothCam, TestFirstToothInPhase) {
	test3tooth(0, 1, true, 0);
}

TEST(Toyota3ToothCam, TestFirstToothOutOfPhase) {
	test3tooth(1, 1, true, 1);
}

TEST(Toyota3ToothCam, TestSecondToothInPhase) {
	test3tooth(0, 5, false, 0);
}

TEST(Toyota3ToothCam, TestSecondToothOutOfPhase) {
	test3tooth(1, 5, false, 0);
}

TEST(Toyota3ToothCam, TestThirdToothInPhase) {
	test3tooth(0, 9, false, 0);
}

TEST(Toyota3ToothCam, TestThirdToothOutOfPhase) {
	test3tooth(1, 9, false, 0);
}
