// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_central.txt Thu Sep 01 14:10:15 UTC 2022
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
	int hwEventCounters[HW_EVENT_TYPES];
	/**
	 * offset 24
	 */
	int vvtCamCounter = (int)0;
	/**
	 * offset 28
	 */
	scaled_channel<float, 1, 1> mapVvt_MAP_AT_SPECIAL_POINT = (float)0;
	/**
	 * offset 32
	 */
	scaled_channel<float, 1, 1> mapVvt_MAP_AT_DIFF = (float)0;
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
	scaled_channel<float, 1, 1> currentEngineDecodedPhase = (float)0;
};
static_assert(sizeof(trigger_central_s) == 44);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_central.txt Thu Sep 01 14:10:15 UTC 2022
