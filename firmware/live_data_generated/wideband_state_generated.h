// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/sensors//wideband_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of wideband_state_s
struct wideband_state_s {
	/**
	 * WBO: Fault code
	 * offset 0
	 */
	uint8_t faultCode = (uint8_t)0;
	/**
	 * WBO: Heater duty
	 * units: %
	 * offset 1
	 */
	uint8_t heaterDuty = (uint8_t)0;
	/**
	 * WBO: Pump duty
	 * units: %
	 * offset 2
	 */
	uint8_t pumpDuty = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 3
	 */
	uint8_t alignmentFill_at_3[1] = {};
	/**
	 * WBO: Temperature
	 * units: C
	 * offset 4
	 */
	uint16_t tempC = (uint16_t)0;
	/**
	 * WBO: Nernst Voltage
	 * units: V
	 * offset 6
	 */
	scaled_channel<uint16_t, 1000, 1> nernstVoltage = (uint16_t)0;
	/**
	 * WBO: ESR
	 * units: ohm
	 * offset 8
	 */
	uint16_t esr = (uint16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 10
	 */
	uint8_t alignmentFill_at_10[2] = {};
};
static_assert(sizeof(wideband_state_s) == 12);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/sensors//wideband_state.txt
