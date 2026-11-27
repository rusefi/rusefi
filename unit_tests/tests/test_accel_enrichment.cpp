/**
 * @file	test_accel_enrichment.cpp
 *
 * See also test_fuel_wall_wetting.cpp
 *
 * @date Apr 29, 2014
 *  	Author: Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "accel_enrichment.h"
#include "defaults.h"
#include "fuel_math.h"
#include "rusefi_lua.h"
#include "tunerstudio.h"
#include "tunerstudio_impl.h"

// [tag:tps_rate_detector] A held TPS step, one sample per 5 ms (the fast-callback period):
// enough samples at `from` to fill the median window of tpsEventConfirmation, then `to`
// for as many samples as the confirmation needs. The event starts on the last sample.
static void feedTpsStep(TpsAccelEnrichment& ae, float from, float to) {
	efitick_t nowNt = getTimeNowNt();
	int confirmation = config->tpsEventConfirmation;
	for (int i = 0; i < 2 * confirmation - 1; i++) {
		nowNt += US2NT(5000);
		ae.onNewValue(from, nowNt);
	}
	for (int i = 0; i < confirmation; i++) {
		nowNt += US2NT(5000);
		ae.onNewValue(to, nowNt);
	}
}

// [tag:tps_rate_detector] Fast callbacks 5 ms apart with a mocked TPS.
static void fastCallbacksWithTps(EngineTestHelper& eth, float tps, int count) {
	Sensor::setMockValue(SensorType::Tps1, tps);
	for (int i = 0; i < count; i++) {
		eth.moveTimeForwardMs(5);
		engine->periodicFastCallback();
	}
}

// [tag:tps_rate_detector] Detector coverage: rate window, event start/extreme TPS,
// hold time, closing events, re-entrant samples, gaps and invalid TPS. These tests use
// one-sample confirmation so each sample is seen as fed; TpsEventConfirmation covers
// the median filter.
class TpsRateDetector : public ::testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};
	efitick_t nowNt = 0;

	void SetUp() override {
		engineConfiguration->tpsAccelRateThreshold = 100;
		engineConfiguration->tpsDecelRateThreshold = 100;
		engineConfiguration->tpsAccelHoldTime = 0.1f;
		config->tpsEventConfirmation = TPS_EVENT_CONFIRM_1;
		ae().resetAE();
		nowNt = getTimeNowNt();
	}

	TpsAccelEnrichment& ae() {
		return engine->module<TpsAccelEnrichment>().unmock();
	}

	// One sample per 5 ms, the 200 Hz fast-callback rate.
	void feed(float tps, int samples = 1) {
		for (int i = 0; i < samples; i++) {
			nowNt += US2NT(5000);
			ae().onNewValue(tps, nowNt);
		}
	}
};

TEST_F(TpsRateDetector, MeasuresRateOverTwoSampleIntervals) {
	feed(10);
	EXPECT_FLOAT_EQ(0, ae().tpsRate); // no previous sample
	feed(10);
	EXPECT_FLOAT_EQ(0, ae().tpsRate);
	feed(11);
	// 1% over 10 ms
	EXPECT_NEAR(100, ae().tpsRate, 0.01f);
	EXPECT_FALSE(ae().isAboveAccelThreshold); // not above 100 %/s
	feed(12);
	EXPECT_NEAR(200, ae().tpsRate, 0.01f);
	EXPECT_TRUE(ae().isAboveAccelThreshold);
}

TEST_F(TpsRateDetector, OpeningEventTracksStartAndHighestTps) {
	feed(10, 3);
	feed(20);
	ASSERT_TRUE(ae().isAboveAccelThreshold);
	EXPECT_TRUE(ae().isAccelEventTriggered());
	EXPECT_FALSE(ae().isAccelEventTriggered()); // latched once per event
	// The window start, not the previous sample, is the event's start TPS.
	EXPECT_FLOAT_EQ(10, ae().tpsFrom);
	EXPECT_FLOAT_EQ(20, ae().tpsTo);
	feed(40);
	EXPECT_FLOAT_EQ(10, ae().tpsFrom);
	EXPECT_FLOAT_EQ(40, ae().tpsTo);
	EXPECT_FLOAT_EQ(30, ae().deltaTps);
	EXPECT_NEAR(3000, ae().peakTpsRate, 0.1f);
	EXPECT_FALSE(ae().isAccelEventTriggered()); // same event

	// Throttle stops: the event is held for 100 ms after the last opening sample.
	feed(40, 2);
	EXPECT_TRUE(ae().isAboveAccelThreshold);
	feed(40, 18);
	EXPECT_TRUE(ae().isAboveAccelThreshold);
	feed(40, 2);
	EXPECT_FALSE(ae().isAboveAccelThreshold);
	EXPECT_FLOAT_EQ(0, ae().deltaTps);
	EXPECT_FLOAT_EQ(0, ae().peakTpsRate);

	// A new opening is a new event.
	feed(60);
	EXPECT_TRUE(ae().isAboveAccelThreshold);
	EXPECT_TRUE(ae().isAccelEventTriggered());
	EXPECT_FLOAT_EQ(40, ae().tpsFrom);
}

TEST_F(TpsRateDetector, ClosingEventEndsOpeningEvent) {
	feed(10, 3);
	feed(50);
	ASSERT_TRUE(ae().isAboveAccelThreshold);
	feed(50);
	feed(30);
	EXPECT_FALSE(ae().isAboveAccelThreshold);
	EXPECT_TRUE(ae().isBelowDecelThreshold);
	EXPECT_FLOAT_EQ(50, ae().tpsFrom);
	EXPECT_FLOAT_EQ(30, ae().tpsTo);
	feed(20);
	EXPECT_FLOAT_EQ(20, ae().tpsTo); // lowest TPS reached
	EXPECT_FLOAT_EQ(-30, ae().deltaTps);
	EXPECT_LT(ae().peakTpsRate, -100);
	feed(20, 23);
	EXPECT_FALSE(ae().isBelowDecelThreshold);
}

TEST_F(TpsRateDetector, ZeroThresholdDisablesDirection) {
	engineConfiguration->tpsAccelRateThreshold = 0;
	engineConfiguration->tpsDecelRateThreshold = 0;
	feed(0, 3);
	feed(80);
	EXPECT_FALSE(ae().isAboveAccelThreshold);
	feed(0, 2);
	EXPECT_FALSE(ae().isBelowDecelThreshold);
	EXPECT_NEAR(-8000, ae().tpsRate, 0.1f); // still reported
}

TEST_F(TpsRateDetector, ZeroHoldEndsEventWhenThrottleStops) {
	engineConfiguration->tpsAccelHoldTime = 0;
	feed(0, 3);
	feed(30);
	EXPECT_TRUE(ae().isAboveAccelThreshold);
	feed(30, 2);
	EXPECT_FALSE(ae().isAboveAccelThreshold);
}

TEST_F(TpsRateDetector, ReentrantSamplesAndGaps) {
	feed(10, 3);
	// A sample closer than 2 ms to the previous one is ignored.
	nowNt += US2NT(500);
	ae().onNewValue(90, nowNt);
	EXPECT_FALSE(ae().isAboveAccelThreshold);
	EXPECT_FLOAT_EQ(0, ae().tpsRate);

	// After a gap longer than 100 ms the window restarts: no rate from stale history.
	nowNt += US2NT(150000);
	ae().onNewValue(90, nowNt);
	EXPECT_FALSE(ae().isAboveAccelThreshold);
	EXPECT_FLOAT_EQ(0, ae().tpsRate);
	feed(90);
	EXPECT_FLOAT_EQ(0, ae().tpsRate);
}

TEST_F(TpsRateDetector, InvalidTpsClearsEvent) {
	feed(0, 3);
	feed(50);
	ASSERT_TRUE(ae().isAboveAccelThreshold);
	ae().onInvalidTps();
	EXPECT_FALSE(ae().isAboveAccelThreshold);
	EXPECT_FALSE(ae().isAccelEventTriggered());
	EXPECT_FLOAT_EQ(0, ae().tpsRate);
	// History is gone: the next valid sample cannot form a rate.
	feed(50);
	EXPECT_FLOAT_EQ(0, ae().tpsRate);
}

// [tag:tps_rate_detector] tpsEventConfirmation N: the detector uses the median of the
// last 2N-1 samples, so a TPS change must last N samples. Shorter spikes disappear in
// both directions; held steps pass N-1 samples late.
class TpsEventConfirmation : public TpsRateDetector {
protected:
	int openings = 0;
	int closings = 0;

	void setConfirmation(int samples) {
		config->tpsEventConfirmation = static_cast<tps_event_confirmation_e>(samples);
		ae().resetAE();
		openings = 0;
		closings = 0;
	}

	// Feeds samples and counts event starts.
	void feedCounting(float tps, int samples = 1) {
		for (int i = 0; i < samples; i++) {
			bool wasOpening = ae().isAboveAccelThreshold;
			bool wasClosing = ae().isBelowDecelThreshold;
			feed(tps);
			if (ae().isAboveAccelThreshold && !wasOpening) {
				openings++;
			}
			if (ae().isBelowDecelThreshold && !wasClosing) {
				closings++;
			}
		}
	}
};

TEST_F(TpsEventConfirmation, SingleSampleSpike) {
	// One sample: the spike is an opening, and its return a closing.
	setConfirmation(1);
	feedCounting(20, 5);
	feedCounting(23);
	feedCounting(20, 30);
	EXPECT_EQ(1, openings);
	EXPECT_EQ(1, closings);

	// Two samples: the spike is gone in both directions.
	setConfirmation(2);
	feedCounting(20, 5);
	feedCounting(23);
	feedCounting(20, 30);
	EXPECT_EQ(0, openings);
	EXPECT_EQ(0, closings);
	EXPECT_FLOAT_EQ(0, ae().tpsRate);
}

TEST_F(TpsEventConfirmation, TwoSampleSpike) {
	setConfirmation(2);
	feedCounting(20, 5);
	feedCounting(23, 2);
	feedCounting(20, 30);
	EXPECT_EQ(1, openings);
	EXPECT_EQ(1, closings);

	setConfirmation(3);
	feedCounting(20, 5);
	feedCounting(23, 2);
	feedCounting(20, 30);
	EXPECT_EQ(0, openings);
	EXPECT_EQ(0, closings);
}

TEST_F(TpsEventConfirmation, HeldStepStartsAfterConfirmationSamples) {
	for (int samples = 1; samples <= 3; samples++) {
		setConfirmation(samples);
		feed(20, 5);
		int samplesUntilEvent = 0;
		while (!ae().isAboveAccelThreshold && samplesUntilEvent < 10) {
			feed(50);
			samplesUntilEvent++;
		}
		EXPECT_EQ(samples, samplesUntilEvent) << "confirmation " << samples;
		EXPECT_FLOAT_EQ(20, ae().tpsFrom) << "confirmation " << samples;
		EXPECT_FLOAT_EQ(50, ae().tpsTo) << "confirmation " << samples;
		EXPECT_TRUE(ae().isAccelEventTriggered());
	}
}

TEST_F(TpsEventConfirmation, RampLatencyGrowsFiveMsPerSample) {
	// 1300 %/s: 6.5 % per 5 ms sample.
	for (int samples = 1; samples <= 3; samples++) {
		setConfirmation(samples);
		feed(10, 5);
		float tps = 10;
		int samplesUntilEvent = 0;
		while (!ae().isAboveAccelThreshold && samplesUntilEvent < 10) {
			tps += 6.5f;
			feed(tps);
			samplesUntilEvent++;
		}
		EXPECT_EQ(samples, samplesUntilEvent) << "confirmation " << samples;
	}
}

TEST_F(TpsEventConfirmation, SteppedTpsFromCanIsDetected) {
	// A TPS received at 20 Hz moves in steps held for ten samples (50 ms).
	setConfirmation(3);
	feedCounting(20, 5);
	for (int step = 1; step <= 4; step++) {
		feedCounting(20 + 5 * step, 10);
	}
	// The 100 ms hold time joins the steps into one event.
	EXPECT_EQ(1, openings);
	EXPECT_EQ(0, closings);
	EXPECT_FLOAT_EQ(20, ae().tpsFrom);
	EXPECT_FLOAT_EQ(40, ae().tpsTo);

	// Without hold time every step is an event of its own.
	engineConfiguration->tpsAccelHoldTime = 0;
	setConfirmation(3);
	feedCounting(20, 5);
	for (int step = 1; step <= 4; step++) {
		feedCounting(20 + 5 * step, 10);
	}
	EXPECT_EQ(4, openings);
	EXPECT_EQ(0, closings);
}

TEST_F(TpsEventConfirmation, NoiseRejection) {
	// Deterministic noise on a steady 20 % TPS: the sum of four uniform integers,
	// about 0.3 % standard deviation, so every platform produces the same samples.
	auto countEvents = [this](int samples) {
		setConfirmation(samples);
		uint32_t seed = 12345;
		auto uniform = [&seed]() {
			seed = seed * 1664525u + 1013904223u;
			return static_cast<int>((seed >> 16) % 1001) - 500;
		};
		for (int i = 0; i < 12000; i++) { // 60 s
			int sum = uniform() + uniform() + uniform() + uniform();
			feedCounting(20 + sum * 0.0005196f);
		}
		return openings + closings;
	};
	int one = countEvents(1);
	int two = countEvents(2);
	int three = countEvents(3);
	EXPECT_GT(one, 100);
	EXPECT_LT(two * 20, one);
	EXPECT_LE(three, two);
}

TEST_F(TpsEventConfirmation, SettingChangeWhileRunning) {
	setConfirmation(3);
	feed(20, 5);
	config->tpsEventConfirmation = TPS_EVENT_CONFIRM_1;
	feed(50);
	EXPECT_TRUE(ae().isAboveAccelThreshold);
	EXPECT_FLOAT_EQ(20, ae().tpsFrom);
}

TEST(TpsEventConfirmationDefaults, DefaultAndOldTunes) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_EQ(TPS_EVENT_CONFIRM_2, config->tpsEventConfirmation);
	// Tunes that predate the setting store zero.
	config->tpsEventConfirmation = TPS_EVENT_CONFIRM_INVALID;
	EXPECT_TRUE(applyDefaultsOrFixAfterBurn(nullptr));
	EXPECT_EQ(TPS_EVENT_CONFIRM_2, config->tpsEventConfirmation);
	config->tpsEventConfirmation = TPS_EVENT_CONFIRM_3;
	applyDefaultsOrFixAfterBurn(nullptr);
	EXPECT_EQ(TPS_EVENT_CONFIRM_3, config->tpsEventConfirmation);
}

TEST(fuel, tpsRateDetectorRunsFromFastCallbackBeforeFuelMath) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->tpsAccelRateThreshold = 100;
	engineConfiguration->tpsAccelHoldTime = 0.2f;
	// Default: a TPS change must last two samples.
	ASSERT_EQ(TPS_EVENT_CONFIRM_2, config->tpsEventConfirmation);
	auto& ae = engine->module<TpsAccelEnrichment>().unmock();
	ae.resetAE();
	fastCallbacksWithTps(eth, 10, 4);
	EXPECT_FALSE(ae.isAboveAccelThreshold);
	fastCallbacksWithTps(eth, 60, 1);
	EXPECT_FALSE(ae.isAboveAccelThreshold); // first sample of the change: not confirmed yet
	fastCallbacksWithTps(eth, 60, 1);
	EXPECT_TRUE(ae.isAboveAccelThreshold);
	EXPECT_FLOAT_EQ(10, ae.tpsFrom);
	EXPECT_FLOAT_EQ(60, ae.tpsTo);
	// Further calls at the same instant are ignored, not treated as a zero-length window.
	engine->periodicFastCallback();
	engine->periodicFastCallback();
	EXPECT_TRUE(ae.isAboveAccelThreshold);
	EXPECT_FLOAT_EQ(60, ae.tpsTo);

	// Invalid TPS ends the event.
	Sensor::setInvalidMockValue(SensorType::Tps1);
	eth.moveTimeForwardMs(5);
	engine->periodicFastCallback();
	EXPECT_FALSE(ae.isAboveAccelThreshold);
}

TEST(fuel, testTpsAccelEnrichmentScheduling) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	setCrankOperationMode();

	engineConfiguration->tpsAccelRateThreshold = 100;
	engineConfiguration->tpsAccelHoldTime = 2;

	eth.setTriggerType(trigger_type_e::TT_HALF_MOON);

	eth.fireTriggerEvents2(/* count */ 4, 25 /* ms */);
	ASSERT_EQ( 1200,  Sensor::getOrZero(SensorType::Rpm)) << "RPM";
	// Establish the TPS baseline after crank synchronization, which clears AE state,
	// then a step held for the default two-sample confirmation.
	fastCallbacksWithTps(eth, 0, 3);
	fastCallbacksWithTps(eth, 7, 2);
	int expectedInvocationCounter = 1;
	ASSERT_EQ(expectedInvocationCounter, engine->module<TpsAccelEnrichment>()->onUpdateInvocationCounter);

	auto& ae = engine->module<TpsAccelEnrichment>().unmock();
	ASSERT_TRUE(ae.isAboveAccelThreshold);
	float expectedAEValue = interpolate3d(config->tpsTpsAccelTable,
		config->tpsTpsAccelToRpmBins, 7.0f,
		config->tpsTpsAccelFromRpmBins, 0.0f);

	eth.fireTriggerEvents2(/* count */ 1, 25 /* ms */);

	// it does not matter how many times we invoke 'getTpsEnrichment' - state does not change
	for (int i = 0; i < 20; i++) {
		ASSERT_NEAR(expectedAEValue, ae.getTpsEnrichment(), EPS4D);
	}

	expectedInvocationCounter++;
	ASSERT_EQ(expectedInvocationCounter, ae.onUpdateInvocationCounter);

	eth.engine.periodicFastCallback();
	eth.engine.periodicFastCallback();
	eth.engine.periodicFastCallback();

	ASSERT_EQ(expectedInvocationCounter, ae.onUpdateInvocationCounter);
}

static void doFractionalTpsIteration(int period, int divisor, int numCycles, std::vector<floatms_t> &tpsEnrich) {
	// every cycle
	engineConfiguration->tpsAccelFractionPeriod = period;
	// split into 2 portions
	engineConfiguration->tpsAccelFractionDivisor = divisor;

	auto& ae = engine->module<TpsAccelEnrichment>().unmock();
	ae.resetAE();
	for (int i = 0; i < numCycles; i++) {
		// The opening event lasts for the first engine cycle only; the pump spreads it.
		ae.isAboveAccelThreshold = i == 0;
		ae.tpsFrom = 0;
		ae.tpsTo = 10;
		ae.onEngineCycleTps();
		tpsEnrich[i] = ae.getTpsEnrichment();
	}
}

TEST(fuel, testAccelEnrichmentFractionalTps) {
	printf("====================================================================================== testAccelEnrichmentFractionalTps\r\n");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// setup
	engineConfiguration->tpsAccelHoldTime = 2;

	// fill tps2tps map (todo: there should be a better way?)
	static const float tpsTpsConst = 1.0f;
	for (int loadIndex = 0; loadIndex < TPS_TPS_ACCEL_TABLE; loadIndex++) {
		for (int rpmIndex = 0; rpmIndex < TPS_TPS_ACCEL_TABLE; rpmIndex++) {
			config->tpsTpsAccelTable[loadIndex][rpmIndex] = tpsTpsConst;
		}
	}

	initAccelEnrichment();

	engine->rpmCalculator.setRpmValue(600);
	engine->periodicFastCallback();

	const int numCycles = 4;
	std::vector<floatms_t> tpsEnrich(numCycles);

	// first, test the default behavior without fractional division
	doFractionalTpsIteration(1, 1, numCycles, tpsEnrich);
	// the portion for the first cycle is full-enriched and there's no enrichment for next cycles
	EXPECT_THAT(tpsEnrich, testing::ElementsAre(1.0f, 0.0f, 0.0f, 0.0f)) << "fractionalTps#1";

	// divide into 2 each cycle
	doFractionalTpsIteration(1, 2, numCycles, tpsEnrich);
	// we have half-portion for the first cycle, then 1/4-th and 1/8th and so on...
	EXPECT_THAT(tpsEnrich, testing::ElementsAre(0.5f, 0.25f, 0.125f, 0.0625f)) << "fractionalTps#2";

	// split every portion into 3 sub-portions (so the whole enrichment takes longer)
	doFractionalTpsIteration(3, 1, numCycles, tpsEnrich);
	// we have 1/3rd-portion for the first three cycles
	const float th = (1.0f / 3.0f);
	EXPECT_THAT(tpsEnrich, testing::ElementsAre(testing::FloatEq(th), testing::FloatEq(th), testing::FloatEq(th), 0.0f)) << "fractionalTps#3";

	// split every divided portion into 2 sub-portions (so the whole enrichment takes longer)
	doFractionalTpsIteration(2, 2, numCycles, tpsEnrich);
	// we have half-portion for the first two cycles, and 1/4-th portion for the next 2 cycles, and so on...
	EXPECT_THAT(tpsEnrich, testing::ElementsAre(0.25f, 0.25f, 0.125f, 0.125f)) << "fractionalTps#4";

}

TEST(fuel, testTpsAccelEnrichment) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->accelEnrichmentMode = AE_MODE_PREDICTIVE_MAP;
	// should return 0 if we are using predictive map
	EXPECT_EQ(0, engine->module<TpsAccelEnrichment>()->getTpsEnrichment());
}

TEST(fuel, tpsAccelFlexFuelCompensation) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->tpsAccelRateThreshold = 100;
	engineConfiguration->tpsAccelHoldTime = 2;
	// non-fractional so the raw enrichment is the table value directly
	engineConfiguration->tpsAccelFractionPeriod = 1;
	engineConfiguration->tpsAccelFractionDivisor = 1;

	// Constant AE table (1.0) and flat RPM correction so the base enrichment is a known 1.0
	setTable(config->tpsTpsAccelTable, 1.0f);
	setRpmTableBin(config->tpsTspCorrValuesBins);
	setLinearCurve(config->tpsTspCorrValues, 1, 1);

	// Flex transient AE table = 2.0 everywhere
	setLinearCurve(config->flexTransientCltBins, -40, 100, 1);
	setLinearCurve(config->flexTransientEthanolBins, 0, 100, 1);
	setTable(config->flexAeMult, 2.0f);

	engineConfiguration->flexFuelTransientComp = true;
	Sensor::setMockValue(SensorType::Clt, 20);

	initAccelEnrichment();
	engine->rpmCalculator.setRpmValue(600);
	engine->periodicFastCallback();

	auto ae = engine->module<TpsAccelEnrichment>();

	auto triggerAccel = [&]() {
		ae->resetAE();
		feedTpsStep(*ae, 0, 10); // 2000 %/s > 100 %/s -> table value 1.0
		return ae->getTpsEnrichment();
	};

	// No flex sensor -> compensation neutral, enrichment = base 1.0
	Sensor::resetMockValue(SensorType::FuelEthanolPercent);
	EXPECT_NEAR(1.0f, triggerAccel(), EPS4D);

	// E100 with compensation enabled -> enrichment doubles
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 100);
	EXPECT_NEAR(2.0f, triggerAccel(), EPS4D);

	// Disable flag -> back to neutral even with a flex sensor present
	engineConfiguration->flexFuelTransientComp = false;
	EXPECT_NEAR(1.0f, triggerAccel(), EPS4D);
}

// [tag:lua_accel_enrichment] Exercise the Lua binding through the fuel pipeline,
// including unit conversion, expiry, native bypasses and restart behavior.
class LuaAccelEnrichment : public ::testing::Test {
protected:
	EngineTestHelper eth{engine_type_e::TEST_ENGINE};

	void SetUp() override {
		setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
		// Custom boards can resize the lambda table beyond the default 16x16
		// calibration. Give this fuel-pipeline fixture a valid target explicitly.
		setTable(config->lambdaTable, 1.0f);
		EXPECT_CALL(*eth.mockAirmass, getAirmass(testing::_, testing::_))
			.WillRepeatedly(testing::Return(AirmassResult{1.3440001f, 50.0f}));
		engine->rpmCalculator.setRpmValue(1200);
		engineConfiguration->accelEnrichmentMode = AE_MODE_LUA;
		engineConfiguration->isTuningDetectorEnabled = false;
		engineConfiguration->coastingFuelCutEnabled = false;
		engineConfiguration->tpsAccelHoldTime = 0;
		initAccelEnrichment();
	}

	TpsAccelEnrichment& ae() {
		return engine->module<TpsAccelEnrichment>().unmock();
	}

	void publish() {
		testLuaExecString("setAccelEnrichment(0.004)");
	}
};

TEST_F(LuaAccelEnrichment, HasFiniteBaselineFuelMass) {
	const float base = getInjectionMass(1200);
	ASSERT_TRUE(std::isfinite(base));
	EXPECT_GT(base, 0);
}

class LuaAccelEnrichmentZeroLambdaDefaults : public LuaAccelEnrichment {
public:
	LuaAccelEnrichmentZeroLambdaDefaults() {
		// Start with a zero table before SetUp() fills it.
		setTable(config->lambdaTable, 0.0f);
	}
};

TEST_F(LuaAccelEnrichmentZeroLambdaDefaults, FuelBaselineAndLuaCorrection) {
	// SetUp() must produce valid fueling even when the table starts at zero.
	float base = getInjectionMass(1200);
	ASSERT_TRUE(std::isfinite(base));
	EXPECT_GT(base, 0);
	publish();
	// Batch injection applies half of the 0.004 g cycle correction.
	EXPECT_NEAR(base + 0.002f, getInjectionMass(1200), 1e-7f);
}

TEST_F(LuaAccelEnrichment, ScalesCycleMassOnceForEachInjectionMode) {
	const injection_mode_e modes[] = {IM_SEQUENTIAL, IM_BATCH, IM_SIMULTANEOUS, IM_SINGLE_POINT};
	const float multipliers[] = {1, 0.5f, 0.25f, 1};
	ASSERT_EQ(4, engineConfiguration->cylindersCount);
	for (size_t i = 0; i < efi::size(modes); i++) {
		engineConfiguration->injectionMode = modes[i];
		testLuaExecString("setAccelEnrichment(0)");
		float base = getInjectionMass(1200);
		ASSERT_TRUE(std::isfinite(base));
		publish();
		EXPECT_NEAR(base + 0.004f * multipliers[i], getInjectionMass(1200), 1e-7f);
		// It is a held correction, not a one-shot fuel quantity consumed on read.
		EXPECT_NEAR(base + 0.004f * multipliers[i], getInjectionMass(1200), 1e-7f);
		EXPECT_FLOAT_EQ(0.004f, ae().luaAccelFuel);
		EXPECT_FLOAT_EQ(0, engine->engineState.tpsAccelEnrich);
	}
}

TEST_F(LuaAccelEnrichment, ReplacesNativeTpsCorrectionsAndLeavesOtherModesAlone) {
	engineConfiguration->tpsAccelHoldTime = 1;
	engineConfiguration->tpsAccelFractionPeriod = 3;
	engineConfiguration->tpsAccelFractionDivisor = 2;
	setTable(config->tpsTpsAccelTable, 10);
	setArrayValues(config->tpsTspCorrValues, 5.0f);
	engineConfiguration->flexFuelTransientComp = true;
	engineConfiguration->flexSensorPin = Gpio::A0;
	Sensor::setMockValue(SensorType::FuelEthanolPercent, 100);
	setTable(config->flexAeMult, 3);
	ae().isAboveAccelThreshold = true;
	// Ethanol also changes native stoichiometric fueling: measure that baseline
	// after configuring it, independently of the transient correction.
	float base = getInjectionMass(1200);
	ASSERT_TRUE(std::isfinite(base));
	publish();
	EXPECT_NEAR(base + 0.002f, getInjectionMass(1200), 1e-7f); // batch
	EXPECT_FLOAT_EQ(0, ae().getTpsEnrichment());

	for (auto mode : {AE_MODE_MS_ADDER, AE_MODE_PERCENT_ADDER, AE_MODE_PREDICTIVE_MAP}) {
		engineConfiguration->accelEnrichmentMode = mode;
		ae().resetFractionValues();
		ae().resetLua();
		float native = getInjectionMass(1200);
		ASSERT_TRUE(std::isfinite(native));
		publish();
		EXPECT_FLOAT_EQ(native, getInjectionMass(1200));
		EXPECT_FLOAT_EQ(0, ae().luaAccelFuel);
	}
}

TEST_F(LuaAccelEnrichment, TimeoutRefreshAndClosedLoopHoldoff) {
	float base = getInjectionMass(1200);
	ASSERT_TRUE(std::isfinite(base));
	engineConfiguration->noFuelTrimAfterAccelTime = 0.5f;
	engineConfiguration->noFuelTrimAfterDfcoTime = 0;
	engineConfiguration->stft.minAfr = 10;
	engineConfiguration->stft.maxAfr = 18;
	Sensor::setMockValue(SensorType::Lambda1, 1.0f);
	ShortTermFuelTrim stft;
	stft.init(&engineConfiguration->stft);
	advanceTimeUs(3000000);
	EXPECT_EQ(stftEnabled, stft.getLearningState(SensorType::Lambda1));
	publish();
	EXPECT_GT(getInjectionMass(1200), base);
	EXPECT_FLOAT_EQ(0, ae().getTimeSinceAcell());
	EXPECT_EQ(stftDisabledTpsAccel, stft.getLearningState(SensorType::Lambda1));
	advanceTimeUs(199000);
	EXPECT_GT(getInjectionMass(1200), base);
	EXPECT_FLOAT_EQ(0, ae().getTimeSinceAcell());
	advanceTimeUs(2000);
	EXPECT_FLOAT_EQ(base, getInjectionMass(1200));
	EXPECT_FLOAT_EQ(0, ae().luaAccelFuel);
	EXPECT_NEAR(0.002f, ae().getTimeSinceAcell(), 1e-6f);
	EXPECT_EQ(stftDisabledTpsAccel, stft.getLearningState(SensorType::Lambda1));
	publish();
	EXPECT_GT(getInjectionMass(1200), base);
	testLuaExecString("setAccelEnrichment(0)");
	advanceTimeUs(10000);
	EXPECT_FLOAT_EQ(base, getInjectionMass(1200));
	EXPECT_NEAR(0.01f, ae().getTimeSinceAcell(), 1e-6f);
	advanceTimeUs(500000);
	EXPECT_EQ(stftEnabled, stft.getLearningState(SensorType::Lambda1));
}

TEST_F(LuaAccelEnrichment, ResetAndModeChangesClearRequest) {
	float base = getInjectionMass(1200);
	ASSERT_TRUE(std::isfinite(base));
	publish();
	EXPECT_GT(getInjectionMass(1200), base);
	engine->resetLua();
	EXPECT_FLOAT_EQ(base, getInjectionMass(1200));
	publish();
	engine->onEngineStopped();
	EXPECT_FLOAT_EQ(base, getInjectionMass(1200));
	publish();
	ae().resetAE(); // same reset used when the engine stops
	EXPECT_FLOAT_EQ(base, getInjectionMass(1200));
	publish();
	ae().onConfigurationChange(engineConfiguration);
	EXPECT_FLOAT_EQ(base, getInjectionMass(1200));

	publish();
	engine_configuration_s previous = *engineConfiguration;
	engineConfiguration->accelEnrichmentMode = AE_MODE_MS_ADDER;
	ae().onConfigurationChange(&previous);
	previous = *engineConfiguration;
	engineConfiguration->accelEnrichmentMode = AE_MODE_LUA;
	ae().onConfigurationChange(&previous);
	EXPECT_FLOAT_EQ(base, getInjectionMass(1200));
}

TEST_F(LuaAccelEnrichment, RejectsInvalidMass) {
	float base = getInjectionMass(1200);
	ASSERT_TRUE(std::isfinite(base));
	for (const char* script : {"setAccelEnrichment(-0.001)", "setAccelEnrichment(0/0)",
		"setAccelEnrichment(math.huge)", "setAccelEnrichment(-math.huge)"}) {
		publish();
		EXPECT_THROW(testLuaExecString(script), std::logic_error);
		EXPECT_FLOAT_EQ(base, getInjectionMass(1200));
	}
}

TEST_F(LuaAccelEnrichment, SuppressedWhenStoppedCrankingOrDfco) {
	for (float rpm : {0.0f, 200.0f}) {
		engine->rpmCalculator.setRpmValue(rpm);
		float base = getInjectionMass(rpm);
		ASSERT_TRUE(std::isfinite(base));
		publish();
		EXPECT_FLOAT_EQ(base, getInjectionMass(rpm));
		EXPECT_FLOAT_EQ(0, ae().luaAccelFuel);
	}

	engine->rpmCalculator.setRpmValue(2000);
	engineConfiguration->coastingFuelCutEnabled = true;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutClt = 30;
	engineConfiguration->coastingFuelCutMap = 100;
	engineConfiguration->coastingFuelCutVssHigh = 0;
	engineConfiguration->coastingFuelCutVssLow = 0;
	engineConfiguration->dfcoDelay = 0;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	Sensor::setMockValue(SensorType::Clt, 90);
	Sensor::setMockValue(SensorType::Map, 20);
	engine->module<DfcoController>()->update();
	ASSERT_TRUE(engine->module<DfcoController>()->cutFuel());
	publish();
	EXPECT_FLOAT_EQ(0, getInjectionMass(2000));
	EXPECT_FLOAT_EQ(0, ae().luaAccelFuel);
}

TEST_F(LuaAccelEnrichment, TuningDetectorSuspendsCorrection) {
	float base = getInjectionMass(1200);
	ASSERT_TRUE(std::isfinite(base));
	engineConfiguration->isTuningDetectorEnabled = true;
	::testing::NiceMock<MockTsChannel> channel;
	const uint16_t offset = static_cast<uint16_t>(offsetof(persistent_config_s, veTable));
	uint8_t dummy = 0;
	TunerStudio ts;
	ts.handleWriteChunkCommand(&channel, TS_PAGE_SETTINGS, offset, 1, &dummy);
	ASSERT_TRUE(isTuningVeNow());
	publish();
	EXPECT_FLOAT_EQ(base, getInjectionMass(1200));
	EXPECT_FLOAT_EQ(0, ae().luaAccelFuel);
	resetCalibrationTimerForTest();
	EXPECT_GT(getInjectionMass(1200), base);
}

TEST(fuel, PreviousTpsAccelerationClearedOnFastPhaseResync) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	setTable(config->lambdaTable, 1.0f);
	EXPECT_CALL(*eth.mockAirmass, getAirmass(::testing::_, ::testing::_))
		.WillRepeatedly(::testing::Return(AirmassResult{1.3440001f, 50.0f}));
	engine->rpmCalculator.setRpmValue(1200);
	engineConfiguration->accelEnrichmentMode = AE_MODE_PERCENT_ADDER;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->isTuningDetectorEnabled = false;
	engineConfiguration->coastingFuelCutEnabled = false;
	engineConfiguration->tpsAccelHoldTime = 1;
	engineConfiguration->tpsAccelRateThreshold = 100;
	engineConfiguration->tpsAccelFractionPeriod = 1;
	engineConfiguration->tpsAccelFractionDivisor = 1;
	engineConfiguration->flexFuelTransientComp = false;
	setArrayValues(config->tpsTspCorrValues, 1.0f);
	setTable(config->tpsTpsAccelTable, 0.20f);
	initAccelEnrichment();
	auto& ae = engine->module<TpsAccelEnrichment>().unmock();
	const float baseline = getInjectionMass(1200);
	ASSERT_TRUE(std::isfinite(baseline));
	ASSERT_GT(baseline, 0);
	feedTpsStep(ae, 0, 20);
	ASSERT_NEAR(1.20f, getInjectionMass(1200) / baseline, 0.00001f);
	engine->OnTriggerSynchronizationLost();
	ASSERT_FALSE(engine->rpmCalculator.isRunning());
	ASSERT_NEAR(0, ae.getTpsEnrichment(), 0.00001f);
	// Phase sync returns before the next slow callback can reset TPS history.
	// A quick restart before the next slow callback must not replay the old TPS step.
	engine->rpmCalculator.setRpmValue(1200);
	ASSERT_TRUE(engine->rpmCalculator.isRunning());
	EXPECT_NEAR(0, ae.getTpsEnrichment(), 0.00001f);
	EXPECT_NEAR(1, getInjectionMass(1200) / baseline, 0.00001f);
}

TEST(fuel, StoppedTpsResetDoesNotInventAccelerationFromHeldThrottle) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->accelEnrichmentMode = AE_MODE_PERCENT_ADDER;
	engineConfiguration->tpsAccelHoldTime = 1;
	engineConfiguration->tpsAccelRateThreshold = 100;
	engineConfiguration->tpsAccelFractionPeriod = 1;
	engineConfiguration->tpsAccelFractionDivisor = 1;
	setArrayValues(config->tpsTspCorrValues, 1.0f);
	setTable(config->tpsTpsAccelTable, 0.20f);
	initAccelEnrichment();
	auto& ae = engine->module<TpsAccelEnrichment>().unmock();
	Sensor::setMockValue(SensorType::Tps1, 20);
	ASSERT_FALSE(engine->rpmCalculator.isRunning());

	// Reproduce the stopped branch of doPeriodicSlowCallback() on each slow tick for
	// two seconds, with the fast callback (TPS detector) running in between.
	for (int i = 0; i < 20; i++) {
		eth.moveTimeForwardMs(50);
		engine->periodicFastCallback();
		eth.moveTimeForwardMs(50);
		engine->rpmCalculator.onSlowCallback();
		ae.resetAE();
		engine->periodicSlowCallback();
		engine->periodicFastCallback();
	}
	ASSERT_FALSE(ae.isAboveAccelThreshold);
	engine->rpmCalculator.setRpmValue(1200);
	EXPECT_NEAR(0, ae.getTpsEnrichment(), 0.00001f);
}
