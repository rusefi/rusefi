#pragma once
#include "rusefi_types.h"
struct trigger_state_s {
	/**
	 * Crank sync counter
	 * Usually matches crank revolutions
	 * offset 0
	 */
	uint32_t crankSynchronizationCounter = (uint32_t)0;
	/**
	 * offset 4
	 */
	float vvtSyncGapRatio = (float)0;
	/**
	 * offset 8
	 */
	float vvtCurrentPosition = (float)0;
	/**
	 * @@GAUGE_NAME_TRG_GAP@@
	 * offset 12
	 */
	float triggerSyncGapRatio = (float)0;
	/**
	 * offset 16
	 */
	uint8_t triggerStateIndex = (uint8_t)0;
	/**
	 * offset 17
	 */
	uint8_t vvtCounter = (uint8_t)0;
	/**
	 * offset 18
	 */
	uint8_t camResyncCounter = (uint8_t)0;
	/**
	 * offset 19
	 */
	uint8_t vvtStateIndex = (uint8_t)0;
};
static_assert(sizeof(trigger_state_s) == 20);

