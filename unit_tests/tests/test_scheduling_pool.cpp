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
