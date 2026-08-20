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

// hardware-ci bench has NO physical sensors: MAP/CLT/IAT/TPS are all dead and
// fueling runs on failed-sensor fallbacks. Reproduces the on-hardware
// "No slots in scheduling pool, queue size 78, headInUs -2553" observed 2026-08-05:
// fallback fueling produces pulse widths far longer than the engine cycle, so
// injection close events pile up holding pool slots for many cycles
TEST(scheduler, poolHeadroomMinimalPinsDeadSensors) {
	EngineTestHelper eth(engine_type_e::MINIMAL_PINS);

	// bench VBatt divider does read the real supply
	Sensor::setMockValue(SensorType::BatteryVoltage, 12);

	testSpinEngineUntilData spinInfo = { 0, 0, 0 };

	// hardware-ci runs self-stim at 2000 rpm (helper rpm is double actual)
	eth.spin60_2UntilDeg(spinInfo, 2 * 2000, 720 * 20);

	int maxUsed = eth.engine.scheduler.getMaxUsedPoolSlots();
	printf("poolHeadroomMinimalPinsDeadSensors: max pool slots used %d of 64, lastInjection %.1f ms\r\n",
			maxUsed, (double)engine->outputChannels.actualLastInjection);
}

// mechanism check for the on-hardware exhaustion: an injection close event holds its
// pool slot for the whole pulse width, so pulses much longer than the engine cycle
// (untuned defaults + floating bench ADC inputs -> eg freezing-cold CLT enrichment)
// stack open/close pairs from many cycles: 4 cyl * 2 slots * pulse/cycleTime slots in
// flight. At 2000 rpm (60 ms cycle) a ~500 ms pulse needs ~64+ slots -> pool empty
TEST(scheduler, poolExhaustionLongInjectionPulse) {
	EngineTestHelper eth(engine_type_e::MINIMAL_PINS);

	Sensor::setMockValue(SensorType::Clt, 80);
	Sensor::setMockValue(SensorType::Iat, 30);
	Sensor::setMockValue(SensorType::Map, 60);
	Sensor::setMockValue(SensorType::Tps1, 20);
	Sensor::setMockValue(SensorType::BatteryVoltage, 13.5f);

	// tiny injector -> pulse width far beyond one engine cycle
	engineConfiguration->injector.flow = 1;

	testSpinEngineUntilData spinInfo = { 0, 0, 0 };
	eth.spin60_2UntilDeg(spinInfo, 2 * 2000, 720 * 20);

	int maxUsed = eth.engine.scheduler.getMaxUsedPoolSlots();
	printf("poolExhaustionLongInjectionPulse: max pool slots used %d of 64, lastInjection %.1f ms\r\n",
			maxUsed, (double)engine->outputChannels.actualLastInjection);

	// long pulses alone do NOT exhaust the pool: injector duty protection cuts
	// injection entirely well before pulse width reaches the danger zone
	EXPECT_LT(maxUsed, 48);
}

// hardware-ci observation 2026-08-05: "No slots in scheduling pool, queue size 78,
// headInUs -2553" moments after a ~1.2 s settings-burn flash write (which freezes
// the firmware, see the writeconfig note in EventQueue::getNextEventTime) with
// self-stim at 2000 rpm, alongside "C9008: tooth #26 error of -10.2".
// Approximate that on the host: steady spin, one giant inter-tooth gap, resume.
TEST(scheduler, poolHeadroomMinimalPinsTriggerStall) {
	EngineTestHelper eth(engine_type_e::MINIMAL_PINS);

	Sensor::setMockValue(SensorType::Clt, 80);
	Sensor::setMockValue(SensorType::Iat, 30);
	Sensor::setMockValue(SensorType::Map, 60);
	Sensor::setMockValue(SensorType::Tps1, 20);
	Sensor::setMockValue(SensorType::BatteryVoltage, 13.5f);

	testSpinEngineUntilData spinInfo = { 0, 0, 0 };

	// steady 2000 rpm
	eth.spin60_2UntilDeg(spinInfo, 2 * 2000, 720 * 5);
	// firmware frozen for 1.2 s mid-tooth: no edges, then spin resumes
	eth.smartFireTriggerEvents2(1, 1200);
	eth.spin60_2UntilDeg(spinInfo, 2 * 2000, 720 * 15);

	int maxUsed = eth.engine.scheduler.getMaxUsedPoolSlots();
	printf("poolHeadroomMinimalPinsTriggerStall: max pool slots used %d of 64\r\n", maxUsed);
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
