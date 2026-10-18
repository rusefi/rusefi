#include "pch.h"

static WotEnrichment& getWot() {
	return engine->module<WotEnrichment>().unmock();
}

static void setupWot() {
	engineConfiguration->useEngineStateMachine = true;
	engineConfiguration->wotEnrichmentEnabled = true;
	engineConfiguration->smWotTpsThreshold = 90;
	for (size_t i = 0; i < WOT_ENRICHMENT_SIZE; i++) {
		float time = 30.0f * i / (WOT_ENRICHMENT_SIZE - 1);
		engineConfiguration->wotEnrichmentTimeBins[i] = time;
		engineConfiguration->wotEnrichmentAfrAdder[i] = -0.1f * time;
	}
}

static void setWotAdder(float adder) {
	for (auto& value : engineConfiguration->wotEnrichmentAfrAdder) {
		value = adder;
	}
}

static void stepWot(float throttle, float rpm = 3000) {
	engine->rpmCalculator.setRpmValue(rpm);
	Sensor::setMockValue(SensorType::DriverThrottleIntent, throttle);
	engine->periodicSlowCallback();
}

TEST(WotEnrichment, DefaultsAreDisabledAndNeutral) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_FALSE(engineConfiguration->wotEnrichmentEnabled);
	for (size_t i = 0; i < WOT_ENRICHMENT_SIZE; i++) {
		EXPECT_NEAR(engineConfiguration->wotEnrichmentTimeBins[i], 30.0f * i / 7, 0.001f);
		EXPECT_FLOAT_EQ(engineConfiguration->wotEnrichmentAfrAdder[i], 0);
	}
	EXPECT_FLOAT_EQ(getWot().getAfrAdder(), 0);
}

TEST(WotEnrichment, InterpolatesElapsedTimeAndCapsAtThirtySeconds) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupWot();
	stepWot(50);
	stepWot(95);
	EXPECT_TRUE(getWot().isWotEnrichmentActive);
	EXPECT_FLOAT_EQ(getWot().wotEnrichmentTimer, 0);
	eth.moveTimeForwardSec(15);
	stepWot(95);
	EXPECT_NEAR(getWot().getAfrAdder(), -1.5f, 0.001f);
	eth.moveTimeForwardSec(30);
	stepWot(95);
	EXPECT_FLOAT_EQ(getWot().wotEnrichmentTimer, 30);
	EXPECT_FLOAT_EQ(getWot().getAfrAdder(), -3);
}

TEST(WotEnrichment, ThrottleLiftResetsAndSeparateBurstsRestart) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupWot();
	stepWot(50);
	stepWot(95);
	eth.moveTimeForwardSec(10);
	stepWot(95);
	ASSERT_NEAR(getWot().getAfrAdder(), -1, 0.001f);
	stepWot(50);
	EXPECT_FALSE(getWot().isWotEnrichmentActive);
	EXPECT_FLOAT_EQ(getWot().getAfrAdder(), 0);
	EXPECT_FLOAT_EQ(getWot().wotEnrichmentTimer, 0);
	eth.moveTimeForwardSec(10);
	stepWot(50);
	stepWot(95);
	eth.moveTimeForwardSec(2);
	stepWot(95);
	EXPECT_NEAR(getWot().getAfrAdder(), -0.2f, 0.001f);
}

TEST(WotEnrichment, UsesStateMachineWotThresholdAndRunningState) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupWot();
	setWotAdder(-1);
	stepWot(90);
	EXPECT_FALSE(getWot().isWotEnrichmentActive);
	stepWot(91);
	EXPECT_FLOAT_EQ(getWot().getAfrAdder(), -1);
	stepWot(95, 0);
	EXPECT_FLOAT_EQ(getWot().getAfrAdder(), 0);
	stepWot(95, 200);
	EXPECT_FLOAT_EQ(getWot().getAfrAdder(), 0);
	stepWot(95);
	EXPECT_FLOAT_EQ(getWot().getAfrAdder(), -1);
	Sensor::setInvalidMockValue(SensorType::DriverThrottleIntent);
	engine->periodicSlowCallback();
	EXPECT_FALSE(getWot().isWotEnrichmentActive);
}

TEST(WotEnrichment, EitherEnableCanDisableCorrection) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupWot();
	setWotAdder(-1);
	stepWot(50);
	stepWot(95);
	engineConfiguration->wotEnrichmentEnabled = false;
	stepWot(95);
	EXPECT_FLOAT_EQ(getWot().getAfrAdder(), 0);
	engineConfiguration->wotEnrichmentEnabled = true;
	stepWot(95);
	EXPECT_FLOAT_EQ(getWot().getAfrAdder(), -1);
	engineConfiguration->useEngineStateMachine = false;
	stepWot(95);
	EXPECT_FLOAT_EQ(getWot().getAfrAdder(), 0);
}

TEST(WotEnrichment, FuelMassAndPublishedTargetsUseAdjustedAfr) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupWot();
	setWotAdder(-2);
	engineConfiguration->stoichRatioPrimary = 15;
	engineConfiguration->stoichRatioSecondary = 10;
	setTable(config->lambdaTable, 1);
	stepWot(50);
	EXPECT_NEAR(engine->fuelComputer.getCycleFuel(15, 3000, 50), 1, 0.001f);
	stepWot(95);
	EXPECT_NEAR(engine->fuelComputer.getCycleFuel(15, 3000, 50), 15.0f / 13, 0.001f);
	EXPECT_NEAR(engine->fuelComputer.targetAFR, 13, 0.01f);
	EXPECT_NEAR(engine->fuelComputer.targetLambda, 13.0f / 15, 0.001f);

	Sensor::setMockValue(SensorType::FuelEthanolPercent, 50);
	EXPECT_NEAR(engine->fuelComputer.getCycleFuel(15, 3000, 50), 15.0f / 10.5f, 0.001f);
	EXPECT_NEAR(engine->fuelComputer.targetAFR, 10.5f, 0.01f);
	EXPECT_NEAR(engine->fuelComputer.targetLambda, 10.5f / 12.5f, 0.001f);
}

TEST(WotEnrichment, PositiveAfrAdderLeansMixture) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupWot();
	setWotAdder(2);
	engineConfiguration->stoichRatioPrimary = 15;
	setTable(config->lambdaTable, 1);
	stepWot(50);
	stepWot(95);
	EXPECT_NEAR(engine->fuelComputer.getCycleFuel(15, 3000, 50), 15.0f / 17, 0.001f);
	EXPECT_NEAR(engine->fuelComputer.targetAFR, 17, 0.01f);
}

// Regression coverage established against the source behavior before the fixes.
TEST(WotEnrichmentRegression, FirstWotSampleStartsAtZero) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupWot();
	eth.moveTimeForwardSec(10);
	stepWot(95);
	EXPECT_FLOAT_EQ(getWot().wotEnrichmentTimer, 0);
}

TEST(WotEnrichmentRegression, WotEntryExcludesTimeBeforeTheFirstWotSample) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupWot();
	stepWot(50);
	eth.moveTimeForwardSec(5);
	stepWot(95);
	EXPECT_FLOAT_EQ(getWot().wotEnrichmentTimer, 0);
}

TEST(WotEnrichmentRegression, EngineStopAndBurnClearCorrection) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupWot();
	setWotAdder(-1);
	stepWot(95);
	getWot().onEngineStop();
	EXPECT_FLOAT_EQ(getWot().getAfrAdder(), 0);
	EXPECT_FALSE(getWot().isWotEnrichmentActive);
	EXPECT_FLOAT_EQ(getWot().wotEnrichmentTimer, 0);
	stepWot(95);
	getWot().onConfigurationChange(nullptr);
	EXPECT_FLOAT_EQ(getWot().getAfrAdder(), 0);
	EXPECT_FALSE(getWot().isWotEnrichmentActive);
	EXPECT_FLOAT_EQ(getWot().wotEnrichmentTimer, 0);
}

TEST(WotEnrichmentRegression, NonpositiveAdjustedAfrKeepsBaseTarget) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupWot();
	setWotAdder(-10);
	engineConfiguration->stoichRatioPrimary = 10;
	setTable(config->lambdaTable, 1);
	stepWot(95);
	EXPECT_FLOAT_EQ(engine->fuelComputer.getCycleFuel(10, 3000, 50), 1);
	EXPECT_FLOAT_EQ(engine->fuelComputer.targetAFR, 10);
	EXPECT_FLOAT_EQ(engine->fuelComputer.targetLambda, 1);
}
