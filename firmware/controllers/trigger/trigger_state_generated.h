#pragma once
#include "rusefi_types.h"
struct trigger_state_s {
	// Crank sync counter
	// Usually matches crank revolutions
	// offset 0
	uint32_t crankSynchronizationCounter = (uint32_t)0;
	// offset 4
	float vvtSyncGapRatio = (float)0;
	// Trigger Sync Latest Ratio
	// offset 8
	float triggerSyncGapRatio = (float)0;
	// offset 12
	uint8_t triggerStateIndex = (uint8_t)0;
	// offset 13
	uint8_t vvtCounter = (uint8_t)0;
	// offset 14
	uint8_t vvtStateIndex = (uint8_t)0;
	// offset 15
	uint8_t alignmentFill_at_15[1];
};
static_assert(sizeof(trigger_state_s) == 16);
static_assert(offsetof(trigger_state_s, crankSynchronizationCounter) == 0);
static_assert(offsetof(trigger_state_s, vvtSyncGapRatio) == 4);
static_assert(offsetof(trigger_state_s, triggerSyncGapRatio) == 8);
static_assert(offsetof(trigger_state_s, triggerStateIndex) == 12);
static_assert(offsetof(trigger_state_s, vvtCounter) == 13);
static_assert(offsetof(trigger_state_s, vvtStateIndex) == 14);

