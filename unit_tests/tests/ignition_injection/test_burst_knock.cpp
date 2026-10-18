#include "pch.h"
#include "burst_knock.h"
#include "accel_enrichment.h"

static BurstKnock& getBurst() {
	return engine->module<BurstKnock>().unmock();
}

static TpsAccelEnrichment& getAe() {
	return engine->module<TpsAccelEnrichment>().unmock();
}

static void setupBurst(float retard = 10, float decay = 2) {
	engineConfiguration->burstKnockEnabled = true;
	engineConfiguration->burstKnockDecayTime = decay;
	setLinearCurve(engineConfiguration->burstKnockRpmBins, 0, 7000);
	setLinearCurve(engineConfiguration->burstKnockTpsRateBins, 0, 1400);
	setTable(engineConfiguration->burstKnockRetardTable, retard);
	engineConfiguration->tpsAccelLookback = 1;
	Sensor::setMockValue(SensorType::Rpm, 3000);
}

static void feedTransient(float delta, bool aboveThreshold) {
	getAe().deltaTps = delta;
	getAe().isAboveAccelThreshold = aboveThreshold;
	getBurst().onSlowCallback();
}

TEST(BurstKnock, DefaultsAreDisabledWithZeroTable) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_FALSE(engineConfiguration->burstKnockEnabled);
	EXPECT_FLOAT_EQ(engineConfiguration->burstKnockDecayTime, 1);
	for (size_t y = 0; y < BURST_KNOCK_TPS_RATE_SIZE; y++) {
		EXPECT_FLOAT_EQ(engineConfiguration->burstKnockTpsRateBins[y], y * 150);
		for (size_t x = 0; x < BURST_KNOCK_RPM_SIZE; x++) {
			EXPECT_FLOAT_EQ(engineConfiguration->burstKnockRpmBins[x], 800 + x * 800);
			EXPECT_FLOAT_EQ(engineConfiguration->burstKnockRetardTable[y][x], 0);
		}
	}
	feedTransient(300, true);
	EXPECT_FALSE(getBurst().isBurstKnockActive);
	EXPECT_FLOAT_EQ(getBurst().getTimingRetard(), 0);
}

TEST(BurstKnock, PullsTimingThenDecaysFromLastTransientSample) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupBurst();
	feedTransient(300, true);
	ASSERT_TRUE(getBurst().isBurstKnockActive);
	EXPECT_FLOAT_EQ(getBurst().getTimingRetard(), 10);

	eth.moveTimeForwardSec(0.5f);
	feedTransient(0, false);
	EXPECT_NEAR(getBurst().getTimingRetard(), 7.5f, 0.001f);
	eth.moveTimeForwardSec(0.5f);
	getBurst().onSlowCallback();
	EXPECT_NEAR(getBurst().getTimingRetard(), 5, 0.001f);
	eth.moveTimeForwardSec(1);
	getBurst().onSlowCallback();
	EXPECT_FALSE(getBurst().isBurstKnockActive);
	EXPECT_FLOAT_EQ(getBurst().burstKnockRetard, 0);
	EXPECT_FLOAT_EQ(getBurst().getTimingRetard(), 0);
}

TEST(BurstKnock, TransientRefreshesTableValueAndDecayTimer) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupBurst();
	feedTransient(300, true);
	eth.moveTimeForwardSec(5);
	feedTransient(300, true);
	EXPECT_FLOAT_EQ(getBurst().getTimingRetard(), 10);

	// Track the current table value, even when it drops during a sustained transient.
	setTable(engineConfiguration->burstKnockRetardTable, 6);
	feedTransient(300, true);
	EXPECT_FLOAT_EQ(getBurst().getTimingRetard(), 6);
	eth.moveTimeForwardSec(1);
	feedTransient(0, false);
	EXPECT_NEAR(getBurst().getTimingRetard(), 3, 0.001f);

	// A new transient during decay restores full table retard and restarts the timer.
	feedTransient(300, true);
	EXPECT_FLOAT_EQ(getBurst().getTimingRetard(), 6);
	eth.moveTimeForwardSec(0.5f);
	feedTransient(0, false);
	EXPECT_NEAR(getBurst().getTimingRetard(), 4.5f, 0.001f);
}

TEST(BurstKnock, DisablingClearsRetardAndTelemetry) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupBurst();
	feedTransient(300, true);
	ASSERT_TRUE(getBurst().isBurstKnockActive);
	engineConfiguration->burstKnockEnabled = false;
	feedTransient(300, true);
	EXPECT_FALSE(getBurst().isBurstKnockActive);
	EXPECT_FLOAT_EQ(getBurst().getTimingRetard(), 0);
	EXPECT_FLOAT_EQ(getBurst().burstKnockRetard, 0);
	EXPECT_FLOAT_EQ(getBurst().burstKnockTpsRate, 0);

	engineConfiguration->burstKnockEnabled = true;
	feedTransient(0, false);
	EXPECT_FLOAT_EQ(getBurst().getTimingRetard(), 0);
}

TEST(BurstKnock, ZeroOrNegativeCellsNeverAdvanceTiming) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupBurst();
	for (float retard : {0.0f, -5.0f}) {
		setTable(engineConfiguration->burstKnockRetardTable, retard);
		feedTransient(300, true);
		EXPECT_FALSE(getBurst().isBurstKnockActive);
		EXPECT_FLOAT_EQ(getBurst().burstKnockRetard, 0);
		EXPECT_FLOAT_EQ(getBurst().getTimingRetard(), 0);
	}
}

TEST(BurstKnock, ZeroDecayReleasesImmediatelyAfterTransient) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupBurst(10, 0);
	feedTransient(300, true);
	EXPECT_FLOAT_EQ(getBurst().getTimingRetard(), 10);
	feedTransient(0, false);
	EXPECT_FALSE(getBurst().isBurstKnockActive);
	EXPECT_FLOAT_EQ(getBurst().getTimingRetard(), 0);
}

TEST(BurstKnock, InterpolatesRpmAndTpsRateWithLookbackFloor) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupBurst();
	for (size_t y = 0; y < BURST_KNOCK_TPS_RATE_SIZE; y++) {
		for (size_t x = 0; x < BURST_KNOCK_RPM_SIZE; x++) {
			engineConfiguration->burstKnockRetardTable[y][x] = x + 2 * y;
		}
	}
	Sensor::setMockValue(SensorType::Rpm, 2500);
	engineConfiguration->tpsAccelLookback = 0.2f;
	feedTransient(60, true);
	EXPECT_NEAR(getBurst().burstKnockTpsRate, 300, 0.001f);
	EXPECT_NEAR(getBurst().getTimingRetard(), 5.5f, 0.001f);

	engineConfiguration->tpsAccelLookback = 0;
	feedTransient(0.3f, true);
	EXPECT_NEAR(getBurst().burstKnockTpsRate, 300, 0.001f);
	EXPECT_NEAR(getBurst().getTimingRetard(), 5.5f, 0.001f);
}

TEST(BurstKnock, UsesRealAccelEnrichmentThresholdAndHistory) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupBurst();
	engineConfiguration->tpsAccelLookback = 0.2f;
	engineConfiguration->tpsAccelEnrichmentThreshold = 5;
	getAe().onConfigurationChange(nullptr);
	for (int i = 0; i < 4; i++) {
		getAe().onNewValue(0);
	}
	getAe().onNewValue(5);
	getBurst().onSlowCallback();
	EXPECT_FALSE(getBurst().isBurstKnockActive); // Threshold is strictly greater than.
	getAe().onNewValue(25);
	getBurst().onSlowCallback();
	EXPECT_TRUE(getBurst().isBurstKnockActive);
	EXPECT_FLOAT_EQ(getAe().deltaTps, 20);
	EXPECT_NEAR(getBurst().burstKnockTpsRate, 100, 0.001f);
	EXPECT_FLOAT_EQ(getBurst().getTimingRetard(), 10);

	// Stable TPS eventually ages the jump out of AE's shared buffer.
	for (int i = 0; i < 4; i++) {
		getAe().onNewValue(25);
	}
	ASSERT_FALSE(getAe().isAboveAccelThreshold);
	eth.moveTimeForwardSec(1);
	getBurst().onSlowCallback();
	EXPECT_NEAR(getBurst().getTimingRetard(), 5, 0.001f);
}

TEST(BurstKnock, SubtractsRelativeRetardFromRunningAdvance) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupBurst();
	engine->rpmCalculator.setRpmValue(3000);
	engineConfiguration->timingMode = TM_DYNAMIC;
	setTable(config->ignitionTable, 25);
	float baseline = engine->ignitionState.getWrappedAdvance(3000, 50);
	feedTransient(300, true);
	EXPECT_NEAR(engine->ignitionState.getWrappedAdvance(3000, 50), baseline - 10, 0.001f);
	eth.moveTimeForwardSec(1);
	feedTransient(0, false);
	EXPECT_NEAR(engine->ignitionState.getWrappedAdvance(3000, 50), baseline - 5, 0.001f);
}

TEST(BurstKnock, PreservesFixedTimingAndCrankingCorrectionSwitch) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupBurst();
	feedTransient(300, true);
	engineConfiguration->timingMode = TM_FIXED;
	engineConfiguration->fixedTiming = 15;
	EXPECT_FLOAT_EQ(engine->ignitionState.getWrappedAdvance(3000, 50), 15);

	engineConfiguration->timingMode = TM_DYNAMIC;
	engine->rpmCalculator.setRpmValue(200);
	ASSERT_TRUE(engine->rpmCalculator.isCranking());
	engineConfiguration->useSeparateAdvanceForCranking = true;
	for (auto& advance : config->crankingAdvance) {
		advance = 20;
	}
	engineConfiguration->useAdvanceCorrectionsForCranking = false;
	float baseline = engine->ignitionState.getWrappedAdvance(200, 50);
	engineConfiguration->burstKnockEnabled = false;
	getBurst().onSlowCallback();
	EXPECT_FLOAT_EQ(engine->ignitionState.getWrappedAdvance(200, 50), baseline);

	engineConfiguration->useAdvanceCorrectionsForCranking = true;
	baseline = engine->ignitionState.getWrappedAdvance(200, 50);
	engineConfiguration->burstKnockEnabled = true;
	feedTransient(300, true);
	EXPECT_NEAR(engine->ignitionState.getWrappedAdvance(200, 50), baseline - 10, 0.001f);
}

TEST(BurstKnock, SlowCallbackConsumesCurrentTpsSampleBeforeComputingRetard) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupBurst();
	engineConfiguration->tpsAccelLookback = 0.2f;
	engineConfiguration->tpsAccelEnrichmentThreshold = 5;
	getAe().onConfigurationChange(nullptr);
	Sensor::setMockValue(SensorType::Tps1, 0);
	for (int i = 0; i < 4; i++) {
		eth.moveTimeForwardMs(50);
		engine->periodicSlowCallback();
	}
	ASSERT_FALSE(getBurst().isBurstKnockActive);

	// Use the engine dispatch path: feeding AE by hand would hide callback-order bugs.
	Sensor::setMockValue(SensorType::Tps1, 20);
	eth.moveTimeForwardMs(50);
	engine->periodicSlowCallback();
	EXPECT_TRUE(getBurst().isBurstKnockActive);
	EXPECT_FLOAT_EQ(getAe().deltaTps, 20);
	EXPECT_NEAR(getBurst().burstKnockTpsRate, 100, 0.001f);
	EXPECT_FLOAT_EQ(getBurst().getTimingRetard(), 10);

	// Age the jump out through the same dispatch path, then observe decay.
	for (int i = 0; i < 4; i++) {
		eth.moveTimeForwardMs(50);
		engine->periodicSlowCallback();
	}
	ASSERT_FALSE(getAe().isAboveAccelThreshold);
	const float retard = getBurst().getTimingRetard();
	ASSERT_GT(retard, 0);
	ASSERT_LT(retard, 10);
	eth.moveTimeForwardSec(0.5f);
	engine->periodicSlowCallback();
	EXPECT_NEAR(getBurst().getTimingRetard(), retard - 2.5f, 0.001f);
}

TEST(BurstKnock, RetardAddsToRollingLaunchAndDecaysIndependently) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupBurst(6, 2);
	engine->rpmCalculator.setRpmValue(3500);
	engineConfiguration->timingMode = TM_DYNAMIC;
	setTable(config->ignitionTable, 25);
	const float baseline = engine->ignitionState.getWrappedAdvance(3500, 50);
	ASSERT_TRUE(std::isfinite(baseline));
	engineConfiguration->rollingLaunchEnabled = true;
	engineConfiguration->rollingLaunchTimingRetard = 10;
	engineConfiguration->rollingLaunchRampOutTime = 1;
	Sensor::setMockValue(SensorType::VehicleSpeed, 30);
	auto& rolling = engine->module<RollingLaunchControl>().unmock();
	rolling.luaRollingLaunchState = true;
	rolling.onSlowCallback();
	ASSERT_TRUE(rolling.isRollingLaunchArmed);
	feedTransient(300, true);
	EXPECT_NEAR(engine->ignitionState.getWrappedAdvance(3500, 50), baseline - 16, 0.001f);

	rolling.luaRollingLaunchState = false;
	rolling.onSlowCallback();
	feedTransient(0, false);
	eth.moveTimeForwardSec(0.5f);
	rolling.onSlowCallback();
	getBurst().onSlowCallback();
	EXPECT_NEAR(engine->ignitionState.getWrappedAdvance(3500, 50), baseline - 9.5f, 0.001f);
	eth.moveTimeForwardSec(0.5f);
	rolling.onSlowCallback();
	getBurst().onSlowCallback();
	EXPECT_NEAR(engine->ignitionState.getWrappedAdvance(3500, 50), baseline - 3, 0.001f);
	eth.moveTimeForwardSec(1);
	getBurst().onSlowCallback();
	EXPECT_NEAR(engine->ignitionState.getWrappedAdvance(3500, 50), baseline, 0.001f);
}
