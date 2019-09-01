// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/wall_fuel.txt Sat Aug 31 23:11:37 EDT 2019
// by class com.rusefi.output.CHeaderConsumer
// begin
#ifndef CONTROLLERS_GENERATED_WALL_FUEL_GENERATED_H
#define CONTROLLERS_GENERATED_WALL_FUEL_GENERATED_H
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

#endif
// end
// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/wall_fuel.txt Sat Aug 31 23:11:37 EDT 2019
