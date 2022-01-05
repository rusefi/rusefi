// this section null
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of trigger_state_s
struct trigger_state_s {
	/**
	 * offset 0
	 */
	float currentGap = (float)0;
	/**
	 * Crank revolution counter
	 * offset 4
	 */
	uint32_t totalRevolutionCounter = (uint32_t)0;
	/** total size 8*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger/trigger_state.txt Wed Jan 05 05:06:33 UTC 2022
