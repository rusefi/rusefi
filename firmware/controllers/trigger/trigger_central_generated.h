#pragma once
#include "rusefi_types.h"
struct trigger_central_s {
	// Hardware events since boot
	// offset 0
	uint32_t hwEventCounters[6];
	// offset 24
	uint32_t vvtCamCounter = (uint32_t)0;
	// offset 28
	float mapVvt_MAP_AT_SPECIAL_POINT = (float)0;
	// offset 32
	float mapVvt_MAP_AT_DIFF = (float)0;
	// offset 36
	uint8_t mapVvt_MAP_AT_CYCLE_COUNT = (uint8_t)0;
	// offset 37
	uint8_t mapVvt_map_peak = (uint8_t)0;
	// offset 38
	uint8_t alignmentFill_at_38[2];
	// Engine Phase
	// deg
	// offset 40
	float currentEngineDecodedPhase = (float)0;
	// deg
	// offset 44
	float triggerToothAngleError = (float)0;
	// offset 48
	uint8_t triggerIgnoredToothCount = (uint8_t)0;
	// offset 49
	uint8_t alignmentFill_at_49[3];
};
static_assert(sizeof(trigger_central_s) == 52);
static_assert(offsetof(trigger_central_s, hwEventCounters) == 0);
static_assert(offsetof(trigger_central_s, vvtCamCounter) == 24);
static_assert(offsetof(trigger_central_s, mapVvt_MAP_AT_SPECIAL_POINT) == 28);
static_assert(offsetof(trigger_central_s, mapVvt_MAP_AT_DIFF) == 32);
static_assert(offsetof(trigger_central_s, mapVvt_MAP_AT_CYCLE_COUNT) == 36);
static_assert(offsetof(trigger_central_s, mapVvt_map_peak) == 37);
static_assert(offsetof(trigger_central_s, currentEngineDecodedPhase) == 40);
static_assert(offsetof(trigger_central_s, triggerToothAngleError) == 44);
static_assert(offsetof(trigger_central_s, triggerIgnoredToothCount) == 48);

