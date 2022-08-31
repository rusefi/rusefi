// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_state.txt Sun Aug 21 16:42:01 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of trigger_state_s
struct trigger_state_s {
	/**
	 * Crank revolution counter
	 * offset 0
	 */
	uint32_t totalRevolutionCounter = (uint32_t)0;
	/**
	 * offset 4
	 */
	scaled_channel<float, 1, 1> vvtSyncGapRatio = (float)0;
	/**
	 * offset 8
	 */
	scaled_channel<float, 1, 1> vvtCurrentPosition = (float)0;
	/**
	 * @@GAUGE_NAME_TRG_GAP@@
	 * offset 12
	 */
	scaled_channel<float, 1, 1> triggerSyncGapRatio = (float)0;
	/**
	 * trigger: unused field?
	 * offset 16
	 */
	float triggerActualSyncGapRatio = (float)0;
	/**
	 * offset 20
	 */
	uint8_t triggerStateIndex = (uint8_t)0;
	/**
	 * offset 21
	 */
	uint8_t vvtCounter = (uint8_t)0;
	/**
	 * offset 22
	 */
	uint8_t camResyncCounter = (uint8_t)0;
	/**
	 * offset 23
	 */
	uint8_t vvtStateIndex = (uint8_t)0;
};
static_assert(sizeof(trigger_state_s) == 24);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_state.txt Sun Aug 21 16:42:01 UTC 2022
