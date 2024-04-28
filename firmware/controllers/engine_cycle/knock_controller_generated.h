// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt Sun Apr 28 13:42:38 UTC 2024
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of knock_controller_s
struct knock_controller_s {
	/**
	 * @@GAUGE_NAME_KNOCK_LEVEL@@
	 * units: Volts
	 * offset 0
	 */
	float m_knockLevel = (float)0;
	/**
	 * Knock: Cyl
	 * units: dBv
	 * offset 4
	 */
	int8_t m_knockCyl[12];
	/**
	 * @@GAUGE_NAME_KNOCK_RETARD@@
	 * units: deg
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
	/**
	 * @@GAUGE_NAME_KNOCK_FREQ@@
	 * units: Hz
	 * offset 32
	 */
	uint32_t m_knockFrequency = (uint32_t)0;
};
static_assert(sizeof(knock_controller_s) == 36);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/engine_cycle/knock_controller.txt Sun Apr 28 13:42:38 UTC 2024
