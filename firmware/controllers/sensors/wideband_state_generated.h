#pragma once
#include "rusefi_types.h"
struct wideband_state_s {
	/**
	 * offset 0
	 */
	uint8_t faultCode = (uint8_t)0;
	/**
	%
	 * offset 1
	 */
	uint8_t heaterDuty = (uint8_t)0;
	/**
	%
	 * offset 2
	 */
	uint8_t pumpDuty = (uint8_t)0;
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

