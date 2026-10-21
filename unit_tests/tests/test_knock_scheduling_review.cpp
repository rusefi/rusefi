#include "pch.h"

#if EFI_SOFTWARE_KNOCK

#include "spark_logic.h"

namespace {

struct KnockRequest {
	uint8_t cylinder;
	uint8_t bank;
	float durationSeconds;
	efitick_t time;
};

std::vector<KnockRequest> requests;

void configureKnockWindows(float startAngle) {
	requests.clear();
	engineConfiguration->cylindersCount = 12;
	engineConfiguration->firingOrder = FO_1_2_3_4_5_6_7_8_9_10_11_12;
	engineConfiguration->enableSoftwareKnock = true;
	engineConfiguration->knockDetectionWindowStart = startAngle;
	engineConfiguration->knockSamplingDuration = 50;
	engineConfiguration->knockBankCyl1 = 0;
	engineConfiguration->knockBankCyl2 = 1;
	engine->updateTriggerConfiguration();
	engine->rpmCalculator.setRpmValue(1000);
}

} // namespace

// This replaces only the hardware driver. The caller, angle conversion,
// scheduler queue and delayed callback are the production implementations.
void initSoftwareKnock() {
}

void onStartKnockSampling(uint8_t cylinderNumber, float durationSeconds, uint8_t channel) {
	requests.push_back({cylinderNumber, channel, durationSeconds, getTimeNowNt()});
}

TEST(KnockSchedulingReview, WindowBeforeNextSparkRetainsIdentity) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureKnockWindows(20);
	engine->onSparkFireKnockSense(0, getTimeNowNt());
	eth.moveTimeForwardUs(3334);
	eth.executeActions();
	ASSERT_EQ(1u, requests.size());
	EXPECT_EQ(0, requests[0].cylinder);
	EXPECT_EQ(0, requests[0].bank);
	EXPECT_NEAR(50.0f / 6000, requests[0].durationSeconds, 0.000001f);
	eth.moveTimeForwardUs(6666); // Next V12 spark is 60 degrees / 10 ms later.
	engine->onSparkFireKnockSense(1, getTimeNowNt());
	eth.moveTimeForwardUs(3334);
	eth.executeActions();
	ASSERT_EQ(2u, requests.size());
	EXPECT_EQ(1, requests[1].cylinder);
	EXPECT_EQ(1, requests[1].bank);
}

TEST(KnockSchedulingReview, DelayedWindowUsesNextSparkCylinderAndBank) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureKnockWindows(70);
	engine->onSparkFireKnockSense(0, getTimeNowNt());
	eth.moveTimeForwardUs(10000);
	eth.executeActions();
	ASSERT_TRUE(requests.empty());
	engine->onSparkFireKnockSense(1, getTimeNowNt());
	ASSERT_EQ(2, engine->scheduler.size());
	eth.moveTimeForwardUs(1667);
	eth.executeActions();
	ASSERT_EQ(1u, requests.size());
	// ECU-032: this window belongs to cylinder 0/bank 0.
	EXPECT_EQ(1, requests[0].cylinder);
	EXPECT_EQ(1, requests[0].bank);
	eth.moveTimeForwardUs(10000);
	eth.executeActions();
	ASSERT_EQ(2u, requests.size());
	EXPECT_EQ(1, requests[1].cylinder);
	EXPECT_EQ(0, engine->scheduler.size());
}

TEST(KnockSchedulingReview, OverlappingWindowsRequestBothSamples) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureKnockWindows(20);
	engineConfiguration->knockSamplingDuration = 80;
	engine->onSparkFireKnockSense(0, getTimeNowNt());
	eth.moveTimeForwardUs(3334);
	eth.executeActions();
	eth.moveTimeForwardUs(6666);
	engine->onSparkFireKnockSense(1, getTimeNowNt());
	eth.moveTimeForwardUs(3334);
	eth.executeActions();
	ASSERT_EQ(2u, requests.size());
	EXPECT_EQ(0, requests[0].cylinder);
	EXPECT_EQ(1, requests[1].cylinder);
	const float secondsBetweenRequests = NT2US(requests[1].time - requests[0].time) / 1000000.0f;
	EXPECT_LT(secondsBetweenRequests, requests[0].durationSeconds);
	// No ADC busy state is modeled: this proves overlapping requests, not
	// simultaneous acquisition or the time needed by the hardware DSP thread.
}

TEST(KnockSchedulingReview, StoppedEngineDiscardsDueWindow) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureKnockWindows(70);
	engine->onSparkFireKnockSense(0, getTimeNowNt());
	engine->rpmCalculator.setStopSpinning();
	eth.moveTimeForwardUs(12000);
	eth.executeActions();
	EXPECT_TRUE(requests.empty());
	EXPECT_EQ(0, engine->scheduler.size());
}

TEST(KnockSchedulingReview, RealSparkSamplesFiringOrderPositionInsteadOfCylinder) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureKnockWindows(20);
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->knockBankCyl2 = 0;
	engineConfiguration->knockBankCyl3 = 1;
	engine->updateTriggerConfiguration();
	engine->ignitionState.sparkDwell = 3;
	engine->ignitionState.dwellDurationAngle = 18;
	setArrayValues(engine->engineState.timingAdvance, 0.0f);
	initializeIgnitionActions();
	const auto startUs = getTimeNowUs();
	// Firing-order position 1 serves physical cylinder 3. Its dwell/spark
	// at 162/180 degrees fall within this trigger interval.
	onTriggerEventSparkLogic(1000, getTimeNowNt(), 150, 200);
	eth.setTimeAndInvokeEventsUs(startUs + 9000);
	ASSERT_EQ(1u, requests.size());
	EXPECT_EQ(1, requests[0].cylinder); // Should be physical index 2.
	EXPECT_EQ(0, requests[0].bank); // Cylinder 3 is configured on bank 1.
	EXPECT_FALSE(enginePins.coils[0].getLogicValue());
	EXPECT_EQ(0, engine->scheduler.size());
}

TEST(KnockSchedulingReview, RestrikesScheduleOverlappingWindowsForOneCombustion) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureKnockWindows(20);
	engineConfiguration->cylindersCount = 1;
	engineConfiguration->firingOrder = FO_1;
	engineConfiguration->ignitionMode = IM_ONE_COIL;
	engineConfiguration->multisparkEnable = true;
	engineConfiguration->multisparkMaxRpm = 3000;
	engineConfiguration->multisparkMaxSparkingAngle = 60;
	engineConfiguration->multisparkMaxExtraSparkCount = 2;
	engineConfiguration->multisparkSparkDuration = 0.5f;
	engineConfiguration->multisparkDwell = 1;
	engine->updateTriggerConfiguration();
	engine->ignitionState.sparkDwell = 3;
	engine->ignitionState.dwellDurationAngle = 18;
	setArrayValues(engine->engineState.timingAdvance, -30.0f);
	engine->engineState.multispark.count = 2;
	engine->engineState.multispark.delay = US2NT(500);
	engine->engineState.multispark.dwell = MS2NT(1);
	initializeIgnitionActions();
	const auto startUs = getTimeNowUs();
	onTriggerEventSparkLogic(1000, getTimeNowNt(), 0, 90);
	eth.setTimeAndInvokeEventsUs(startUs + 12000);
	ASSERT_EQ(3u, requests.size());
	for (size_t i = 0; i < requests.size(); i++) {
		EXPECT_EQ(0, requests[i].cylinder);
		EXPECT_EQ(0, requests[i].bank);
		if (i > 0) {
			const auto intervalUs = NT2US(requests[i].time - requests[i - 1].time);
			EXPECT_EQ(1500, intervalUs);
			EXPECT_LT(intervalUs / 1000000.0f, requests[i - 1].durationSeconds);
		}
	}
	EXPECT_FALSE(enginePins.coils[0].getLogicValue());
	EXPECT_EQ(0, engine->scheduler.size());
}

#endif // EFI_SOFTWARE_KNOCK
