#pragma once
#include "rusefi_types.h"
struct boost_control_s {
	// offset 0 bit 0
	bool isTpsInvalid : 1 {};
	// offset 0 bit 1
	bool m_shouldResetPid : 1 {};
	// offset 0 bit 2
	bool isBelowClosedLoopThreshold : 1 {};
	// offset 0 bit 3
	bool isNotClosedLoop : 1 {};
	// offset 0 bit 4
	bool isZeroRpm : 1 {};
	// offset 0 bit 5
	bool hasInitBoost : 1 {};
	// offset 0 bit 6
	bool rpmTooLow : 1 {};
	// offset 0 bit 7
	bool tpsTooLow : 1 {};
	// offset 0 bit 8
	bool mapTooLow : 1 {};
	// offset 0 bit 9
	bool unusedBit_9_9 : 1 {};
	// offset 0 bit 10
	bool unusedBit_9_10 : 1 {};
	// offset 0 bit 11
	bool unusedBit_9_11 : 1 {};
	// offset 0 bit 12
	bool unusedBit_9_12 : 1 {};
	// offset 0 bit 13
	bool unusedBit_9_13 : 1 {};
	// offset 0 bit 14
	bool unusedBit_9_14 : 1 {};
	// offset 0 bit 15
	bool unusedBit_9_15 : 1 {};
	// offset 0 bit 16
	bool unusedBit_9_16 : 1 {};
	// offset 0 bit 17
	bool unusedBit_9_17 : 1 {};
	// offset 0 bit 18
	bool unusedBit_9_18 : 1 {};
	// offset 0 bit 19
	bool unusedBit_9_19 : 1 {};
	// offset 0 bit 20
	bool unusedBit_9_20 : 1 {};
	// offset 0 bit 21
	bool unusedBit_9_21 : 1 {};
	// offset 0 bit 22
	bool unusedBit_9_22 : 1 {};
	// offset 0 bit 23
	bool unusedBit_9_23 : 1 {};
	// offset 0 bit 24
	bool unusedBit_9_24 : 1 {};
	// offset 0 bit 25
	bool unusedBit_9_25 : 1 {};
	// offset 0 bit 26
	bool unusedBit_9_26 : 1 {};
	// offset 0 bit 27
	bool unusedBit_9_27 : 1 {};
	// offset 0 bit 28
	bool unusedBit_9_28 : 1 {};
	// offset 0 bit 29
	bool unusedBit_9_29 : 1 {};
	// offset 0 bit 30
	bool unusedBit_9_30 : 1 {};
	// offset 0 bit 31
	bool unusedBit_9_31 : 1 {};
	// "Boost: Lua target add"
	// v
	// offset 4
	scaled_channel<int16_t, 2, 1> luaTargetAdd = (int16_t)0;
	// @@GAUGE_NAME_BOOST_OUTPUT@@
	// percent
	// offset 6
	scaled_channel<int16_t, 100, 1> boostOutput = (int16_t)0;
	// "Boost: Lua target mult"
	// v
	// offset 8
	float luaTargetMult = (float)0;
	// @@GAUGE_NAME_BOOST_OPEN_LOOP@@
	// v
	// offset 12
	float openLoopPart = (float)0;
	// "Boost: Lua open loop add"
	// v
	// offset 16
	float luaOpenLoopAdd = (float)0;
	// @@GAUGE_NAME_BOOST_CLOSED_LOOP@@
	// %
	// offset 20
	scaled_channel<int8_t, 2, 1> boostControllerClosedLoopPart = (int8_t)0;
	// offset 21
	uint8_t alignmentFill_at_21[1];
	// @@GAUGE_NAME_BOOST_TARGET@@
	// kPa
	// offset 22
	scaled_channel<int16_t, 30, 1> boostControlTarget = (int16_t)0;
};
static_assert(sizeof(boost_control_s) == 24);

