// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/boost_control.txt
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
	bool hasInitBoost : 1 {};
	/**
	offset 0 bit 6 */
	bool rpmTooLow : 1 {};
	/**
	offset 0 bit 7 */
	bool tpsTooLow : 1 {};
	/**
	offset 0 bit 8 */
	bool mapTooLow : 1 {};
	/**
	offset 0 bit 9 */
	bool isPlantValid : 1 {};
	/**
	offset 0 bit 10 */
	bool isBoostControlled : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_11_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_11_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_11_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_11_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_11_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_11_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_11_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_11_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_11_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_11_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_11_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_11_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_11_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_11_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_11_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_11_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_11_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_11_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_11_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_11_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_11_31 : 1 {};
	/**
	 * "Boost: Lua target add"
	 * units: percent
	 * offset 4
	 */
	scaled_channel<int16_t, 2, 1> luaTargetAdd = (int16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 6
	 */
	uint8_t alignmentFill_at_6[2] = {};
	/**
	 * "Boost: Lua target mult"
	 * offset 8
	 */
	float luaTargetMult = (float)0;
	/**
	 * @@GAUGE_NAME_BOOST_TARGET@@
	 * units: kPa
	 * offset 12
	 */
	scaled_channel<int16_t, 30, 1> boostControlTarget = (int16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 14
	 */
	uint8_t alignmentFill_at_14[2] = {};
	/**
	 * @@GAUGE_NAME_BOOST_OPEN_LOOP@@
	 * units: percent
	 * offset 16
	 */
	float openLoopPart = (float)0;
	/**
	 * offset 20
	 */
	scaled_channel<int16_t, 10, 1> openLoopYAxis = (int16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 22
	 */
	uint8_t alignmentFill_at_22[2] = {};
	/**
	 * "Boost: Lua open loop add"
	 * units: percent
	 * offset 24
	 */
	float luaOpenLoopAdd = (float)0;
	/**
	 * @@GAUGE_NAME_BOOST_CLOSED_LOOP@@
	 * units: %
	 * offset 28
	 */
	scaled_channel<int8_t, 2, 1> boostControllerClosedLoopPart = (int8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 29
	 */
	uint8_t alignmentFill_at_29[1] = {};
	/**
	 * @@GAUGE_NAME_BOOST_OUTPUT@@
	 * units: percent
	 * offset 30
	 */
	scaled_channel<int16_t, 100, 1> boostOutput = (int16_t)0;
};
static_assert(sizeof(boost_control_s) == 32);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/actuators/boost_control.txt
