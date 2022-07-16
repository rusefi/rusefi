// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/boost_control.txt Thu Jul 14 13:56:19 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of boost_control_s
struct boost_control_s {
	/**
	offset 0 bit 0 */
	bool isTpsInvalid : 1 {};
	/**
	offset 0 bit 1 */
	bool m_shouldResetPid : 1 {};
	/**
	offset 0 bit 2 */
	bool isBelowClosedLoopThreshold : 1 {};
	/**
	offset 0 bit 3 */
	bool isNotClosedLoop : 1 {};
	/**
	offset 0 bit 4 */
	bool isZeroRpm : 1 {};
	/**
	offset 0 bit 5 */
	bool unusedBit_5_5 : 1 {};
	/**
	offset 0 bit 6 */
	bool unusedBit_5_6 : 1 {};
	/**
	offset 0 bit 7 */
	bool unusedBit_5_7 : 1 {};
	/**
	offset 0 bit 8 */
	bool unusedBit_5_8 : 1 {};
	/**
	offset 0 bit 9 */
	bool unusedBit_5_9 : 1 {};
	/**
	offset 0 bit 10 */
	bool unusedBit_5_10 : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_5_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_5_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_5_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_5_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_5_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_5_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_5_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_5_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_5_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_5_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_5_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_5_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_5_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_5_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_5_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_5_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_5_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_5_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_5_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_5_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_5_31 : 1 {};
	/**
	 * offset 4
	 */
	float luaTargetAdd = (float)0;
	/**
	 * offset 8
	 */
	float luaTargetMult = (float)0;
	/**
	 * offset 12
	 */
	float openLoopPart = (float)0;
	/**
	 * @@GAUGE_NAME_BOOST_CLOSED_LOOP@@
	%
	 * offset 16
	 */
	scaled_channel<int8_t, 2, 1> boostControllerClosedLoopPart = (int8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 17
	 */
	uint8_t alignmentFill_at_17[1];
	/**
	 * @@GAUGE_NAME_BOOST_TARGET@@
	kPa
	 * offset 18
	 */
	scaled_channel<int16_t, 30, 1> boostControlTarget = (int16_t)0;
};
static_assert(sizeof(boost_control_s) == 20);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/boost_control.txt Thu Jul 14 13:56:19 UTC 2022
