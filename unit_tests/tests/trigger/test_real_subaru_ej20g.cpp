#include "pch.h"
#include "real_trigger_helper.h"

TEST(real, SubaruEj20gcranking_only_cam7) {
	RealTriggerHelper helper;
	/* 0 - cam
	 * 1 - crank */
	engineConfiguration->invertPrimaryTriggerSignal = true;

	helper.expectedFirstRpm = 240;
	helper.expectedFirstRpmLineIndex = 38;
	helper.runTest("tests/trigger/resources/subaru_6_7.csv", trigger_type_e::TT_VVT_SUBARU_7_WITHOUT_6, 1, 0, false, 0.0);
	// this currently fails
	//ASSERT_EQ(0u, eth.recentWarnings()->getCount());
}

TEST(real, SubaruEj20gDefaultCranking) {
	RealTriggerHelper helper;

	/* 0 - cam
	 * 1 - crank */
	engineConfiguration->invertPrimaryTriggerSignal = true;
	engineConfiguration->invertSecondaryTriggerSignal = true;

	helper.expectedFirstRpm = 193;
	helper.expectedFirstRpmLineIndex = 38;
	helper.runTest("tests/trigger/resources/subaru_6_7.csv", trigger_type_e::TT_SUBARU_7_6, 2, 0, false, 0.0);
}

TEST(real, SubaruEj20gCrankingWot) {
	RealTriggerHelper helper;
	/* 0 - cam
	 * 1 - crank */
	engineConfiguration->invertPrimaryTriggerSignal = true;
	engineConfiguration->invertSecondaryTriggerSignal = true;

	helper.expectedFirstRpm = 188;
	helper.expectedFirstRpmLineIndex = 32;
	helper.runTest("tests/trigger/resources/subaru_6_7_wot.csv", trigger_type_e::TT_SUBARU_7_6, 2, 0, false, 0.0);
}

TEST(real, SubaruEj20gDefaultCranking_only_crank) {
	RealTriggerHelper helper;
	engineConfiguration->invertPrimaryTriggerSignal = true;

	helper.expectedFirstRpm = 182;
	helper.expectedFirstRpmLineIndex = 13;
	helper.runTest("tests/trigger/resources/subaru_6_7_crank_first.csv", trigger_type_e::TT_SUBARU_7_6_CRANK, 1, 0, false, 0.0);
}

TEST(real, SubaruEj20gDefaultCrankingSeparateTrigger) {
	RealTriggerHelper helper;
	/* 1 - cam
	 * 0 - crank */
	// test CSV file was captured without inversion of cam and crank signals.
	// triggers are defined with SyncEdge::RiseOnly, while with real VR sensor we should rely on falling edges only.
	// FallOnly is not supported (yet?). So trigger is defined as SyncEdge::RiseOnly and with 1 degree tooth width.
	// setting both crank and cam inversion allows us to feed trigger decoder with correct (falling) edges.
	engineConfiguration->vvtMode[0] = VVT_SUBARU_7TOOTH;
	engineConfiguration->invertPrimaryTriggerSignal = true;
	engineConfiguration->invertCamVVTSignal = true;

	bool gotFullSync = false;

	helper.expectedFirstRpm = 182;
	helper.expectedFirstRpmLineIndex = 13;
	helper.runTest("tests/trigger/resources/subaru_6_7_crank_first.csv", trigger_type_e::TT_SUBARU_7_6_CRANK, 1, 1, false, 0.0, [&](CsvReader& reader) {
		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		bool hasFullSync = getTriggerCentral()->triggerState.hasSynchronizedPhase();

		if (!gotFullSync && hasFullSync && rpm) {
			gotFullSync = true;

			// Should get full sync and non-zero RPM
			EXPECT_EQ(reader.lineIndex(), 38);
			EXPECT_NEAR(rpm, 220.0f, 1.0f);
		}
	});
}

// Following test loses hasFullSync twice
// Test also has RPM falling to zero twice
// While real car was runnig fine while capturing this CSV
// TODO: investigate
// TODO: this test also triggers a watchdog warning after clearing the EngineTestHelper!
TEST(real, SubaruEj20gDefaultRev) {
	extern bool unitTestTaskNoFastCallWhileAdvancingTimeHack;
	unitTestTaskNoFastCallWhileAdvancingTimeHack = true;

	RealTriggerHelper helper;
	/* 1 - cam
	 * 0 - crank */
	engineConfiguration->vvtMode[0] = VVT_SUBARU_7TOOTH;
	engineConfiguration->invertPrimaryTriggerSignal = false;
	engineConfiguration->invertCamVVTSignal = false;

	bool gotFullSync = false;
	helper.runTest("tests/trigger/resources/subaru_6_7_rev.csv", trigger_type_e::TT_SUBARU_7_6_CRANK, 1, 1, false, 0.0, [&](CsvReader& reader) {
		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		bool hasFullSync = getTriggerCentral()->triggerState.hasSynchronizedPhase();

		if (!gotFullSync && hasFullSync && rpm) {
			gotFullSync = true;

			// Should get full sync and non-zero RPM
			EXPECT_EQ(reader.lineIndex(), 37);
			EXPECT_NEAR(rpm, 2174.0f, 1.0f);
		}
	});
}
