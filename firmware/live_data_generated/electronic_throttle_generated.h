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
	 * offset 16
	 */
	percent_t etbFeedForward = (percent_t)0;
	/**
	 * ETB: target for current pedal
	 * units: %
	 * offset 20
	 */
	float etbCurrentTarget = (float)0;
	/**
	 * Adjusted target
	 * units: %
	 * offset 24
	 */
	scaled_channel<int16_t, 100, 1> m_adjustedTarget = (int16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 26
	 */
	uint8_t alignmentFill_at_26[2] = {};
	/**
	offset 28 bit 0 */
	bool etbRevLimitActive : 1 {};
	/**
	offset 28 bit 1 */
	bool jamDetected : 1 {};
	/**
	offset 28 bit 2 */
	bool validPlantPosition : 1 {};
	/**
	offset 28 bit 3 */
	bool unusedBit_11_3 : 1 {};
	/**
	offset 28 bit 4 */
	bool unusedBit_11_4 : 1 {};
	/**
	offset 28 bit 5 */
	bool unusedBit_11_5 : 1 {};
	/**
	offset 28 bit 6 */
	bool unusedBit_11_6 : 1 {};
	/**
	offset 28 bit 7 */
	bool unusedBit_11_7 : 1 {};
	/**
	offset 28 bit 8 */
	bool unusedBit_11_8 : 1 {};
	/**
	offset 28 bit 9 */
	bool unusedBit_11_9 : 1 {};
	/**
	offset 28 bit 10 */
	bool unusedBit_11_10 : 1 {};
	/**
	offset 28 bit 11 */
	bool unusedBit_11_11 : 1 {};
	/**
	offset 28 bit 12 */
	bool unusedBit_11_12 : 1 {};
	/**
	offset 28 bit 13 */
	bool unusedBit_11_13 : 1 {};
	/**
	offset 28 bit 14 */
	bool unusedBit_11_14 : 1 {};
	/**
	offset 28 bit 15 */
	bool unusedBit_11_15 : 1 {};
	/**
	offset 28 bit 16 */
	bool unusedBit_11_16 : 1 {};
	/**
	offset 28 bit 17 */
	bool unusedBit_11_17 : 1 {};
	/**
	offset 28 bit 18 */
	bool unusedBit_11_18 : 1 {};
	/**
	offset 28 bit 19 */
	bool unusedBit_11_19 : 1 {};
	/**
	offset 28 bit 20 */
	bool unusedBit_11_20 : 1 {};
	/**
	offset 28 bit 21 */
	bool unusedBit_11_21 : 1 {};
	/**
	offset 28 bit 22 */
	bool unusedBit_11_22 : 1 {};
	/**
	offset 28 bit 23 */
	bool unusedBit_11_23 : 1 {};
	/**
	offset 28 bit 24 */
	bool unusedBit_11_24 : 1 {};
	/**
	offset 28 bit 25 */
	bool unusedBit_11_25 : 1 {};
	/**
	offset 28 bit 26 */
	bool unusedBit_11_26 : 1 {};
	/**
	offset 28 bit 27 */
	bool unusedBit_11_27 : 1 {};
	/**
	offset 28 bit 28 */
	bool unusedBit_11_28 : 1 {};
	/**
	offset 28 bit 29 */
	bool unusedBit_11_29 : 1 {};
	/**
	offset 28 bit 30 */
	bool unusedBit_11_30 : 1 {};
	/**
	offset 28 bit 31 */
	bool unusedBit_11_31 : 1 {};
	/**
	 * "ETB TPS error counter"
	 * units: count
	 * offset 32
	 */
	uint16_t etbTpsErrorCounter = (uint16_t)0;
	/**
	 * "ETB pedal error counter"
	 * units: count
	 * offset 34
	 */
	uint16_t etbPpsErrorCounter = (uint16_t)0;
	/**
	 * offset 36
	 */
	int8_t etbErrorCode = (int8_t)0;
	/**
	 * offset 37
	 */
	int8_t etbErrorCodeBlinker = (int8_t)0;
	/**
	 * ETB traction control
	 * units: %
	 * offset 38
	 */
	int8_t tcEtbDrop = (int8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 39
	 */
	uint8_t alignmentFill_at_39[1] = {};
	/**
	 * ETB jam timer
	 * units: sec
	 * offset 40
	 */
	scaled_channel<uint16_t, 100, 1> jamTimer = (uint16_t)0;
	/**
	 * ETB with adjustments
	 * units: %
	 * offset 42
	 */
	int8_t adjustedEtbTarget = (int8_t)0;
	/**
	 * offset 43
	 */
	uint8_t state = (uint8_t)0;
};
static_assert(sizeof(electronic_throttle_s) == 44);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/electronic_throttle.txt
