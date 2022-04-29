// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt Fri Apr 29 16:01:47 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of knock_controller_s
struct knock_controller_s {
	/**
	 * Degrees retarded: larger number = more retard
	 * offset 0
	 */
	angle_t m_requested_pump = (angle_t)0;
};
static_assert(sizeof(knock_controller_s) == 4);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt Fri Apr 29 16:01:47 UTC 2022
