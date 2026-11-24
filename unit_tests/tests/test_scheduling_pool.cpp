/**
 * @file	test_scheduling_pool.cpp
 *
 * Measures EventQueue fixed scheduling pool (64 slots) headroom under the
 * MINIMAL_PINS hardware-ci scenario: inline-4, sequential injection,
 * individual coils, 60-2 trigger self-stimulated at 3000 rpm.
 *
 * Only injection open/close events and knock windows draw from the pool -
 * spark events use scheduling_s members embedded in IgnitionEvent.
 */

#include "pch.h"

TEST(scheduler, poolHeadroomMinimalPins3000Rpm) {
	// MINIMAL_PINS already defaults to a crank-mounted TT_TOOTHED_WHEEL_60_2
	EngineTestHelper eth(engine_type_e::MINIMAL_PINS);

	Sensor::setMockValue(SensorType::Clt, 80);
	Sensor::setMockValue(SensorType::Iat, 30);
	Sensor::setMockValue(SensorType::Map, 60);
	Sensor::setMockValue(SensorType::Tps1, 20);
	Sensor::setMockValue(SensorType::BatteryVoltage, 13.5f);

	testSpinEngineUntilData spinInfo = { 0, 0, 0 };

	// spin60_2UntilDeg tooth timing assumes a cam-mounted wheel (as on TEST_ENGINE);
	// MINIMAL_PINS runs the default crank-mounted 60-2, so ask for double the target rpm
	constexpr int helperRpm = 2 * 3000;

	// get to speed and sync
	eth.spin60_2UntilDeg(spinInfo, helperRpm, 720 * 2);
	ASSERT_NEAR(3000, Sensor::getOrZero(SensorType::Rpm), 20) << "RPM";

	// steady state for a bunch of engine cycles
	eth.spin60_2UntilDeg(spinInfo, helperRpm, 720 * 22);

	int maxUsed = eth.engine.scheduler.getMaxUsedPoolSlots();
	printf("poolHeadroomMinimalPins3000Rpm: max pool slots used %d of 64\r\n", maxUsed);

	EXPECT_GT(maxUsed, 0) << "expected injection events to draw from the pool";
	EXPECT_LT(maxUsed, 48) << "scheduling pool dangerously close to exhaustion";
}

// hardware-ci changes self-stimulation speed instantly ("rpm NNN" console command),
// so also measure the worst case across a cranking -> 3000 rpm step
TEST(scheduler, poolHeadroomMinimalPinsRpmStep) {
	EngineTestHelper eth(engine_type_e::MINIMAL_PINS);

	Sensor::setMockValue(SensorType::Clt, 80);
	Sensor::setMockValue(SensorType::Iat, 30);
	Sensor::setMockValue(SensorType::Map, 60);
	Sensor::setMockValue(SensorType::Tps1, 20);
	Sensor::setMockValue(SensorType::BatteryVoltage, 13.5f);

	testSpinEngineUntilData spinInfo = { 0, 0, 0 };

	// crank slowly (helper rpm is double actual for the crank-mounted wheel)
	eth.spin60_2UntilDeg(spinInfo, 2 * 200, 720 * 3);
	// instant step to 3000 rpm
	eth.spin60_2UntilDeg(spinInfo, 2 * 3000, 720 * 20);

	int maxUsed = eth.engine.scheduler.getMaxUsedPoolSlots();
	printf("poolHeadroomMinimalPinsRpmStep: max pool slots used %d of 64\r\n", maxUsed);

	EXPECT_GT(maxUsed, 0) << "expected injection events to draw from the pool";
	EXPECT_LT(maxUsed, 48) << "scheduling pool dangerously close to exhaustion";
}

static void poolPlaceholder() {
}

static constexpr int schedulingPoolSize = 64;

static int countOpenInjectors() {
	int result = 0;
	for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
		if (enginePins.injectors[i].getLogicValue()) {
			result++;
		}
	}
	return result;
}

// [tag:scheduling_pool_exhaustion] Injection opening and closing each take their own pool slot
// (InjectionEvent::onTriggerTooth, main_trigger_callback.cpp). They are scheduled with
// Scheduler::scheduleTogether: if there is no room for both, the whole pulse is skipped and a
// warning raised. An opening that took the last slot without its closing would leave the
// injector open with nothing to close it.
TEST(scheduler, poolExhaustionSkipsInjectionPulse) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engine->tdcMarkEnabled = false;
	EXPECT_CALL(*eth.mockAirmass, getAirmass(::testing::_, ::testing::_))
		.WillRepeatedly(::testing::Return(AirmassResult{0.1008f, 50.0f}));
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth, IM_SEQUENTIAL);

	// 3000 rpm: the half moon wheel is on the cam, one 40 ms engine cycle is two edges 20 ms apart
	constexpr float edgeMs = 20;
	eth.smartFireTriggerEvents2(/* count */ 20, edgeMs);
	ASSERT_NEAR(3000, Sensor::getOrZero(SensorType::Rpm), 20) << "RPM";
	ASSERT_GT(engine->engineState.injectionDuration, 1) << "pulses long enough to schedule";
	ASSERT_GT(engine->engineState.fuelInjectionCounter, 0u) << "injecting before the pool fills up";

	// wait for a moment between pulses: all slots are free and all injectors closed
	for (int i = 0; i < 1000 && (eth.engine.scheduler.getUsedPoolSlots() != 0 || countOpenInjectors() != 0); i++) {
		eth.moveTimeForwardAndInvokeEventsUs(100);
	}
	ASSERT_EQ(0, eth.engine.scheduler.getUsedPoolSlots());
	ASSERT_EQ(0, countOpenInjectors());

	// something else holds all but one slot for longer than this test runs
	efitick_t farFuture = getTimeNowNt() + US2NT(MS2US(10000));
	for (int i = 0; i < schedulingPoolSize - 1; i++) {
		engine->scheduler.schedule("placeholder", nullptr, farFuture, action_s::make<poolPlaceholder>());
	}
	ASSERT_EQ(schedulingPoolSize - 1, eth.engine.scheduler.getUsedPoolSlots());

	eth.smartFireTriggerEvents2(/* count */ 12, edgeMs);
	EXPECT_NE(nullptr, eth.recentWarnings()->find(ObdCode::CUSTOM_ERR_SCHEDULING_ERROR)) << "skipped pulses are reported";

	// far longer than any pulse: a scheduled closing would have run by now
	eth.moveTimeForwardAndInvokeEventsUs(MS2US(100));

	// only the placeholders are still queued, and the one free slot was never taken
	EXPECT_EQ(schedulingPoolSize - 1, eth.engine.scheduler.size());
	EXPECT_EQ(1, eth.engine.scheduler.getAvailablePoolSlots());

	// every pulse was skipped whole: no fuel, and no injector left open
	EXPECT_EQ(0, countOpenInjectors());
	for (int i = 0; i < 4; i++) {
		EXPECT_FALSE(enginePins.injectors[i].getLogicValue()) << "injector " << i;
		EXPECT_EQ(0, enginePins.injectors[i].getOverlappingCounter()) << "injector " << i;
	}

	// leave the shared pins clean for the next test
	for (int i = 0; i < 4; i++) {
		enginePins.injectors[i].reset();
	}
}

// [tag:scheduling_pool_exhaustion] InjectorOutputPin merges overlapping pulses by counting
// openings and closings. If a closing is lost (injection pulses can no longer lose one, see
// poolExhaustionSkipsInjectionPulse) the stopped-engine safety, EnginePins::stopPins(), drives
// the pin low through InjectorOutputPin::stop(), which also clears the count. With a stale
// count each later opening only moved it from one to two and that injector stayed shut until
// a configuration reset.
TEST(scheduler, injectorWorksAfterStopFollowingLostClose) {
	EngineTestHelper eth(engine_type_e::MINIMAL_PINS);

	InjectorOutputPin& injector = enginePins.injectors[0];
	ASSERT_EQ(0, injector.getOverlappingCounter());

	// an opening whose closing never comes
	injector.open(getTimeNowNt());
	ASSERT_TRUE(injector.getLogicValue());

	// engine stops, the safety turns the stuck injector off
	EXPECT_TRUE(enginePins.stopPins());
	EXPECT_FALSE(injector.getLogicValue());
	EXPECT_EQ(0, injector.getOverlappingCounter());

	// next start: a normal pulse
	injector.open(getTimeNowNt());
	EXPECT_TRUE(injector.getLogicValue());
	EXPECT_EQ(1, injector.getOverlappingCounter());
	injector.close(getTimeNowNt());
	EXPECT_FALSE(injector.getLogicValue());
	EXPECT_EQ(0, injector.getOverlappingCounter());

	// leave the shared pin clean for the next test
	injector.reset();
}
