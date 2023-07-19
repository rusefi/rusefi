#pragma once
#include "rusefi_types.h"
struct wall_fuel_state_s {
	// fuel wallwetting injection time
	//  correction to account for wall wetting effect for current cycle
	// offset 0
	float wallFuelCorrection = (float)0;
	// Fuel on the wall
	// in ms of injector open time for each injector.
	// offset 4
	floatms_t wallFuel = (floatms_t)0;
};
static_assert(sizeof(wall_fuel_state_s) == 8);

