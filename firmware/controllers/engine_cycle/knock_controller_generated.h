// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt Tue Nov 01 20:59:53 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of knock_controller_s
struct knock_controller_s {
	/**
	 * Knock Degrees retarded
	 * larger number = more retard
	 * offset 0
	 */
	angle_t m_knockRetard = (angle_t)0;
	/**
	 * offset 4
	 */
	float m_knockThreshold = (float)0;
	/**
	 * offset 8
	 */
	uint32_t m_knockCount = (uint32_t)0;
	/**
	 * offset 12
	 */
	float m_maximumRetard = (float)0;
};
static_assert(sizeof(knock_controller_s) == 16);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt Tue Nov 01 20:59:53 UTC 2022
