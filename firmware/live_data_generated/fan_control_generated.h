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
	 * Engine stopped
	offset 0 bit 2 */
	bool disabledWhileEngineStopped : 1 {};
	/**
	 * Broken CLT
	offset 0 bit 3 */
	bool brokenClt : 1 {};
	/**
	 * Enable for AC
	offset 0 bit 4 */
	bool enabledForAc : 1 {};
	/**
	 * Above hot threshold
	offset 0 bit 5 */
	bool hot : 1 {};
	/**
	 * Below cold threshold
	offset 0 bit 6 */
	bool cold : 1 {};
	/**
	offset 0 bit 7 */
	bool disabledBySpeed : 1 {};
	/**
	 * On
	offset 0 bit 8 */
	bool m_state : 1 {};
	/**
	offset 0 bit 9 */
	bool unusedBit_9_9 : 1 {};
	/**
	offset 0 bit 10 */
	bool unusedBit_9_10 : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_9_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_9_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_9_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_9_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_9_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_9_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_9_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_9_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_9_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_9_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_9_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_9_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_9_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_9_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_9_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_9_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_9_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_9_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_9_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_9_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_9_31 : 1 {};
	/**
	 * offset 4
	 */
	uint8_t radiatorFanStatus = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 5
	 */
	uint8_t alignmentFill_at_5[3] = {};
};
static_assert(sizeof(fan_control_s) == 8);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/fan_control.txt
