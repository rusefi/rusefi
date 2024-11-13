// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/fan_control.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of fan_control_s
struct fan_control_s {
	/**
	offset 0 bit 0 */
	bool cranking : 1 {};
	/**
	offset 0 bit 1 */
	bool notRunning : 1 {};
	/**
	offset 0 bit 2 */
	bool disabledWhileEngineStopped : 1 {};
	/**
	offset 0 bit 3 */
	bool brokenClt : 1 {};
	/**
	offset 0 bit 4 */
	bool enabledForAc : 1 {};
	/**
	offset 0 bit 5 */
	bool enabledAcOld : 1 {};
	/**
	offset 0 bit 6 */
	bool unusedBit_6_6 : 1 {};
	/**
	offset 0 bit 7 */
	bool unusedBit_6_7 : 1 {};
	/**
	offset 0 bit 8 */
	bool unusedBit_6_8 : 1 {};
	/**
	offset 0 bit 9 */
	bool unusedBit_6_9 : 1 {};
	/**
	offset 0 bit 10 */
	bool unusedBit_6_10 : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_6_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_6_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_6_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_6_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_6_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_6_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_6_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_6_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_6_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_6_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_6_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_6_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_6_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_6_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_6_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_6_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_6_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_6_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_6_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_6_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_6_31 : 1 {};
	/**
	 * offset 4
	 */
	uint32_t acEnableTime = (uint32_t)0;
	/**
	offset 8 bit 0 */
	bool hot : 1 {};
	/**
	offset 8 bit 1 */
	bool cold : 1 {};
	/**
	offset 8 bit 2 */
	bool disabledBySpeed : 1 {};
	/**
	offset 8 bit 3 */
	bool unusedBit_36_3 : 1 {};
	/**
	offset 8 bit 4 */
	bool unusedBit_36_4 : 1 {};
	/**
	offset 8 bit 5 */
	bool unusedBit_36_5 : 1 {};
	/**
	offset 8 bit 6 */
	bool unusedBit_36_6 : 1 {};
	/**
	offset 8 bit 7 */
	bool unusedBit_36_7 : 1 {};
	/**
	offset 8 bit 8 */
	bool unusedBit_36_8 : 1 {};
	/**
	offset 8 bit 9 */
	bool unusedBit_36_9 : 1 {};
	/**
	offset 8 bit 10 */
	bool unusedBit_36_10 : 1 {};
	/**
	offset 8 bit 11 */
	bool unusedBit_36_11 : 1 {};
	/**
	offset 8 bit 12 */
	bool unusedBit_36_12 : 1 {};
	/**
	offset 8 bit 13 */
	bool unusedBit_36_13 : 1 {};
	/**
	offset 8 bit 14 */
	bool unusedBit_36_14 : 1 {};
	/**
	offset 8 bit 15 */
	bool unusedBit_36_15 : 1 {};
	/**
	offset 8 bit 16 */
	bool unusedBit_36_16 : 1 {};
	/**
	offset 8 bit 17 */
	bool unusedBit_36_17 : 1 {};
	/**
	offset 8 bit 18 */
	bool unusedBit_36_18 : 1 {};
	/**
	offset 8 bit 19 */
	bool unusedBit_36_19 : 1 {};
	/**
	offset 8 bit 20 */
	bool unusedBit_36_20 : 1 {};
	/**
	offset 8 bit 21 */
	bool unusedBit_36_21 : 1 {};
	/**
	offset 8 bit 22 */
	bool unusedBit_36_22 : 1 {};
	/**
	offset 8 bit 23 */
	bool unusedBit_36_23 : 1 {};
	/**
	offset 8 bit 24 */
	bool unusedBit_36_24 : 1 {};
	/**
	offset 8 bit 25 */
	bool unusedBit_36_25 : 1 {};
	/**
	offset 8 bit 26 */
	bool unusedBit_36_26 : 1 {};
	/**
	offset 8 bit 27 */
	bool unusedBit_36_27 : 1 {};
	/**
	offset 8 bit 28 */
	bool unusedBit_36_28 : 1 {};
	/**
	offset 8 bit 29 */
	bool unusedBit_36_29 : 1 {};
	/**
	offset 8 bit 30 */
	bool unusedBit_36_30 : 1 {};
	/**
	offset 8 bit 31 */
	bool unusedBit_36_31 : 1 {};
	/**
	 * offset 12
	 */
	uint8_t radiatorFanStatus = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 13
	 */
	uint8_t alignmentFill_at_13[3];
};
static_assert(sizeof(fan_control_s) == 16);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/fan_control.txt
