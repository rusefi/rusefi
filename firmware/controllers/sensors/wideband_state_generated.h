// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/sensors//wideband_state.txt Fri Jun 24 00:17:27 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of wideband_state_s
struct wideband_state_s {
	/**
	 * offset 0
	 */
	uint8_t faultCode = (uint8_t)0;
	/**
	%
	 * offset 1
	 */
	scaled_channel<uint8_t, 1, 1> heaterDuty = (uint8_t)0;
	/**
	%
	 * offset 2
	 */
	scaled_channel<uint8_t, 1, 1> pumpDuty = (uint8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 3
	 */
	uint8_t alignmentFill_at_3[1];
	/**
	C
	 * offset 4
	 */
	uint16_t tempC = (uint16_t)0;
	/**
	V
	 * offset 6
	 */
	scaled_channel<uint16_t, 1000, 1> nernstVoltage = (uint16_t)0;
	/**
	 * offset 8
	 */
	uint16_t esr = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 10
	 */
	uint8_t alignmentFill_at_10[2];
};
static_assert(sizeof(wideband_state_s) == 12);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/sensors//wideband_state.txt Fri Jun 24 00:17:27 UTC 2022
