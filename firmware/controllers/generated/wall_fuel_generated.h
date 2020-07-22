// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on (unknown script) integration/wall_fuel_state.txt Mon Jul 06 06:07:20 UTC 2020
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of wall_fuel_state
struct wall_fuel_state {
	/**
	 * * fuel injection time correction to account for wall wetting effect, for current cycle
	 * offset 0
	 */
	float wallFuelCorrection = (float)0;
	/**
	 * Amount of fuel on the wall, in ms of injector open time, for each injector.
	 * offset 4
	 */
	floatms_t wallFuel = (floatms_t)0;
	/** total size 8*/
};

typedef struct wall_fuel_state wall_fuel_state;

// end
// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on (unknown script) integration/wall_fuel_state.txt Mon Jul 06 06:07:20 UTC 2020
