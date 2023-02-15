// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/sensors//sent_state.txt Wed Feb 15 00:55:51 UTC 2023
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of sent_state_s
struct sent_state_s {
	/**
	 * offset 0
	 */
	uint16_t value1 = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 2
	 */
	uint8_t alignmentFill_at_2[2];
	/**
	 * "ETB: SENT error rate"
	ratio
	 * offset 4
	 */
	float errorRate = (float)0;
};
static_assert(sizeof(sent_state_s) == 8);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/sensors//sent_state.txt Wed Feb 15 00:55:51 UTC 2023
