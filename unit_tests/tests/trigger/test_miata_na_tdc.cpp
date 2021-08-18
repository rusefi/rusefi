#include "pch.h"

#include "trigger_simulator.h"
#include "trigger_emulator_algo.h"

TEST(miata, miata_na_tdc) {
	WITH_ENGINE_TEST_HELPER(MIATA_NA6_MAP);

#define TEST_REVOLUTIONS 6

	TriggerWaveform & shape = engine->triggerCentral.triggerShape;

	TriggerEmulatorHelper emulatorHelper;

	/**
	 * let's feed two more cycles to validate shape definition
	 */
	for (uint32_t i = 0; i <= TEST_REVOLUTIONS * shape.getSize(); i++) {

		int time = getSimulatedEventTime(shape, i);
		eth.setTimeAndInvokeEventsUs(time);

		emulatorHelper.handleEmulatorCallback(shape.getSize(),
				shape.wave,
				i  % shape.getSize() PASS_ENGINE_PARAMETER_SUFFIX);
	}

	ASSERT_EQ(167,  GET_RPM()) << "miata_na_tdc RPM";
	ASSERT_EQ(294000, engine->tdcScheduler[0].momentX % SIMULATION_CYCLE_PERIOD); // let's assert TDC position and sync point
	ASSERT_EQ(294000, engine->tdcScheduler[1].momentX % SIMULATION_CYCLE_PERIOD); // let's assert TDC position and sync point
}
