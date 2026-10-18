#include "pch.h"

#include "init.h"
#include "tps.h"
#include "tps_smoothing.h"
#include "defaults.h"
#include "functional_sensor.h"

static void configureAnalogTps() {
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_0;
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_NONE;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_2;
	engineConfiguration->tps2_2AdcChannel = EFI_ADC_NONE;
	engineConfiguration->tpsMin = 200;
	engineConfiguration->tpsMax = 800;
	engineConfiguration->tps2Min = 200;
	engineConfiguration->tps2Max = 800;
	engineConfiguration->tpsExpAverageAlpha = 1;
	engineConfiguration->tpsExpAverageResetThreshold = 0;
	engineConfiguration->tpsExpAverageResetTimeMs = 0;
}

static void seedTps(float value) {
	// Make the starting point deterministic even with the fork's static filter history.
	engineConfiguration->tpsExpAverageAlpha = 1;
	Sensor::setMockValue(SensorType::Tps1Unfiltered, value);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), value);
	engineConfiguration->tpsExpAverageAlpha = 0.5f;
}

TEST(TpsSmoothing, InitialZeroIsARealSample) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SmartExpAverage filter;
	EXPECT_FLOAT_EQ(filter.update(0, 0.5f, 0, 0), 0);
	// A closed-throttle sample participates in the next exponential average.
	EXPECT_FLOAT_EQ(filter.update(20, 0.5f, 0, 0), 10);
}

TEST(TpsSmoothing, InvalidReadingClearsHistory) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureAnalogTps();
	initTps();
	seedTps(20);
	Sensor::setInvalidMockValue(SensorType::Tps1Unfiltered);
	EXPECT_FALSE(Sensor::get(SensorType::Tps1).Valid);
	Sensor::setMockValue(SensorType::Tps1Unfiltered, 80);
	// Seed recovery from the new valid input, not from before the fault.
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 80);
}

TEST(TpsSmoothing, ReinitializationClearsHistory) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureAnalogTps();
	initTps();
	seedTps(20);
	Sensor::setMockValue(SensorType::Tps1Unfiltered, 40);
	ASSERT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 30);
	rememberCurrentConfiguration();
	deinitTps();
	initTps();
	Sensor::setMockValue(SensorType::Tps1Unfiltered, 80);
	// The new sensor configuration starts with a fresh filter.
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 80);
}

TEST(TpsSmoothing, SentHasOneResolvedProvider) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureAnalogTps();
	engineConfiguration->sentEtbType = SentEtbType::GM_TYPE_1;
	engineConfiguration->tpsExpAverageAlpha = 0.5f;
	EXPECT_NO_FATAL_ERROR(initTps());
	extern SentTps sentTps;
	EXPECT_EQ(Sensor::getSensorOfType(SensorType::Tps1Unfiltered), &sentTps);
	EXPECT_TRUE(Sensor::isRedundant(SensorType::Tps1));
	sentTps.setValidValue(20, getTimeNowNt());
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 20);
	sentTps.setValidValue(40, getTimeNowNt());
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 30);
	eth.moveTimeForwardMs(201);
	EXPECT_EQ(Sensor::get(SensorType::Tps1).Code, UnexpectedCode::Timeout);
}

TEST(TpsSmoothing, NonfiniteAlphaIsRepaired) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->tpsExpAverageAlpha = NAN;
	applyDefaultsOrFixAfterBurn(nullptr);
	EXPECT_FLOAT_EQ(engineConfiguration->tpsExpAverageAlpha, 1);
}

TEST(TpsSmoothing, UnconfiguredTpsLeavesResolvedSlotAvailable) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_NONE;
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_NONE;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;
	engineConfiguration->tps2_2AdcChannel = EFI_ADC_NONE;
	initTps();
	EXPECT_EQ(Sensor::getSensorOfType(SensorType::Tps1), nullptr);
	EXPECT_EQ(Sensor::getSensorOfType(SensorType::Tps2), nullptr);
	StoredValueSensor externalTps(SensorType::Tps1, MS2NT(300));
	ASSERT_TRUE(externalTps.Register());
	externalTps.setValidValue(42, getTimeNowNt());
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 42);
	externalTps.unregister();
}

TEST(TpsSmoothing, NonfiniteReadingDoesNotPoisonRecovery) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureAnalogTps();
	initTps();
	seedTps(20);
	Sensor::setMockValue(SensorType::Tps1Unfiltered, NAN);
	EXPECT_FALSE(Sensor::get(SensorType::Tps1).Valid);
	Sensor::setMockValue(SensorType::Tps1Unfiltered, 80);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 80);
}

TEST(TpsSmoothing, Defaults) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_FLOAT_EQ(engineConfiguration->tpsExpAverageAlpha, 1);
	EXPECT_FLOAT_EQ(engineConfiguration->tpsExpAverageResetThreshold, 0);
	EXPECT_FLOAT_EQ(engineConfiguration->tpsExpAverageResetTimeMs, 0);
}

TEST(TpsSmoothing, CalibrationRepairPreservesDisabledReset) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	for (float alpha : {0.0f, -1.0f, 1.1f, INFINITY, NAN}) {
		engineConfiguration->tpsExpAverageAlpha = alpha;
		EXPECT_TRUE(applyDefaultsOrFixAfterBurn(nullptr));
		EXPECT_FLOAT_EQ(engineConfiguration->tpsExpAverageAlpha, 1);
	}
	engineConfiguration->tpsExpAverageAlpha = 0.25f;
	applyDefaultsOrFixAfterBurn(nullptr);
	EXPECT_FLOAT_EQ(engineConfiguration->tpsExpAverageAlpha, 0.25f);
	EXPECT_FLOAT_EQ(engineConfiguration->tpsExpAverageResetThreshold, 0);
	EXPECT_FLOAT_EQ(engineConfiguration->tpsExpAverageResetTimeMs, 0);
	for (float invalid : {-1.0f, NAN, INFINITY}) {
		engineConfiguration->tpsExpAverageResetThreshold = invalid;
		engineConfiguration->tpsExpAverageResetTimeMs = invalid;
		EXPECT_TRUE(applyDefaultsOrFixAfterBurn(nullptr));
		EXPECT_FLOAT_EQ(engineConfiguration->tpsExpAverageResetThreshold, 10);
		EXPECT_FLOAT_EQ(engineConfiguration->tpsExpAverageResetTimeMs, 50);
	}
}

TEST(TpsSmoothing, TransientResetThresholdAndTimeBoundaries) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	SmartExpAverage filter;
	EXPECT_FLOAT_EQ(filter.update(20, 0.5f, 10, 50), 20);
	eth.moveTimeForwardMs(49);
	// Equality meets the threshold, while the elapsed-time check is strict.
	EXPECT_FLOAT_EQ(filter.update(30, 0.5f, 10, 50), 30);
	eth.moveTimeForwardMs(50);
	EXPECT_FLOAT_EQ(filter.update(40, 0.5f, 10, 50), 35);
	eth.moveTimeForwardMs(1);
	EXPECT_FLOAT_EQ(filter.update(20, 0.5f, 10, 50), 20);
	eth.moveTimeForwardMs(1);
	EXPECT_FLOAT_EQ(filter.update(29, 0.5f, 10, 50), 24.5f);
}

TEST(TpsSmoothing, BothResetSettingsMustBePositive) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	for (auto settings : {std::pair{0.0f, 50.0f}, std::pair{10.0f, 0.0f}}) {
		SmartExpAverage filter;
		EXPECT_FLOAT_EQ(filter.update(20, 0.5f, settings.first, settings.second), 20);
		eth.moveTimeForwardMs(1);
		EXPECT_FLOAT_EQ(filter.update(80, 0.5f, settings.first, settings.second), 50);
	}
}

TEST(TpsSmoothing, InvalidAlphaPassesThroughUntilBurn) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	for (float alpha : {0.0f, -1.0f, 1.1f, INFINITY, NAN}) {
		SmartExpAverage filter;
		EXPECT_FLOAT_EQ(filter.update(20, alpha, 0, 0), 20);
		EXPECT_FLOAT_EQ(filter.update(80, alpha, 0, 0), 80);
	}
}

TEST(TpsSmoothing, IndependentFiltersAndDriverIntent) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureAnalogTps();
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;
	engineConfiguration->tpsExpAverageAlpha = 0.5f;
	initTps();
	Sensor::setMockValue(SensorType::Tps1Primary, 10);
	Sensor::setMockValue(SensorType::Tps2Primary, 60);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 10);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps2), 60);
	Sensor::setMockValue(SensorType::Tps1Primary, 30);
	Sensor::setMockValue(SensorType::Tps2Primary, 80);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1Unfiltered), 30);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps2Unfiltered), 80);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 20);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps2), 70);
	// Match the fork/PPS convention: each resolved read advances the EMA.
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::DriverThrottleIntent), 25);
	EXPECT_FALSE(Sensor::isRedundant(SensorType::Tps1));
}

TEST(TpsSmoothing, DisagreementIsCheckedBeforeFiltering) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureAnalogTps();
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_1;
	engineConfiguration->tps1SecondaryMin = 800;
	engineConfiguration->tps1SecondaryMax = 200;
	engineConfiguration->etbSplit = MAX_TPS_PPS_DISCREPANCY;
	engineConfiguration->tpsExpAverageAlpha = 0.1f;
	initTps();
	EXPECT_TRUE(Sensor::isRedundant(SensorType::Tps1));
	EXPECT_TRUE(Sensor::isRedundant(SensorType::Tps1Unfiltered));
	Sensor::setMockValue(SensorType::Tps1Primary, 20);
	Sensor::setMockValue(SensorType::Tps1Secondary, 20 + MAX_TPS_PPS_DISCREPANCY);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 20 + MAX_TPS_PPS_DISCREPANCY / 2.0f);
	Sensor::setMockValue(SensorType::Tps1Secondary, 20.5f + MAX_TPS_PPS_DISCREPANCY);
	EXPECT_EQ(Sensor::get(SensorType::Tps1Unfiltered).Code, UnexpectedCode::Inconsistent);
	EXPECT_EQ(Sensor::get(SensorType::Tps1).Code, UnexpectedCode::Inconsistent);
	Sensor::setMockValue(SensorType::Tps1Primary, 80);
	Sensor::setMockValue(SensorType::Tps1Secondary, 80);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 80);
}

TEST(TpsSmoothing, FordRedundancyIsPreserved) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureAnalogTps();
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_1;
	engineConfiguration->tps1SecondaryMin = 800;
	engineConfiguration->tps1SecondaryMax = 200;
	engineConfiguration->useFordRedundantTps = true;
	engineConfiguration->tpsSecondaryMaximum = 50;
	engineConfiguration->tpsExpAverageAlpha = 0.5f;
	initTps();
	ASSERT_TRUE(Sensor::isRedundant(SensorType::Tps1));
	Sensor::setMockValue(SensorType::Tps1Primary, 20);
	Sensor::setMockValue(SensorType::Tps1Secondary, 40);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 20);
	Sensor::setMockValue(SensorType::Tps1Primary, 30);
	Sensor::setMockValue(SensorType::Tps1Secondary, 60);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1Unfiltered), 30);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 25);
}

TEST(TpsSmoothing, CalibrationInvalidityAndTimeoutPropagate) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureAnalogTps();
	engineConfiguration->tpsExpAverageAlpha = 0.5f;
	initTps();
	auto sensor = static_cast<FunctionalSensor*>(const_cast<Sensor*>(Sensor::getSensorOfType(SensorType::Tps1Primary)));
	ASSERT_NE(sensor, nullptr);
	sensor->postRawValue(1, getTimeNowNt());
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 0);
	sensor->postRawValue(1.6f, getTimeNowNt());
	EXPECT_NEAR(Sensor::getOrZero(SensorType::Tps1Unfiltered), 20, 0.001f);
	EXPECT_NEAR(Sensor::getOrZero(SensorType::Tps1), 10, 0.001f);
	EXPECT_FLOAT_EQ(Sensor::getRaw(SensorType::Tps1Primary), 1.6f);
	sensor->postRawValue(-1, getTimeNowNt());
	EXPECT_FALSE(Sensor::get(SensorType::Tps1).Valid);
	sensor->postRawValue(2.5f, getTimeNowNt());
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 50);
	eth.moveTimeForwardMs(11);
	EXPECT_EQ(Sensor::get(SensorType::Tps1).Code, UnexpectedCode::Timeout);
	sensor->postRawValue(4, getTimeNowNt());
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 100);
}

TEST(TpsSmoothing, ObdProviderIsNotReplaced) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->consumeObdSensors = true;
	StoredValueSensor obdTps(SensorType::Tps1, MS2NT(300));
	ASSERT_TRUE(obdTps.Register());
	obdTps.setValidValue(33, getTimeNowNt());
	initTps();
	EXPECT_EQ(Sensor::getSensorOfType(SensorType::Tps1), &obdTps);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 33);
	rememberCurrentConfiguration();
	deinitTps();
	EXPECT_EQ(Sensor::getSensorOfType(SensorType::Tps1), &obdTps);
	obdTps.unregister();
}

TEST(TpsSmoothing, PreRegisteredRawProviderCanUseFilter) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureAnalogTps();
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_NONE;
	StoredValueSensor rawTps(SensorType::Tps1Unfiltered, MS2NT(300));
	ASSERT_TRUE(rawTps.Register());
	initTps();
	rawTps.setValidValue(40, getTimeNowNt());
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 40);
	engineConfiguration->tpsExpAverageAlpha = 0.5f;
	rawTps.setValidValue(60, getTimeNowNt());
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 50);
	rememberCurrentConfiguration();
	deinitTps();
	EXPECT_EQ(Sensor::getSensorOfType(SensorType::Tps1Unfiltered), &rawTps);
	EXPECT_EQ(Sensor::getSensorOfType(SensorType::Tps1), nullptr);
	rawTps.unregister();
}

TEST(TpsSmoothing, FaultAndRecoveryOnlyResetTheAffectedThrottle) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	configureAnalogTps();
	engineConfiguration->tpsExpAverageAlpha = 0.5f;
	initTps();
	Sensor::setMockValue(SensorType::Tps1Primary, 20);
	Sensor::setMockValue(SensorType::Tps2Primary, 60);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 20);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps2), 60);

	Sensor::setInvalidMockValue(SensorType::Tps1Primary);
	Sensor::setMockValue(SensorType::Tps2Primary, 80);
	EXPECT_FALSE(Sensor::get(SensorType::Tps1).Valid);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps2), 70);
	Sensor::setMockValue(SensorType::Tps1Primary, 80);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 80);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps2), 75);

	Sensor::setMockValue(SensorType::Tps2Primary, INFINITY);
	EXPECT_FALSE(Sensor::get(SensorType::Tps2).Valid);
	Sensor::setMockValue(SensorType::Tps1Primary, 60);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps1), 70);
	Sensor::setMockValue(SensorType::Tps2Primary, 20);
	EXPECT_FLOAT_EQ(Sensor::getOrZero(SensorType::Tps2), 20);
}
