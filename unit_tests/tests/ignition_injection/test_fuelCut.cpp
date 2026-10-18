/*
 * @file test_fuelCut.cpp
 *
 * @date Mar 22, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "event_queue.h"
#include "engine_state_machine.h"
#include "tunerstudio.h"

using ::testing::_;

static void setupStateAwareDfco() {
	engineConfiguration->coastingFuelCutEnabled = true;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutVssLow = 0;
	engineConfiguration->coastingFuelCutVssHigh = 0;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutClt = 30;
	engineConfiguration->coastingFuelCutMap = 100;
	engineConfiguration->dfcoDelay = 0;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	Sensor::setMockValue(SensorType::Clt, 90);
	Sensor::setMockValue(SensorType::Map, 30);
	engine->rpmCalculator.setRpmValue(3000);
}

static bool updateDfco() {
	engine->module<DfcoController>()->update();
	return engine->module<DfcoController>()->cutFuel();
}

// [tag:state_aware_dfco] Mode/gear zero defaults keep old tunes independent of the SM.
TEST(StateAwareDfco, DefaultsAndGearValidity) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupStateAwareDfco();
	EXPECT_EQ(engineConfiguration->dfcoFuelCutMode, dfco_fuel_cut_mode_e::Overrun);
	EXPECT_FALSE(engineConfiguration->coastingFuelCutRequiresGear);
	EXPECT_FALSE(engineConfiguration->useEngineStateMachine);
	Sensor::setInvalidMockValue(SensorType::DetectedGear);
	EXPECT_TRUE(updateDfco());

	engineConfiguration->coastingFuelCutRequiresGear = true;
	EXPECT_FALSE(updateDfco());
	for (float gear : {0.0f, -1.0f, 1.0f, 5.0f, 0.0f}) {
		Sensor::setMockValue(SensorType::DetectedGear, gear);
		EXPECT_EQ(updateDfco(), gear > 0);
	}
	Sensor::setInvalidMockValue(SensorType::DetectedGear);
	EXPECT_FALSE(updateDfco());
	engineConfiguration->coastingFuelCutRequiresGear = false;
	EXPECT_TRUE(updateDfco());
}

TEST(StateAwareDfco, ModeSelectionAndDisabledStateMachine) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupStateAwareDfco();
	auto& sm = engine->module<EngineStateMachine>().unmock();
	for (auto mode : {dfco_fuel_cut_mode_e::Overrun, dfco_fuel_cut_mode_e::Decel, dfco_fuel_cut_mode_e::Both}) {
		engineConfiguration->dfcoFuelCutMode = mode;
		for (bool physicalOverrun : {false, true}) {
			Sensor::setMockValue(SensorType::DriverThrottleIntent, physicalOverrun ? 0 : 50);
			for (bool decel : {false, true}) {
				sm.engineSmIsDecelerating = decel;
				engineConfiguration->useEngineStateMachine = true;
				EXPECT_EQ(updateDfco(), (physicalOverrun && mode != dfco_fuel_cut_mode_e::Decel)
					|| (decel && mode != dfco_fuel_cut_mode_e::Overrun));
				// Disabling takes effect before the slow callback clears stale SM flags.
				engineConfiguration->useEngineStateMachine = false;
				EXPECT_EQ(updateDfco(), physicalOverrun && mode != dfco_fuel_cut_mode_e::Decel);
			}
		}
	}
	engineConfiguration->dfcoFuelCutMode = static_cast<dfco_fuel_cut_mode_e>(3);
	EXPECT_FALSE(updateDfco());
}

TEST(StateAwareDfco, CommonGuardsAndMapHysteresisInEveryMode) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupStateAwareDfco();
	engineConfiguration->useEngineStateMachine = true;
	engine->module<EngineStateMachine>()->engineSmIsDecelerating = true;
	for (auto mode : {dfco_fuel_cut_mode_e::Overrun, dfco_fuel_cut_mode_e::Decel, dfco_fuel_cut_mode_e::Both}) {
		engineConfiguration->dfcoFuelCutMode = mode;
		EXPECT_TRUE(updateDfco());
		engineConfiguration->coastingFuelCutEnabled = false;
		EXPECT_FALSE(updateDfco());
		engineConfiguration->coastingFuelCutEnabled = true;
		engine->engineState.lua.disableDecelerationFuelCutOff = true;
		EXPECT_FALSE(updateDfco());
		engine->engineState.lua.disableDecelerationFuelCutOff = false;
		Sensor::setInvalidMockValue(SensorType::Clt);
		EXPECT_FALSE(updateDfco());
		Sensor::setMockValue(SensorType::Clt, 30);
		EXPECT_FALSE(updateDfco());
		Sensor::setMockValue(SensorType::Clt, 90);
		EXPECT_TRUE(updateDfco());
		Sensor::setInvalidMockValue(SensorType::Map);
		EXPECT_FALSE(updateDfco());
		Sensor::setMockValue(SensorType::Map, 102);
		EXPECT_FALSE(updateDfco());
		Sensor::setMockValue(SensorType::Map, 100);
		EXPECT_FALSE(updateDfco());
		Sensor::setMockValue(SensorType::Map, 98);
		EXPECT_TRUE(updateDfco());
		Sensor::setMockValue(SensorType::Map, 100);
		EXPECT_TRUE(updateDfco());
		Sensor::resetMockValue(SensorType::Map);
		ASSERT_FALSE(Sensor::hasSensor(SensorType::Map));
		EXPECT_TRUE(updateDfco()); // MAP is optional.

		engineConfiguration->isTuningDetectorEnabled = true;
		testing::NiceMock<MockTsChannel> channel;
		uint8_t dummy = 0;
		TunerStudio ts;
		ts.handleWriteChunkCommand(&channel, TS_PAGE_SETTINGS,
			static_cast<uint16_t>(offsetof(persistent_config_s, veTable)), 1, &dummy);
		EXPECT_FALSE(updateDfco());
		resetCalibrationTimerForTest();
		EXPECT_TRUE(updateDfco());
	}
}

static void sampleDfcoRpm(float rpm) {
	advanceTimeUs(100000);
	engine->rpmCalculator.setRpmValue(rpm);
	engine->module<EngineStateMachine>()->onSlowCallback();
}

TEST(StateAwareDfco, DecelUsesClassificationAndAbsentSpeedFallback) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupStateAwareDfco();
	engineConfiguration->useEngineStateMachine = true;
	engineConfiguration->dfcoFuelCutMode = dfco_fuel_cut_mode_e::Decel;
	engineConfiguration->smTransientHoldTimeMs = 0;
	engineConfiguration->smDecelRateThreshold = 500;
	engineConfiguration->smRpmRateWindowMs = 100;
	// These overrun gates must not restrict Decel mode.
	engineConfiguration->coastingFuelCutRpmLow = 4500;
	engineConfiguration->coastingFuelCutRpmHigh = 5000;
	engineConfiguration->coastingFuelCutVssLow = 80;
	engineConfiguration->coastingFuelCutVssHigh = 100;
	engineConfiguration->coastingFuelCutRequiresGear = true;
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 50);
	Sensor::setMockValue(SensorType::DetectedGear, 0);
	Sensor::setMockValue(SensorType::VehicleSpeed, 40);
	MockIdleController idle;
	engine->module<IdleController>().set(&idle);
	ON_CALL(idle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Running));
	auto& sm = engine->module<EngineStateMachine>().unmock();
	sampleDfcoRpm(4000);
	sampleDfcoRpm(3900);
	sampleDfcoRpm(3800);
	ASSERT_TRUE(sm.engineSmIsDecelerating);
	EXPECT_TRUE(updateDfco());

	Sensor::setMockValue(SensorType::DetectedGear, 2);
	sampleDfcoRpm(3700);
	ASSERT_FALSE(sm.engineSmIsDecelerating);
	EXPECT_FALSE(updateDfco()); // Valid speed and engaged gear: no Decel.
	Sensor::setInvalidMockValue(SensorType::DetectedGear);
	sampleDfcoRpm(3600);
	EXPECT_FALSE(updateDfco());
	Sensor::setMockValue(SensorType::DetectedGear, 0);
	Sensor::setInvalidMockValue(SensorType::VehicleSpeed);
	sampleDfcoRpm(3500);
	ASSERT_LT(sm.getLastRpmRate(), -500);
	ASSERT_FALSE(sm.engineSmIsDecelerating);
	EXPECT_FALSE(updateDfco()); // Failed configured VSS is not absent VSS.

	Sensor::resetMockValue(SensorType::VehicleSpeed);
	ASSERT_FALSE(Sensor::hasSensor(SensorType::VehicleSpeed));
	sampleDfcoRpm(3400);
	ASSERT_FALSE(sm.engineSmIsDecelerating);
	EXPECT_TRUE(updateDfco()); // Raw RPM rate fallback, with no state reclassification.
	engineConfiguration->smDecelRateThreshold = 0;
	EXPECT_FALSE(updateDfco());
	engineConfiguration->smDecelRateThreshold = 500;
	sampleDfcoRpm(3350);
	EXPECT_FLOAT_EQ(sm.getLastRpmRate(), -500);
	EXPECT_FALSE(updateDfco()); // Must be strictly faster than the threshold.
	sampleDfcoRpm(3250);
	EXPECT_TRUE(updateDfco());
	engineConfiguration->useEngineStateMachine = false;
	EXPECT_FALSE(updateDfco()); // Even before the next SM update.
}

TEST(StateAwareDfco, BothShiftDirectionsSuppressCutAndRestartDelay) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupStateAwareDfco();
	engineConfiguration->useEngineStateMachine = true;
	engineConfiguration->dfcoDelay = 0.5f;
	engineConfiguration->smTransientHoldTimeMs = 0;
	auto& sm = engine->module<EngineStateMachine>().unmock();
	for (bool upshift : {true, false}) {
		engineConfiguration->smUpshiftClutchSwitch = upshift ? sm_clutch_switch_e::ClutchDown : sm_clutch_switch_e::None;
		engineConfiguration->smDownshiftClutchSwitch = upshift ? sm_clutch_switch_e::None : sm_clutch_switch_e::ClutchDown;
		sm.onConfigurationChange(nullptr);
		engine->engineState.clutchDownState = false;
		sampleDfcoRpm(3000);
		updateDfco();
		advanceTimeUs(600000);
		ASSERT_TRUE(updateDfco());
		engine->engineState.clutchDownState = true;
		sampleDfcoRpm(3000);
		ASSERT_EQ(sm.engineSmIsUpshifting, upshift);
		ASSERT_EQ(sm.engineSmIsDownshifting, !upshift);
		EXPECT_FALSE(updateDfco());
		advanceTimeUs(600000);
		EXPECT_FALSE(updateDfco());
		engine->engineState.clutchDownState = false;
		sampleDfcoRpm(3000);
		ASSERT_FALSE(sm.engineSmIsUpshifting || sm.engineSmIsDownshifting);
		EXPECT_FALSE(updateDfco());
		advanceTimeUs(300000);
		EXPECT_FALSE(updateDfco());
		advanceTimeUs(200000);
		EXPECT_TRUE(updateDfco());
	}
}

TEST(StateAwareDfco, FuelCutGuardsDoNotChangePhysicalOverrunClassification) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	setupStateAwareDfco();
	engineConfiguration->useEngineStateMachine = true;
	engineConfiguration->smTransientHoldTimeMs = 0;
	engineConfiguration->coastingFuelCutEnabled = false;
	Sensor::setMockValue(SensorType::Clt, 0);
	Sensor::setInvalidMockValue(SensorType::Map);
	MockIdleController idle;
	engine->module<IdleController>().set(&idle);
	ON_CALL(idle, getCurrentPhase()).WillByDefault(Return(IIdleController::Phase::Coasting));
	sampleDfcoRpm(3000);
	EXPECT_TRUE(engine->module<EngineStateMachine>()->engineSmIsOverrun);
	EXPECT_FALSE(updateDfco());
}

// Define some helpers for not-cut and cut
#define EXPECT_NORMAL() EXPECT_FLOAT_EQ(normalInjDuration, engine->engineState.injectionDuration)
#define EXPECT_CUT() EXPECT_FLOAT_EQ(0, engine->engineState.injectionDuration)

#if FUEL_RPM_COUNT == 16
TEST(fuelCut, coasting) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));

	// configure coastingFuelCut
	engineConfiguration->coastingFuelCutEnabled = true;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutClt = 30;
	engineConfiguration->coastingFuelCutMap = 100;
	// set cranking threshold
	engineConfiguration->cranking.rpm = 999;

	// basic engine setup
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

	Sensor::setMockValue(SensorType::Map, 0);

	// mock CLT - just above threshold ('hot engine')
	float hotClt = engineConfiguration->coastingFuelCutClt + 1;
	Sensor::setMockValue(SensorType::Clt, hotClt);
	// mock TPS - throttle is opened
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 60);
	// set 'running' RPM - just above RpmHigh threshold
	Sensor::setMockValue(SensorType::Rpm, engineConfiguration->coastingFuelCutRpmHigh + 1);
	// 'advance' time (amount doesn't matter)
	eth.moveTimeForwardUs(1000);

	const float normalInjDuration = 1.5f;
	/*
	 * We need to pass through all rpm changes (high-mid-low-mid-high) because of state-machine
	 */

	// process
	eth.engine.periodicFastCallback();

	// this is normal injection mode (the throttle is opened), no fuel cut-off
	EXPECT_NORMAL();

	// 'releasing' the throttle
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	eth.engine.periodicFastCallback();

	// Fuel cut-off is enabled now
	EXPECT_CUT();

	// Now drop the CLT below threshold
	Sensor::setMockValue(SensorType::Clt, engineConfiguration->coastingFuelCutClt - 1);
	eth.engine.periodicFastCallback();

	// Fuel cut-off should be diactivated - the engine is 'cold'
	EXPECT_NORMAL();

	// restore CLT
	Sensor::setMockValue(SensorType::Clt, hotClt);
	// And set RPM - somewhere between RpmHigh and RpmLow threshold
	Sensor::setMockValue(SensorType::Rpm, (engineConfiguration->coastingFuelCutRpmHigh + engineConfiguration->coastingFuelCutRpmLow) / 2);
	eth.engine.periodicFastCallback();

	// Fuel cut-off is enabled - nothing should change
	EXPECT_NORMAL();

	// Now drop RPM just below RpmLow threshold
	Sensor::setMockValue(SensorType::Rpm, engineConfiguration->coastingFuelCutRpmLow - 1);
	eth.engine.periodicFastCallback();

	// Fuel cut-off is now disabled (the engine is idling)
	EXPECT_NORMAL();

	// Now change RPM just below RpmHigh threshold
	Sensor::setMockValue(SensorType::Rpm, engineConfiguration->coastingFuelCutRpmHigh - 1);
	eth.engine.periodicFastCallback();

	// Fuel cut-off is still disabled
	EXPECT_NORMAL();

	// Now set RPM just above RpmHigh threshold
	Sensor::setMockValue(SensorType::Rpm, engineConfiguration->coastingFuelCutRpmHigh + 1);
	eth.engine.periodicFastCallback();

	// Fuel cut-off is active again!
	EXPECT_CUT();

	// Configure vehicle speed thresholds
	engineConfiguration->coastingFuelCutVssHigh = 50;
	engineConfiguration->coastingFuelCutVssLow = 40;

	// High speed, should still be cut.
	Sensor::setMockValue(SensorType::VehicleSpeed, 55);
	eth.engine.periodicFastCallback();
	EXPECT_CUT();

	// Between thresholds, still cut.
	Sensor::setMockValue(SensorType::VehicleSpeed, 45);
	eth.engine.periodicFastCallback();
	EXPECT_CUT();

	// Below lower threshold, normal fuel resumes
	Sensor::setMockValue(SensorType::VehicleSpeed, 35);
	eth.engine.periodicFastCallback();
	EXPECT_NORMAL();

	// Between thresholds, still normal.
	Sensor::setMockValue(SensorType::VehicleSpeed, 45);
	eth.engine.periodicFastCallback();
	EXPECT_NORMAL();

	// Back above upper, cut again.
	Sensor::setMockValue(SensorType::VehicleSpeed, 55);
	eth.engine.periodicFastCallback();
	EXPECT_CUT();
}
#endif //FUEL_RPM_COUNT == 16

#if FUEL_RPM_COUNT == 16
TEST(fuelCut, delay) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));

	// configure coastingFuelCut
	engineConfiguration->coastingFuelCutEnabled = true;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutClt = 30;
	engineConfiguration->coastingFuelCutMap = 100;
	// set cranking threshold
	engineConfiguration->cranking.rpm = 999;

	// delay is 1 second
	engineConfiguration->dfcoDelay = 1.0f;

	// basic engine setup
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

	Sensor::setMockValue(SensorType::Map, 0);

	// mock CLT - just above threshold ('hot engine')
	float hotClt = engineConfiguration->coastingFuelCutClt + 1;
	Sensor::setMockValue(SensorType::Clt, hotClt);
	// mock TPS - throttle is opened
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 60);
	// set 'running' RPM - just above RpmHigh threshold
	Sensor::setMockValue(SensorType::Rpm, engineConfiguration->coastingFuelCutRpmHigh + 1);
	// 'advance' time (amount doesn't matter)
	eth.moveTimeForwardUs(1000);

	const float normalInjDuration = 1.5f;

	setTimeNowUs(1e6);

	// process
	eth.engine.periodicFastCallback();

	// this is normal injection mode (the throttle is opened), no fuel cut-off
	EXPECT_NORMAL();

	// 'releasing' the throttle
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	eth.engine.periodicFastCallback();

	// Shouldn't cut yet, since not enough time has elapsed
	EXPECT_NORMAL();

	// Change nothing else, but advance time and update again
	advanceTimeUs(0.9e6);
	eth.engine.periodicFastCallback();

	// too soon, still no cut
	EXPECT_NORMAL();

	// Change nothing else, but advance time and update again
	advanceTimeUs(0.2e6);
	eth.engine.periodicFastCallback();

	// Should now be cut!
	EXPECT_CUT();

	// Put the throtle back, and it should come back instantly
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 30);
	eth.engine.periodicFastCallback();
	EXPECT_NORMAL();
}
#endif //FUEL_RPM_COUNT == 16

#if FUEL_RPM_COUNT == 16
// While deceleration fuel cut-off is active the commanded fuel mass is zero, so the
// injection duration computed at injection scheduling time is zero as well.
// Zero duration is an intentional "no injection" and must NOT be reported as an
// "impossibly short" pulse - regression test for false-positive
// CUSTOM_OBD_impossibly_short_INJECTION spam during coasting, see #9874.
TEST(fuelCut, dfcoDoesNotWarnImpossiblyShortInjection) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->tdcMarkEnabled = false;
	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));

	// configure coastingFuelCut, same thresholds as the 'coasting' test above
	engineConfiguration->coastingFuelCutEnabled = true;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutClt = 30;
	engineConfiguration->coastingFuelCutMap = 100;
	// set cranking threshold
	engineConfiguration->cranking.rpm = 999;

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth, IM_SEQUENTIAL);

	Sensor::setMockValue(SensorType::Map, 0);
	// mock CLT - just above threshold ('hot engine')
	Sensor::setMockValue(SensorType::Clt, engineConfiguration->coastingFuelCutClt + 1);
	// mock TPS - the driver is on the throttle
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 60);

	// spin the engine up to 3000 rpm - above coastingFuelCutRpmHigh
	eth.fireTriggerEventsWithDuration(20);
	eth.fireTriggerEventsWithDuration(20);
	ASSERT_EQ(3000, Sensor::getOrZero(SensorType::Rpm));

	// run a few normal engine cycles: fuel is injected, ignore any spin-up noise
	eth.smartFireTriggerEvents2(/*count*/ 4, /*durationMs*/ 20);
	EXPECT_GT(engine->engineState.injectionDuration, 0);
	eth.recentWarnings()->clear();

	// a few more normal cycles - a running engine does not produce short-pulse warnings
	eth.smartFireTriggerEvents2(/*count*/ 4, /*durationMs*/ 20);
	EXPECT_FALSE(hasRecentWarningCode(ObdCode::CUSTOM_OBD_impossibly_short_INJECTION));

	// now lift off the throttle - deceleration fuel cut-off engages
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	eth.engine.periodicFastCallback();
	ASSERT_TRUE(engine->module<DfcoController>()->cutFuel());
	EXPECT_FLOAT_EQ(0, engine->engineState.injectionDuration);

	// keep coasting: injection events run with zero duration, which is an intentional
	// "no injection" and must not be reported as an impossibly short pulse
	eth.smartFireTriggerEvents2(/*count*/ 4, /*durationMs*/ 20);
	EXPECT_FALSE(hasRecentWarningCode(ObdCode::CUSTOM_OBD_impossibly_short_INJECTION));
	EXPECT_EQ(0u, eth.recentWarnings()->getCount());
}

// The warning itself must survive for genuinely short nonzero pulses: with a tiny
// commanded fuel mass and no deadtime the duration lands in (0, 50us) and the
// scheduler-safety warning from PR #596 still fires.
TEST(fuelCut, nonZeroImpossiblyShortInjectionStillWarns) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->tdcMarkEnabled = false;
	// tiny but nonzero airmass -> tiny but nonzero fuel mass
	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{0.0000001f, 50.0f}));

	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth, IM_SEQUENTIAL);

	// no deadtime, so duration is not pushed above the 50us threshold
	setTable(engineConfiguration->injector.battLagCorrTable, 0.0f);

	eth.fireTriggerEventsWithDuration(20);
	eth.fireTriggerEventsWithDuration(20);
	ASSERT_EQ(3000, Sensor::getOrZero(SensorType::Rpm));
	eth.recentWarnings()->clear();

	eth.smartFireTriggerEvents2(/*count*/ 4, /*durationMs*/ 20);

	// commanded duration is nonzero but under 50us - the warning must fire
	ASSERT_GT(engine->engineState.injectionDuration, 0);
	EXPECT_TRUE(hasRecentWarningCode(ObdCode::CUSTOM_OBD_impossibly_short_INJECTION));
}
#endif //FUEL_RPM_COUNT == 16

#if FUEL_RPM_COUNT == 16
TEST(fuelCut, requiresGear) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	EXPECT_CALL(*eth.mockAirmass, getAirmass(_, _))
		.WillRepeatedly(Return(AirmassResult{0.1008f, 50.0f}));

	// configure coastingFuelCut, gated on being in a valid gear
	engineConfiguration->coastingFuelCutEnabled = true;
	engineConfiguration->coastingFuelCutRequiresGear = true;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutClt = 30;
	engineConfiguration->coastingFuelCutMap = 100;
	// set cranking threshold
	engineConfiguration->cranking.rpm = 999;

	// basic engine setup
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);

	Sensor::setMockValue(SensorType::Map, 0);

	// mock CLT - just above threshold ('hot engine')
	float hotClt = engineConfiguration->coastingFuelCutClt + 1;
	Sensor::setMockValue(SensorType::Clt, hotClt);
	// mock TPS - throttle is released, RPM in cut range
	Sensor::setMockValue(SensorType::DriverThrottleIntent, 0);
	Sensor::setMockValue(SensorType::Rpm, engineConfiguration->coastingFuelCutRpmHigh + 1);
	eth.moveTimeForwardUs(1000);

	const float normalInjDuration = 1.5f;

	// Neutral (DetectedGear == 0): all other DFCO conditions met, but no cut while in neutral
	Sensor::setMockValue(SensorType::DetectedGear, 0);
	eth.engine.periodicFastCallback();
	EXPECT_NORMAL();

	// Shift into 1st gear: cut should now engage
	Sensor::setMockValue(SensorType::DetectedGear, 1);
	eth.engine.periodicFastCallback();
	EXPECT_CUT();

	// Back to neutral: cut should disengage immediately
	Sensor::setMockValue(SensorType::DetectedGear, 0);
	eth.engine.periodicFastCallback();
	EXPECT_NORMAL();
}

#endif
