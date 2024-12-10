// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/dc_motors.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of dc_motors_s
struct dc_motors_s {
	/**
	 * "DC: output0"
	 * units: %
	 * offset 0
	 */
	float dcOutput0 = (float)0;
	/**
	 * "DC: en0"
	 * units: %
	 * offset 4
	 */
	uint8_t isEnabled0_int = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[3];
	/**
	offset 8 bit 0 */
	bool isEnabled0 : 1 {};
	/**
	offset 8 bit 1 */
	bool unusedBit_4_1 : 1 {};
	/**
	offset 8 bit 2 */
	bool unusedBit_4_2 : 1 {};
	/**
	offset 8 bit 3 */
	bool unusedBit_4_3 : 1 {};
	/**
	offset 8 bit 4 */
	bool unusedBit_4_4 : 1 {};
	/**
	offset 8 bit 5 */
	bool unusedBit_4_5 : 1 {};
	/**
	offset 8 bit 6 */
	bool unusedBit_4_6 : 1 {};
	/**
	offset 8 bit 7 */
	bool unusedBit_4_7 : 1 {};
	/**
	offset 8 bit 8 */
	bool unusedBit_4_8 : 1 {};
	/**
	offset 8 bit 9 */
	bool unusedBit_4_9 : 1 {};
	/**
	offset 8 bit 10 */
	bool unusedBit_4_10 : 1 {};
	/**
	offset 8 bit 11 */
	bool unusedBit_4_11 : 1 {};
	/**
	offset 8 bit 12 */
	bool unusedBit_4_12 : 1 {};
	/**
	offset 8 bit 13 */
	bool unusedBit_4_13 : 1 {};
	/**
	offset 8 bit 14 */
	bool unusedBit_4_14 : 1 {};
	/**
	offset 8 bit 15 */
	bool unusedBit_4_15 : 1 {};
	/**
	offset 8 bit 16 */
	bool unusedBit_4_16 : 1 {};
	/**
	offset 8 bit 17 */
	bool unusedBit_4_17 : 1 {};
	/**
	offset 8 bit 18 */
	bool unusedBit_4_18 : 1 {};
	/**
	offset 8 bit 19 */
	bool unusedBit_4_19 : 1 {};
	/**
	offset 8 bit 20 */
	bool unusedBit_4_20 : 1 {};
	/**
	offset 8 bit 21 */
	bool unusedBit_4_21 : 1 {};
	/**
	offset 8 bit 22 */
	bool unusedBit_4_22 : 1 {};
	/**
	offset 8 bit 23 */
	bool unusedBit_4_23 : 1 {};
	/**
	offset 8 bit 24 */
	bool unusedBit_4_24 : 1 {};
	/**
	offset 8 bit 25 */
	bool unusedBit_4_25 : 1 {};
	/**
	offset 8 bit 26 */
	bool unusedBit_4_26 : 1 {};
	/**
	offset 8 bit 27 */
	bool unusedBit_4_27 : 1 {};
	/**
	offset 8 bit 28 */
	bool unusedBit_4_28 : 1 {};
	/**
	offset 8 bit 29 */
	bool unusedBit_4_29 : 1 {};
	/**
	offset 8 bit 30 */
	bool unusedBit_4_30 : 1 {};
	/**
	offset 8 bit 31 */
	bool unusedBit_4_31 : 1 {};
};
static_assert(sizeof(dc_motors_s) == 12);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/dc_motors.txt
