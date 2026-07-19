/*
 * @file test_fuelCut.cpp
 *
 * @date Mar 22, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "event_queue.h"

using ::testing::_;

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