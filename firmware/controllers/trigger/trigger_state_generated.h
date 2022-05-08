// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_state.txt Sun May 08 12:02:50 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of trigger_state_s
struct trigger_state_s {
	/**
	 * offset 0
	 */
	float triggerSyncGapRatio = (float)0;
	/**
	 * Crank revolution counter
	 * offset 4
	 */
	uint32_t totalRevolutionCounter = (uint32_t)0;
	/**
	 * offset 8
	 */
	scaled_channel<float, 1, 1> vvtSyncGapRatio = (float)0;
	/**
	 * offset 12
	 */
	scaled_channel<float, 1, 1> vvtCurrentPosition = (float)0;
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
	uint8_t vvtSyncCounter = (uint8_t)0;
	/**
	 * offset 19
	 */
	uint8_t vvtStateIndex = (uint8_t)0;
};
static_assert(sizeof(trigger_state_s) == 20);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_state.txt Sun May 08 12:02:50 UTC 2022
