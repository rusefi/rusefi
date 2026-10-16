// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/wall_fuel_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of wall_fuel_state_s
struct wall_fuel_state_s {
	/**
	 * @@GAUGE_NAME_FUEL_WALL_CORRECTION@@
	 * Fuel mass correction for wall wetting in the last calculated injection event 0. Positive adds fuel, negative subtracts fuel. May retain its last calculated value while compensation is bypassed.
	 * units: mg
	 * offset 0
	 */
	float wallFuelCorrection = (float)0;
	/**
	 * @@GAUGE_NAME_FUEL_WALL_AMOUNT@@
	 * Estimated fuel mass on the intake port wall for injection event 0, not an all-cylinder total.
	 * units: mg
	 * offset 4
	 */
	float wallFuel = (float)0;
};
static_assert(sizeof(wall_fuel_state_s) == 8);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/wall_fuel_state.txt
