// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/electronic_throttle.txt Wed Nov 23 01:41:35 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of electronic_throttle_s
struct electronic_throttle_s {
	/**
	 * "ETB: idlePosition"
	 * offset 0
	 */
	float idlePosition = (float)0;
	/**
	 * "ETB: trim"
	 * offset 4
	 */
	float trim = (float)0;
	/**
	 * "ETB: luaAdjustment"
	 * offset 8
	 */
	float luaAdjustment = (float)0;
	/**
	 * offset 12
	 */
	percent_t etbFeedForward = (percent_t)0;
	/**
	 * offset 16
	 */
	float etbIntegralError = (float)0;
	/**
	%
	 * offset 20
	 */
	float etbCurrentTarget = (float)0;
	/**
	%
	 * offset 24
	 */
	float etbCurrentAdjustedTarget = (float)0;
	/**
	offset 28 bit 0 */
	bool etbRevLimitActive : 1 {};
	/**
	offset 28 bit 1 */
	bool unusedBit_8_1 : 1 {};
	/**
	offset 28 bit 2 */
	bool unusedBit_8_2 : 1 {};
	/**
	offset 28 bit 3 */
	bool unusedBit_8_3 : 1 {};
	/**
	offset 28 bit 4 */
	bool unusedBit_8_4 : 1 {};
	/**
	offset 28 bit 5 */
	bool unusedBit_8_5 : 1 {};
	/**
	offset 28 bit 6 */
	bool unusedBit_8_6 : 1 {};
	/**
	offset 28 bit 7 */
	bool unusedBit_8_7 : 1 {};
	/**
	offset 28 bit 8 */
	bool unusedBit_8_8 : 1 {};
	/**
	offset 28 bit 9 */
	bool unusedBit_8_9 : 1 {};
	/**
	offset 28 bit 10 */
	bool unusedBit_8_10 : 1 {};
	/**
	offset 28 bit 11 */
	bool unusedBit_8_11 : 1 {};
	/**
	offset 28 bit 12 */
	bool unusedBit_8_12 : 1 {};
	/**
	offset 28 bit 13 */
	bool unusedBit_8_13 : 1 {};
	/**
	offset 28 bit 14 */
	bool unusedBit_8_14 : 1 {};
	/**
	offset 28 bit 15 */
	bool unusedBit_8_15 : 1 {};
	/**
	offset 28 bit 16 */
	bool unusedBit_8_16 : 1 {};
	/**
	offset 28 bit 17 */
	bool unusedBit_8_17 : 1 {};
	/**
	offset 28 bit 18 */
	bool unusedBit_8_18 : 1 {};
	/**
	offset 28 bit 19 */
	bool unusedBit_8_19 : 1 {};
	/**
	offset 28 bit 20 */
	bool unusedBit_8_20 : 1 {};
	/**
	offset 28 bit 21 */
	bool unusedBit_8_21 : 1 {};
	/**
	offset 28 bit 22 */
	bool unusedBit_8_22 : 1 {};
	/**
	offset 28 bit 23 */
	bool unusedBit_8_23 : 1 {};
	/**
	offset 28 bit 24 */
	bool unusedBit_8_24 : 1 {};
	/**
	offset 28 bit 25 */
	bool unusedBit_8_25 : 1 {};
	/**
	offset 28 bit 26 */
	bool unusedBit_8_26 : 1 {};
	/**
	offset 28 bit 27 */
	bool unusedBit_8_27 : 1 {};
	/**
	offset 28 bit 28 */
	bool unusedBit_8_28 : 1 {};
	/**
	offset 28 bit 29 */
	bool unusedBit_8_29 : 1 {};
	/**
	offset 28 bit 30 */
	bool unusedBit_8_30 : 1 {};
	/**
	offset 28 bit 31 */
	bool unusedBit_8_31 : 1 {};
};
static_assert(sizeof(electronic_throttle_s) == 32);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/electronic_throttle.txt Wed Nov 23 01:41:35 UTC 2022
