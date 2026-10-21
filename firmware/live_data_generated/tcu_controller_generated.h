// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/tcu/tcu_controller.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of tcu_controller_s
struct tcu_controller_s {
	/**
	 * @@GAUGE_NAME_CURRENT_GEAR@@
	 * units: gear
	 * offset 0
	 */
	int8_t tcuCurrentGear = (int8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 1
	 */
	uint8_t alignmentFill_at_1[1] = {};
	/**
	 * @@GAUGE_NAME_TC_RATIO@@
	 * units: value
	 * offset 2
	 */
	scaled_channel<uint16_t, 100, 1> tcRatio = (uint16_t)0;
	/**
	 * offset 4
	 */
	float lastShiftTime = (float)0;
	/**
	 * "EPC Duty"
	 * units: %
	 * offset 8
	 */
	int8_t pressureControlDuty = (int8_t)0;
	/**
	 * "TC Duty"
	 * units: %
	 * offset 9
	 */
	int8_t torqueConverterDuty = (int8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 10
	 */
	uint8_t alignmentFill_at_10[2] = {};
	/**
	offset 12 bit 0 */
	bool isShifting : 1 {};
	/**
	offset 12 bit 1 */
	bool unusedBit_8_1 : 1 {};
	/**
	offset 12 bit 2 */
	bool unusedBit_8_2 : 1 {};
	/**
	offset 12 bit 3 */
	bool unusedBit_8_3 : 1 {};
	/**
	offset 12 bit 4 */
	bool unusedBit_8_4 : 1 {};
	/**
	offset 12 bit 5 */
	bool unusedBit_8_5 : 1 {};
	/**
	offset 12 bit 6 */
	bool unusedBit_8_6 : 1 {};
	/**
	offset 12 bit 7 */
	bool unusedBit_8_7 : 1 {};
	/**
	offset 12 bit 8 */
	bool unusedBit_8_8 : 1 {};
	/**
	offset 12 bit 9 */
	bool unusedBit_8_9 : 1 {};
	/**
	offset 12 bit 10 */
	bool unusedBit_8_10 : 1 {};
	/**
	offset 12 bit 11 */
	bool unusedBit_8_11 : 1 {};
	/**
	offset 12 bit 12 */
	bool unusedBit_8_12 : 1 {};
	/**
	offset 12 bit 13 */
	bool unusedBit_8_13 : 1 {};
	/**
	offset 12 bit 14 */
	bool unusedBit_8_14 : 1 {};
	/**
	offset 12 bit 15 */
	bool unusedBit_8_15 : 1 {};
	/**
	offset 12 bit 16 */
	bool unusedBit_8_16 : 1 {};
	/**
	offset 12 bit 17 */
	bool unusedBit_8_17 : 1 {};
	/**
	offset 12 bit 18 */
	bool unusedBit_8_18 : 1 {};
	/**
	offset 12 bit 19 */
	bool unusedBit_8_19 : 1 {};
	/**
	offset 12 bit 20 */
	bool unusedBit_8_20 : 1 {};
	/**
	offset 12 bit 21 */
	bool unusedBit_8_21 : 1 {};
	/**
	offset 12 bit 22 */
	bool unusedBit_8_22 : 1 {};
	/**
	offset 12 bit 23 */
	bool unusedBit_8_23 : 1 {};
	/**
	offset 12 bit 24 */
	bool unusedBit_8_24 : 1 {};
	/**
	offset 12 bit 25 */
	bool unusedBit_8_25 : 1 {};
	/**
	offset 12 bit 26 */
	bool unusedBit_8_26 : 1 {};
	/**
	offset 12 bit 27 */
	bool unusedBit_8_27 : 1 {};
	/**
	offset 12 bit 28 */
	bool unusedBit_8_28 : 1 {};
	/**
	offset 12 bit 29 */
	bool unusedBit_8_29 : 1 {};
	/**
	offset 12 bit 30 */
	bool unusedBit_8_30 : 1 {};
	/**
	offset 12 bit 31 */
	bool unusedBit_8_31 : 1 {};
};
static_assert(sizeof(tcu_controller_s) == 16);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/tcu/tcu_controller.txt
