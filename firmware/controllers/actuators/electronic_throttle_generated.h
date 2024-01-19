#pragma once
#include "rusefi_types.h"
struct electronic_throttle_s {
	// ETB: idlePosition
	// per
	// offset 0
	float idlePosition = (float)0;
	// ETB: trim
	// offset 4
	float trim = (float)0;
	// ETB: luaAdjustment
	// per
	// offset 8
	float luaAdjustment = (float)0;
	// DC: wastegatePosition
	// per
	// offset 12
	float m_wastegatePosition = (float)0;
	// offset 16
	float etbFeedForward = (float)0;
	// offset 20
	float etbIntegralError = (float)0;
	// %
	// offset 24
	float etbCurrentTarget = (float)0;
	// %
	// offset 28
	float etbCurrentAdjustedTarget = (float)0;
	// offset 32 bit 0
	bool etbRevLimitActive : 1 {};
	// offset 32 bit 1
	bool jamDetected : 1 {};
	// offset 32 bit 2
	bool unusedBit_32_2 : 1 {};
	// offset 32 bit 3
	bool unusedBit_32_3 : 1 {};
	// offset 32 bit 4
	bool unusedBit_32_4 : 1 {};
	// offset 32 bit 5
	bool unusedBit_32_5 : 1 {};
	// offset 32 bit 6
	bool unusedBit_32_6 : 1 {};
	// offset 32 bit 7
	bool unusedBit_32_7 : 1 {};
	// offset 32 bit 8
	bool unusedBit_32_8 : 1 {};
	// offset 32 bit 9
	bool unusedBit_32_9 : 1 {};
	// offset 32 bit 10
	bool unusedBit_32_10 : 1 {};
	// offset 32 bit 11
	bool unusedBit_32_11 : 1 {};
	// offset 32 bit 12
	bool unusedBit_32_12 : 1 {};
	// offset 32 bit 13
	bool unusedBit_32_13 : 1 {};
	// offset 32 bit 14
	bool unusedBit_32_14 : 1 {};
	// offset 32 bit 15
	bool unusedBit_32_15 : 1 {};
	// offset 32 bit 16
	bool unusedBit_32_16 : 1 {};
	// offset 32 bit 17
	bool unusedBit_32_17 : 1 {};
	// offset 32 bit 18
	bool unusedBit_32_18 : 1 {};
	// offset 32 bit 19
	bool unusedBit_32_19 : 1 {};
	// offset 32 bit 20
	bool unusedBit_32_20 : 1 {};
	// offset 32 bit 21
	bool unusedBit_32_21 : 1 {};
	// offset 32 bit 22
	bool unusedBit_32_22 : 1 {};
	// offset 32 bit 23
	bool unusedBit_32_23 : 1 {};
	// offset 32 bit 24
	bool unusedBit_32_24 : 1 {};
	// offset 32 bit 25
	bool unusedBit_32_25 : 1 {};
	// offset 32 bit 26
	bool unusedBit_32_26 : 1 {};
	// offset 32 bit 27
	bool unusedBit_32_27 : 1 {};
	// offset 32 bit 28
	bool unusedBit_32_28 : 1 {};
	// offset 32 bit 29
	bool unusedBit_32_29 : 1 {};
	// offset 32 bit 30
	bool unusedBit_32_30 : 1 {};
	// offset 32 bit 31
	bool unusedBit_32_31 : 1 {};
	// ETB duty rate of change
	// per
	// offset 36
	float etbDutyRateOfChange = (float)0;
	// ETB average duty
	// per
	// offset 40
	float etbDutyAverage = (float)0;
	// ETB TPS error counter
	// count
	// offset 44
	uint16_t etbTpsErrorCounter = (uint16_t)0;
	// ETB pedal error counter
	// count
	// offset 46
	uint16_t etbPpsErrorCounter = (uint16_t)0;
	// offset 48
	int8_t etbErrorCode = (int8_t)0;
	// offset 49
	uint8_t alignmentFill_at_49[1];
	// ETB jam timer
	// sec
	// offset 50
	scaled_channel<uint16_t, 100, 1> jamTimer = (uint16_t)0;
};
static_assert(sizeof(electronic_throttle_s) == 52);
static_assert(offsetof(electronic_throttle_s, idlePosition) == 0);
static_assert(offsetof(electronic_throttle_s, trim) == 4);
static_assert(offsetof(electronic_throttle_s, luaAdjustment) == 8);
static_assert(offsetof(electronic_throttle_s, m_wastegatePosition) == 12);
static_assert(offsetof(electronic_throttle_s, etbFeedForward) == 16);
static_assert(offsetof(electronic_throttle_s, etbIntegralError) == 20);
static_assert(offsetof(electronic_throttle_s, etbCurrentTarget) == 24);
static_assert(offsetof(electronic_throttle_s, etbCurrentAdjustedTarget) == 28);
static_assert(offsetof(electronic_throttle_s, etbDutyRateOfChange) == 36);
static_assert(offsetof(electronic_throttle_s, etbDutyAverage) == 40);
static_assert(offsetof(electronic_throttle_s, etbTpsErrorCounter) == 44);
static_assert(offsetof(electronic_throttle_s, etbPpsErrorCounter) == 46);
static_assert(offsetof(electronic_throttle_s, etbErrorCode) == 48);
static_assert(offsetof(electronic_throttle_s, jamTimer) == 50);

