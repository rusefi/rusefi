// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/engine_cycle/high_pressure_fuel_pump.txt Fri Dec 31 03:07:40 EST 2021
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of high_pressure_fuel_pump_s
struct high_pressure_fuel_pump_s {
	/**
	 * offset 0
	 */
	angle_t m_requested_pump = (angle_t)0;
	/**
	 * offset 4
	 */
	float fuel_requested_percent = (float)0;
	/**
	 * offset 8
	 */
	float fuel_requested_percent_pi = (float)0;
	/** total size 12*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/engine_cycle/high_pressure_fuel_pump.txt Fri Dec 31 03:07:40 EST 2021
