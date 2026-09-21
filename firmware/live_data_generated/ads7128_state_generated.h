// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) hw_layer/drivers/adc//ads7128_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of ads7128_state_s
struct ads7128_state_s {
	/**
	 * "ADS7128"
	 * offset 0
	 */
	uint16_t adcRaw[8] = {};
};
static_assert(sizeof(ads7128_state_s) == 16);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) hw_layer/drivers/adc//ads7128_state.txt
