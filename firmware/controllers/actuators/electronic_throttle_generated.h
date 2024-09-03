// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/electronic_throttle.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of electronic_throttle_s
struct electronic_throttle_s {
	/**
	 * "ETB: target with idle"
	 * units: per
	 * offset 0
	 */
	float targetWithIdlePosition = (float)0;
	/**
	 * "ETB: trim"
	 * offset 4
	 */
	float trim = (float)0;
	/**
	 * "ETB: luaAdjustment"
	 * units: per
	 * offset 8
	 */
	float luaAdjustment = (float)0;
	/**
	 * "DC: wastegatePosition"
	 * units: per
	 * offset 12
	 */
	float m_wastegatePosition = (float)0;
	/**
	 * offset 16
	 */
	percent_t etbFeedForward = (percent_t)0;
	/**
	 * offset 20
	 */
	float etbIntegralError = (float)0;
	/**
	 * ETB: target for current pedal
	 * units: %
	 * offset 24
	 */
	float etbCurrentTarget = (float)0;
	/**
	 * units: %
	 * offset 28
	 */
	float etbCurrentAdjustedTarget = (float)0;
	/**
	offset 32 bit 0 */
	bool etbRevLimitActive : 1 {};
	/**
	offset 32 bit 1 */
	bool jamDetected : 1 {};
	/**
	offset 32 bit 2 */
	bool unusedBit_10_2 : 1 {};
	/**
	offset 32 bit 3 */
	bool unusedBit_10_3 : 1 {};
	/**
	offset 32 bit 4 */
	bool unusedBit_10_4 : 1 {};
	/**
	offset 32 bit 5 */
	bool unusedBit_10_5 : 1 {};
	/**
	offset 32 bit 6 */
	bool unusedBit_10_6 : 1 {};
	/**
	offset 32 bit 7 */
	bool unusedBit_10_7 : 1 {};
	/**
	offset 32 bit 8 */
	bool unusedBit_10_8 : 1 {};
	/**
	offset 32 bit 9 */
	bool unusedBit_10_9 : 1 {};
	/**
	offset 32 bit 10 */
	bool unusedBit_10_10 : 1 {};
	/**
	offset 32 bit 11 */
	bool unusedBit_10_11 : 1 {};
	/**
	offset 32 bit 12 */
	bool unusedBit_10_12 : 1 {};
	/**
	offset 32 bit 13 */
	bool unusedBit_10_13 : 1 {};
	/**
	offset 32 bit 14 */
	bool unusedBit_10_14 : 1 {};
	/**
	offset 32 bit 15 */
	bool unusedBit_10_15 : 1 {};
	/**
	offset 32 bit 16 */
	bool unusedBit_10_16 : 1 {};
	/**
	offset 32 bit 17 */
	bool unusedBit_10_17 : 1 {};
	/**
	offset 32 bit 18 */
	bool unusedBit_10_18 : 1 {};
	/**
	offset 32 bit 19 */
	bool unusedBit_10_19 : 1 {};
	/**
	offset 32 bit 20 */
	bool unusedBit_10_20 : 1 {};
	/**
	offset 32 bit 21 */
	bool unusedBit_10_21 : 1 {};
	/**
	offset 32 bit 22 */
	bool unusedBit_10_22 : 1 {};
	/**
	offset 32 bit 23 */
	bool unusedBit_10_23 : 1 {};
	/**
	offset 32 bit 24 */
	bool unusedBit_10_24 : 1 {};
	/**
	offset 32 bit 25 */
	bool unusedBit_10_25 : 1 {};
	/**
	offset 32 bit 26 */
	bool unusedBit_10_26 : 1 {};
	/**
	offset 32 bit 27 */
	bool unusedBit_10_27 : 1 {};
	/**
	offset 32 bit 28 */
	bool unusedBit_10_28 : 1 {};
	/**
	offset 32 bit 29 */
	bool unusedBit_10_29 : 1 {};
	/**
	offset 32 bit 30 */
	bool unusedBit_10_30 : 1 {};
	/**
	offset 32 bit 31 */
	bool unusedBit_10_31 : 1 {};
	/**
	 * "ETB duty rate of change"
	 * units: per
	 * offset 36
	 */
	float etbDutyRateOfChange = (float)0;
	/**
	 * "ETB average duty"
	 * units: %
	 * offset 40
	 */
	float etbDutyAverage = (float)0;
	/**
	 * "ETB TPS error counter"
	 * units: count
	 * offset 44
	 */
	uint16_t etbTpsErrorCounter = (uint16_t)0;
	/**
	 * "ETB pedal error counter"
	 * units: count
	 * offset 46
	 */
	uint16_t etbPpsErrorCounter = (uint16_t)0;
	/**
	 * offset 48
	 */
	int8_t etbErrorCode = (int8_t)0;
	/**
	 * ETB traction control
	 * units: %
	 * offset 49
	 */
	int8_t tcEtbDrop = (int8_t)0;
	/**
	 * ETB jam timer
	 * units: sec
	 * offset 50
	 */
	scaled_channel<uint16_t, 100, 1> jamTimer = (uint16_t)0;
	/**
	 * ETB with adjustments
	 * units: %
	 * offset 52
	 */
	int8_t adjustedEtbTarget = (int8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 53
	 */
	uint8_t alignmentFill_at_53[3];
};
static_assert(sizeof(electronic_throttle_s) == 56);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/electronic_throttle.txt
