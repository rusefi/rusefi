// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/sensors/sent_state.txt Wed Jan 03 20:30:10 UTC 2024
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of sent_state_s
struct sent_state_s {
	/**
	 * "ETB: SENT value0"
	 * units: value
	 * offset 0
	 */
	uint16_t value0 = (uint16_t)0;
	/**
	 * "ETB: SENT value1"
	 * units: value
	 * offset 2
	 */
	uint16_t value1 = (uint16_t)0;
	/**
	 * "ETB: SENT error rate"
	 * units: ratio
	 * offset 4
	 */
	float errorRate = (float)0;
};
static_assert(sizeof(sent_state_s) == 8);

// end
// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/sensors/sent_state.txt Wed Jan 03 20:30:10 UTC 2024
