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
	bool unusedBit_0_9 : 1 {};
	// offset 0 bit 10
	bool unusedBit_0_10 : 1 {};
	// offset 0 bit 11
	bool unusedBit_0_11 : 1 {};
	// offset 0 bit 12
	bool unusedBit_0_12 : 1 {};
	// offset 0 bit 13
	bool unusedBit_0_13 : 1 {};
	// offset 0 bit 14
	bool unusedBit_0_14 : 1 {};
	// offset 0 bit 15
	bool unusedBit_0_15 : 1 {};
	// offset 0 bit 16
	bool unusedBit_0_16 : 1 {};
	// offset 0 bit 17
	bool unusedBit_0_17 : 1 {};
	// offset 0 bit 18
	bool unusedBit_0_18 : 1 {};
	// offset 0 bit 19
	bool unusedBit_0_19 : 1 {};
	// offset 0 bit 20
	bool unusedBit_0_20 : 1 {};
	// offset 0 bit 21
	bool unusedBit_0_21 : 1 {};
	// offset 0 bit 22
	bool unusedBit_0_22 : 1 {};
	// offset 0 bit 23
	bool unusedBit_0_23 : 1 {};
	// offset 0 bit 24
	bool unusedBit_0_24 : 1 {};
	// offset 0 bit 25
	bool unusedBit_0_25 : 1 {};
	// offset 0 bit 26
	bool unusedBit_0_26 : 1 {};
	// offset 0 bit 27
	bool unusedBit_0_27 : 1 {};
	// offset 0 bit 28
	bool unusedBit_0_28 : 1 {};
	// offset 0 bit 29
	bool unusedBit_0_29 : 1 {};
	// offset 0 bit 30
	bool unusedBit_0_30 : 1 {};
	// offset 0 bit 31
	bool unusedBit_0_31 : 1 {};
	// Boost: Lua target add
	// v
	// offset 4
	scaled_channel<int16_t, 2, 1> luaTargetAdd = (int16_t)0;
	// Boost: Output
	// percent
	// offset 6
	scaled_channel<int16_t, 100, 1> boostOutput = (int16_t)0;
	// Boost: Lua target mult
	// v
	// offset 8
	float luaTargetMult = (float)0;
	// Boost: Open loop
	// v
	// offset 12
	float openLoopPart = (float)0;
	// Boost: Lua open loop add
	// v
	// offset 16
	float luaOpenLoopAdd = (float)0;
	// Boost: Closed loop
	// %
	// offset 20
	scaled_channel<int8_t, 2, 1> boostControllerClosedLoopPart = (int8_t)0;
	// offset 21
	uint8_t alignmentFill_at_21[1];
	// Boost: Target
	// kPa
	// offset 22
	scaled_channel<int16_t, 30, 1> boostControlTarget = (int16_t)0;
};
static_assert(sizeof(boost_control_s) == 24);
static_assert(offsetof(boost_control_s, luaTargetAdd) == 4);
static_assert(offsetof(boost_control_s, boostOutput) == 6);
static_assert(offsetof(boost_control_s, luaTargetMult) == 8);
static_assert(offsetof(boost_control_s, openLoopPart) == 12);
static_assert(offsetof(boost_control_s, luaOpenLoopAdd) == 16);
static_assert(offsetof(boost_control_s, boostControllerClosedLoopPart) == 20);
static_assert(offsetof(boost_control_s, boostControlTarget) == 22);

