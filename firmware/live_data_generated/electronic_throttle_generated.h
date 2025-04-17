// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/electronic_throttle.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of electronic_throttle_s
struct electronic_throttle_s {
	/**
	 * "ETB: target with idle"
	 * units: %
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
	 * units: %
	 * offset 8
	 */
	float luaAdjustment = (float)0;
	/**
	 * "DC: wastegatePosition"
	 * units: %
	 * offset 12
	 */
	float m_wastegatePosition = (float)0;
	/**
	 * "EBT: last PID dT"
	 * units: mS
	 * offset 16
	 */
	float m_lastPidDtMs = (float)0;
	/**
	 * offset 20
	 */
	percent_t etbFeedForward = (percent_t)0;
	/**
	 * Integral error
	 * offset 24
	 */
	float integralError = (float)0;
	/**
	 * ETB: target for current pedal
	 * units: %
	 * offset 28
	 */
	float etbCurrentTarget = (float)0;
	/**
	 * Adjusted target
	 * units: %
	 * offset 32
	 */
	scaled_channel<int16_t, 100, 1> m_adjustedTarget = (int16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 34
	 */
	uint8_t alignmentFill_at_34[2] = {};
	/**
	offset 36 bit 0 */
	bool etbRevLimitActive : 1 {};
	/**
	offset 36 bit 1 */
	bool jamDetected : 1 {};
	/**
	offset 36 bit 2 */
	bool validPlantPosition : 1 {};
	/**
	offset 36 bit 3 */
	bool unusedBit_13_3 : 1 {};
	/**
	offset 36 bit 4 */
	bool unusedBit_13_4 : 1 {};
	/**
	offset 36 bit 5 */
	bool unusedBit_13_5 : 1 {};
	/**
	offset 36 bit 6 */
	bool unusedBit_13_6 : 1 {};
	/**
	offset 36 bit 7 */
	bool unusedBit_13_7 : 1 {};
	/**
	offset 36 bit 8 */
	bool unusedBit_13_8 : 1 {};
	/**
	offset 36 bit 9 */
	bool unusedBit_13_9 : 1 {};
	/**
	offset 36 bit 10 */
	bool unusedBit_13_10 : 1 {};
	/**
	offset 36 bit 11 */
	bool unusedBit_13_11 : 1 {};
	/**
	offset 36 bit 12 */
	bool unusedBit_13_12 : 1 {};
	/**
	offset 36 bit 13 */
	bool unusedBit_13_13 : 1 {};
	/**
	offset 36 bit 14 */
	bool unusedBit_13_14 : 1 {};
	/**
	offset 36 bit 15 */
	bool unusedBit_13_15 : 1 {};
	/**
	offset 36 bit 16 */
	bool unusedBit_13_16 : 1 {};
	/**
	offset 36 bit 17 */
	bool unusedBit_13_17 : 1 {};
	/**
	offset 36 bit 18 */
	bool unusedBit_13_18 : 1 {};
	/**
	offset 36 bit 19 */
	bool unusedBit_13_19 : 1 {};
	/**
	offset 36 bit 20 */
	bool unusedBit_13_20 : 1 {};
	/**
	offset 36 bit 21 */
	bool unusedBit_13_21 : 1 {};
	/**
	offset 36 bit 22 */
	bool unusedBit_13_22 : 1 {};
	/**
	offset 36 bit 23 */
	bool unusedBit_13_23 : 1 {};
	/**
	offset 36 bit 24 */
	bool unusedBit_13_24 : 1 {};
	/**
	offset 36 bit 25 */
	bool unusedBit_13_25 : 1 {};
	/**
	offset 36 bit 26 */
	bool unusedBit_13_26 : 1 {};
	/**
	offset 36 bit 27 */
	bool unusedBit_13_27 : 1 {};
	/**
	offset 36 bit 28 */
	bool unusedBit_13_28 : 1 {};
	/**
	offset 36 bit 29 */
	bool unusedBit_13_29 : 1 {};
	/**
	offset 36 bit 30 */
	bool unusedBit_13_30 : 1 {};
	/**
	offset 36 bit 31 */
	bool unusedBit_13_31 : 1 {};
	/**
	 * "ETB TPS error counter"
	 * units: count
	 * offset 40
	 */
	uint16_t etbTpsErrorCounter = (uint16_t)0;
	/**
	 * "ETB pedal error counter"
	 * units: count
	 * offset 42
	 */
	uint16_t etbPpsErrorCounter = (uint16_t)0;
	/**
	 * offset 44
	 */
	int8_t etbErrorCode = (int8_t)0;
	/**
	 * offset 45
	 */
	int8_t etbErrorCodeBlinker = (int8_t)0;
	/**
	 * ETB traction control
	 * units: %
	 * offset 46
	 */
	int8_t tcEtbDrop = (int8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 47
	 */
	uint8_t alignmentFill_at_47[1] = {};
	/**
	 * ETB jam timer
	 * units: sec
	 * offset 48
	 */
	scaled_channel<uint16_t, 100, 1> jamTimer = (uint16_t)0;
	/**
	 * ETB with adjustments
	 * units: %
	 * offset 50
	 */
	int8_t adjustedEtbTarget = (int8_t)0;
	/**
	 * offset 51
	 */
	uint8_t state = (uint8_t)0;
};
static_assert(sizeof(electronic_throttle_s) == 52);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/electronic_throttle.txt
