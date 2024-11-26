// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/sensors/sent_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of sent_state_s
struct sent_state_s {
	/**
	 * "SENT ch0 value0"
	 * units: RAW
	 * offset 0
	 */
	uint16_t value0 = (uint16_t)0;
	/**
	 * "SENT ch0 value1"
	 * units: RAW
	 * offset 2
	 */
	uint16_t value1 = (uint16_t)0;
	/**
	 * "SENT ch0 error rate"
	 * units: % (don't belive me)
	 * offset 4
	 */
	float errorRate = (float)0;
};
static_assert(sizeof(sent_state_s) == 8);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/sensors/sent_state.txt
