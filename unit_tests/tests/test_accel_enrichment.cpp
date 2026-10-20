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
#include "fuel_math.h"
#include "rusefi_lua.h"
#include "tunerstudio.h"
#include "tunerstudio_impl.h"

TEST(fuel, testTpsAccelEnrichmentMath) {
	printf("====================================================================================== testAccelEnrichment\r\n");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engine->rpmCalculator.setRpmValue(600);
	engine->periodicFastCallback();

	engine->module<TpsAccelEnrichment>()->setLength(4);

	engine->module<TpsAccelEnrichment>()->onNewValue(0);
	ASSERT_EQ( 0,  engine->module<TpsAccelEnrichment>()->getMaxDelta()) << "maxDelta";
	engine->module<TpsAccelEnrichment>()->onNewValue(10);
	ASSERT_EQ( 10,  engine->module<TpsAccelEnrichment>()->getMaxDelta()) << "maxDelta#1";
	engine->module<TpsAccelEnrichment>()->onNewValue(30);
	ASSERT_EQ( 20,  engine->module<TpsAccelEnrichment>()->getMaxDelta()) << "maxDelta#2";

	engine->module<TpsAccelEnrichment>()->onNewValue(0);
	ASSERT_EQ( 20,  engine->module<TpsAccelEnrichment>()->getMaxDelta()) << "maxDelta#3";
	engine->module<TpsAccelEnrichment>()->onNewValue(0);
	ASSERT_EQ( 20,  engine->module<TpsAccelEnrichment>()->getMaxDelta()) << "maxDelta#4";
	engine->module<TpsAccelEnrichment>()->onNewValue(0);
	ASSERT_EQ( 0,  engine->module<TpsAccelEnrichment>()->getMaxDelta()) << "maxDelta#5";
	engine->module<TpsAccelEnrichment>()->onNewValue(0);
	ASSERT_EQ( 0,  engine->module<TpsAccelEnrichment>()->getMaxDelta()) << "maxDelta";
}

TEST(fuel, testTpsAccelEnrichmentScheduling) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	setCrankOperationMode();

	engineConfiguration->tpsAccelEnrichmentThreshold = 5;
	engineConfiguration->tpsAccelLookback = 2;

	eth.setTriggerType(trigger_type_e::TT_HALF_MOON);


	eth.fireTriggerEvents2(/* count */ 4, 25 /* ms */);
	ASSERT_EQ( 1200,  Sensor::getOrZero(SensorType::Rpm)) << "RPM";
	// Establish the TPS baseline after crank synchronization, which clears AE state.
	Sensor::setMockValue(SensorType::Tps1, 0);
	engine->periodicSlowCallback();
	Sensor::setMockValue(SensorType::Tps1, 7);
	engine->periodicSlowCallback();
	int expectedInvocationCounter = 1;
	ASSERT_EQ(expectedInvocationCounter, engine->module<TpsAccelEnrichment>()->onUpdateInvocationCounter);

	Sensor::setMockValue(SensorType::Tps1, 70);
	eth.fireTriggerEvents2(/* count */ 1, 25 /* ms */);

	float expectedAEValue = 7;
	// it does not matter how many times we invoke 'getTpsEnrichment' - state does not change
	for (int i = 0; i < 20; i++) {
		ASSERT_NEAR(expectedAEValue, engine->module<TpsAccelEnrichment>()->getTpsEnrichment(), EPS4D);
	}

	expectedInvocationCounter++;
	ASSERT_EQ(expectedInvocationCounter, engine->module<TpsAccelEnrichment>()->onUpdateInvocationCounter);

	eth.engine.periodicFastCallback();
	eth.engine.periodicFastCallback();
	eth.engine.periodicFastCallback();

	ASSERT_EQ(expectedInvocationCounter, engine->module<TpsAccelEnrichment>()->onUpdateInvocationCounter);
}

static void doFractionalTpsIteration(int period, int divisor, int numCycles, std::vector<floatms_t> &tpsEnrich) {
	// every cycle
	engineConfiguration->tpsAccelFractionPeriod = period;
	// split into 2 portions
	engineConfiguration->tpsAccelFractionDivisor = divisor;

	engine->module<TpsAccelEnrichment>()->resetAE();
	engine->module<TpsAccelEnrichment>()->onNewValue(0);
	for (int i = 0; i < numCycles; i++) {
		engine->module<TpsAccelEnrichment>()->onNewValue(10);
		engine->module<TpsAccelEnrichment>()->onEngineCycleTps();
		tpsEnrich[i] = engine->module<TpsAccelEnrichment>()->getTpsEnrichment();
	}
}

TEST(fuel, testAccelEnrichmentFractionalTps) {
	printf("====================================================================================== testAccelEnrichmentFractionalTps\r\n");

	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// setup
	engineConfiguration->tpsAccelEnrichmentThreshold = 5;
	engineConfiguration->tpsAccelLookback = 2;

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

	engine->module<TpsAccelEnrichment>()->setLength(2);


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

	engineConfiguration->tpsAccelEnrichmentThreshold = 5;
	engineConfiguration->tpsAccelLookback = 2;
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
	ae->setLength(2);

	auto triggerAccel = [&]() {
		ae->resetAE();
		ae->onNewValue(0);
		ae->onNewValue(10); // delta 10 > threshold 5 -> table value 1.0
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
		engineConfiguration->tpsAccelLookback = 0;
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
	engineConfiguration->tpsAccelLookback = 1;
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
	engineConfiguration->tpsAccelLookback = 1;
	engineConfiguration->tpsAccelEnrichmentThreshold = 5;
	engineConfiguration->tpsAccelFractionPeriod = 1;
	engineConfiguration->tpsAccelFractionDivisor = 1;
	engineConfiguration->flexFuelTransientComp = false;
	setArrayValues(config->tpsTspCorrValues, 1.0f);
	setTable(config->tpsTpsAccelTable, 0.20f);
	initAccelEnrichment();
	auto& ae = engine->module<TpsAccelEnrichment>().unmock();
	ae.onNewValue(0);
	const float baseline = getInjectionMass(1200);
	ASSERT_TRUE(std::isfinite(baseline));
	ASSERT_GT(baseline, 0);
	ae.onNewValue(20);
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
	engineConfiguration->tpsAccelLookback = 1;
	engineConfiguration->tpsAccelEnrichmentThreshold = 5;
	engineConfiguration->tpsAccelFractionPeriod = 1;
	engineConfiguration->tpsAccelFractionDivisor = 1;
	setArrayValues(config->tpsTspCorrValues, 1.0f);
	setTable(config->tpsTpsAccelTable, 0.20f);
	initAccelEnrichment();
	auto& ae = engine->module<TpsAccelEnrichment>().unmock();
	Sensor::setMockValue(SensorType::Tps1, 20);
	ASSERT_FALSE(engine->rpmCalculator.isRunning());

	// Reproduce the stopped branch of doPeriodicSlowCallback() followed by
	// Engine::periodicSlowCallback() on each slow tick for two seconds.
	for (int i = 0; i < 20; i++) {
		eth.moveTimeForwardMs(100);
		engine->rpmCalculator.onSlowCallback();
		ae.resetAE();
		engine->periodicSlowCallback();
	}
	ASSERT_FALSE(ae.isAboveAccelThreshold);
	engine->rpmCalculator.setRpmValue(1200);
	EXPECT_NEAR(0, ae.getTpsEnrichment(), 0.00001f);
}
