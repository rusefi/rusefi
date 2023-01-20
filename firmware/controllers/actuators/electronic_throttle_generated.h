// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/electronic_throttle.txt Tue Jan 10 21:34:44 UTC 2023
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
	bool jamDetected : 1 {};
	/**
	offset 28 bit 2 */
	bool unusedBit_9_2 : 1 {};
	/**
	offset 28 bit 3 */
	bool unusedBit_9_3 : 1 {};
	/**
	offset 28 bit 4 */
	bool unusedBit_9_4 : 1 {};
	/**
	offset 28 bit 5 */
	bool unusedBit_9_5 : 1 {};
	/**
	offset 28 bit 6 */
	bool unusedBit_9_6 : 1 {};
	/**
	offset 28 bit 7 */
	bool unusedBit_9_7 : 1 {};
	/**
	offset 28 bit 8 */
	bool unusedBit_9_8 : 1 {};
	/**
	offset 28 bit 9 */
	bool unusedBit_9_9 : 1 {};
	/**
	offset 28 bit 10 */
	bool unusedBit_9_10 : 1 {};
	/**
	offset 28 bit 11 */
	bool unusedBit_9_11 : 1 {};
	/**
	offset 28 bit 12 */
	bool unusedBit_9_12 : 1 {};
	/**
	offset 28 bit 13 */
	bool unusedBit_9_13 : 1 {};
	/**
	offset 28 bit 14 */
	bool unusedBit_9_14 : 1 {};
	/**
	offset 28 bit 15 */
	bool unusedBit_9_15 : 1 {};
	/**
	offset 28 bit 16 */
	bool unusedBit_9_16 : 1 {};
	/**
	offset 28 bit 17 */
	bool unusedBit_9_17 : 1 {};
	/**
	offset 28 bit 18 */
	bool unusedBit_9_18 : 1 {};
	/**
	offset 28 bit 19 */
	bool unusedBit_9_19 : 1 {};
	/**
	offset 28 bit 20 */
	bool unusedBit_9_20 : 1 {};
	/**
	offset 28 bit 21 */
	bool unusedBit_9_21 : 1 {};
	/**
	offset 28 bit 22 */
	bool unusedBit_9_22 : 1 {};
	/**
	offset 28 bit 23 */
	bool unusedBit_9_23 : 1 {};
	/**
	offset 28 bit 24 */
	bool unusedBit_9_24 : 1 {};
	/**
	offset 28 bit 25 */
	bool unusedBit_9_25 : 1 {};
	/**
	offset 28 bit 26 */
	bool unusedBit_9_26 : 1 {};
	/**
	offset 28 bit 27 */
	bool unusedBit_9_27 : 1 {};
	/**
	offset 28 bit 28 */
	bool unusedBit_9_28 : 1 {};
	/**
	offset 28 bit 29 */
	bool unusedBit_9_29 : 1 {};
	/**
	offset 28 bit 30 */
	bool unusedBit_9_30 : 1 {};
	/**
	offset 28 bit 31 */
	bool unusedBit_9_31 : 1 {};
	/**
	 * offset 32
	 */
	float etbDutyRateOfChange = (float)0;
	/**
	 * offset 36
	 */
	float etbDutyAverage = (float)0;
	/**
	 * "ETB TPS error counter"
	 * offset 40
	 */
	uint16_t etbTpsErrorCounter = (uint16_t)0;
	/**
	 * "ETB pedal error counter"
	 * offset 42
	 */
	uint16_t etbPpsErrorCounter = (uint16_t)0;
	/**
	 * offset 44
	 */
	int8_t etbErrorCode = (int8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 45
	 */
	uint8_t alignmentFill_at_45[1];
	/**
	 * ETB jam timer
	sec
	 * offset 46
	 */
	scaled_channel<uint16_t, 100, 1> jamTimer = (uint16_t)0;
};
static_assert(sizeof(electronic_throttle_s) == 48);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/electronic_throttle.txt Tue Jan 10 21:34:44 UTC 2023
