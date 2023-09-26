// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt Tue Sep 26 02:21:02 UTC 2023
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
	 * Knock: Cyl
	dBv
	 * offset 4
	 */
	int8_t m_knockCyl[12];
	/**
	 * @@GAUGE_NAME_KNOCK_RETARD@@
	deg
	 * offset 16
	 */
	angle_t m_knockRetard = (angle_t)0;
	/**
	 * Knock: Threshold
	 * offset 20
	 */
	float m_knockThreshold = (float)0;
	/**
	 * @@GAUGE_NAME_KNOCK_COUNTER@@
	 * offset 24
	 */
	uint32_t m_knockCount = (uint32_t)0;
	/**
	 * Knock: Max retard
	 * offset 28
	 */
	float m_maximumRetard = (float)0;
};
static_assert(sizeof(knock_controller_s) == 32);

// end
// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt Tue Sep 26 02:21:02 UTC 2023
