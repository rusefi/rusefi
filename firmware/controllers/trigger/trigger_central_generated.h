// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_central.txt Fri Oct 28 14:08:24 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
#define HW_EVENT_TYPES 6
// start of trigger_central_s
struct trigger_central_s {
	/**
	 * Hardware events since boot
	 * offset 0
	 */
	uint32_t hwEventCounters[HW_EVENT_TYPES];
	/**
	 * offset 24
	 */
	uint32_t vvtCamCounter = (uint32_t)0;
	/**
	 * offset 28
	 */
	float mapVvt_MAP_AT_SPECIAL_POINT = (float)0;
	/**
	 * offset 32
	 */
	float mapVvt_MAP_AT_DIFF = (float)0;
	/**
	 * offset 36
	 */
	uint8_t mapVvt_MAP_AT_CYCLE_COUNT = (uint8_t)0;
	/**
	 * offset 37
	 */
	uint8_t mapVvt_map_peak = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 38
	 */
	uint8_t alignmentFill_at_38[2];
	/**
	 * Engine Phase
	deg
	 * offset 40
	 */
	float currentEngineDecodedPhase = (float)0;
	/**
	deg
	 * offset 44
	 */
	float triggerToothAngleError = (float)0;
};
static_assert(sizeof(trigger_central_s) == 48);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_central.txt Fri Oct 28 14:08:24 UTC 2022
