// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/electronic_throttle.txt Fri Aug 26 01:07:49 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of electronic_throttle_s
struct electronic_throttle_s {
	/**
	 * offset 0
	 */
	float idlePosition = (float)0;
	/**
	 * offset 4
	 */
	float trim = (float)0;
	/**
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
	offset 24 bit 0 */
	bool etbRevLimitActive : 1 {};
	/**
	offset 24 bit 1 */
	bool unusedBit_7_1 : 1 {};
	/**
	offset 24 bit 2 */
	bool unusedBit_7_2 : 1 {};
	/**
	offset 24 bit 3 */
	bool unusedBit_7_3 : 1 {};
	/**
	offset 24 bit 4 */
	bool unusedBit_7_4 : 1 {};
	/**
	offset 24 bit 5 */
	bool unusedBit_7_5 : 1 {};
	/**
	offset 24 bit 6 */
	bool unusedBit_7_6 : 1 {};
	/**
	offset 24 bit 7 */
	bool unusedBit_7_7 : 1 {};
	/**
	offset 24 bit 8 */
	bool unusedBit_7_8 : 1 {};
	/**
	offset 24 bit 9 */
	bool unusedBit_7_9 : 1 {};
	/**
	offset 24 bit 10 */
	bool unusedBit_7_10 : 1 {};
	/**
	offset 24 bit 11 */
	bool unusedBit_7_11 : 1 {};
	/**
	offset 24 bit 12 */
	bool unusedBit_7_12 : 1 {};
	/**
	offset 24 bit 13 */
	bool unusedBit_7_13 : 1 {};
	/**
	offset 24 bit 14 */
	bool unusedBit_7_14 : 1 {};
	/**
	offset 24 bit 15 */
	bool unusedBit_7_15 : 1 {};
	/**
	offset 24 bit 16 */
	bool unusedBit_7_16 : 1 {};
	/**
	offset 24 bit 17 */
	bool unusedBit_7_17 : 1 {};
	/**
	offset 24 bit 18 */
	bool unusedBit_7_18 : 1 {};
	/**
	offset 24 bit 19 */
	bool unusedBit_7_19 : 1 {};
	/**
	offset 24 bit 20 */
	bool unusedBit_7_20 : 1 {};
	/**
	offset 24 bit 21 */
	bool unusedBit_7_21 : 1 {};
	/**
	offset 24 bit 22 */
	bool unusedBit_7_22 : 1 {};
	/**
	offset 24 bit 23 */
	bool unusedBit_7_23 : 1 {};
	/**
	offset 24 bit 24 */
	bool unusedBit_7_24 : 1 {};
	/**
	offset 24 bit 25 */
	bool unusedBit_7_25 : 1 {};
	/**
	offset 24 bit 26 */
	bool unusedBit_7_26 : 1 {};
	/**
	offset 24 bit 27 */
	bool unusedBit_7_27 : 1 {};
	/**
	offset 24 bit 28 */
	bool unusedBit_7_28 : 1 {};
	/**
	offset 24 bit 29 */
	bool unusedBit_7_29 : 1 {};
	/**
	offset 24 bit 30 */
	bool unusedBit_7_30 : 1 {};
	/**
	offset 24 bit 31 */
	bool unusedBit_7_31 : 1 {};
};
static_assert(sizeof(electronic_throttle_s) == 28);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/electronic_throttle.txt Fri Aug 26 01:07:49 UTC 2022
