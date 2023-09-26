// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt Tue Sep 26 01:20:38 UTC 2023
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of knock_controller_s
struct knock_controller_s {
	/**
	 * @@GAUGE_NAME_KNOCK_LEVEL@@
	Volts
	 * offset 0
	 */
	float m_knockLevel = (float)0;
	/**
	 * @@GAUGE_NAME_KNOCK_RETARD@@
	deg
	 * offset 4
	 */
	angle_t m_knockRetard = (angle_t)0;
	/**
	 * Knock: Threshold
	 * offset 8
	 */
	float m_knockThreshold = (float)0;
	/**
	 * @@GAUGE_NAME_KNOCK_COUNTER@@
	 * offset 12
	 */
	uint32_t m_knockCount = (uint32_t)0;
	/**
	 * offset 16
	 */
	float m_maximumRetard = (float)0;
};
static_assert(sizeof(knock_controller_s) == 20);

// end
// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt Tue Sep 26 01:20:38 UTC 2023
