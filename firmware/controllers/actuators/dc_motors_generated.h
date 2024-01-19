#pragma once
#include "rusefi_types.h"
struct dc_motors_s {
	// DC: output0
	// per
	// offset 0
	float dcOutput0 = (float)0;
	// DC: en0
	// per
	// offset 4
	uint8_t isEnabled0_int = (uint8_t)0;
	// offset 5
	uint8_t alignmentFill_at_5[3];
	// offset 8 bit 0
	bool isEnabled0 : 1 {};
	// offset 8 bit 1
	bool unusedBit_8_1 : 1 {};
	// offset 8 bit 2
	bool unusedBit_8_2 : 1 {};
	// offset 8 bit 3
	bool unusedBit_8_3 : 1 {};
	// offset 8 bit 4
	bool unusedBit_8_4 : 1 {};
	// offset 8 bit 5
	bool unusedBit_8_5 : 1 {};
	// offset 8 bit 6
	bool unusedBit_8_6 : 1 {};
	// offset 8 bit 7
	bool unusedBit_8_7 : 1 {};
	// offset 8 bit 8
	bool unusedBit_8_8 : 1 {};
	// offset 8 bit 9
	bool unusedBit_8_9 : 1 {};
	// offset 8 bit 10
	bool unusedBit_8_10 : 1 {};
	// offset 8 bit 11
	bool unusedBit_8_11 : 1 {};
	// offset 8 bit 12
	bool unusedBit_8_12 : 1 {};
	// offset 8 bit 13
	bool unusedBit_8_13 : 1 {};
	// offset 8 bit 14
	bool unusedBit_8_14 : 1 {};
	// offset 8 bit 15
	bool unusedBit_8_15 : 1 {};
	// offset 8 bit 16
	bool unusedBit_8_16 : 1 {};
	// offset 8 bit 17
	bool unusedBit_8_17 : 1 {};
	// offset 8 bit 18
	bool unusedBit_8_18 : 1 {};
	// offset 8 bit 19
	bool unusedBit_8_19 : 1 {};
	// offset 8 bit 20
	bool unusedBit_8_20 : 1 {};
	// offset 8 bit 21
	bool unusedBit_8_21 : 1 {};
	// offset 8 bit 22
	bool unusedBit_8_22 : 1 {};
	// offset 8 bit 23
	bool unusedBit_8_23 : 1 {};
	// offset 8 bit 24
	bool unusedBit_8_24 : 1 {};
	// offset 8 bit 25
	bool unusedBit_8_25 : 1 {};
	// offset 8 bit 26
	bool unusedBit_8_26 : 1 {};
	// offset 8 bit 27
	bool unusedBit_8_27 : 1 {};
	// offset 8 bit 28
	bool unusedBit_8_28 : 1 {};
	// offset 8 bit 29
	bool unusedBit_8_29 : 1 {};
	// offset 8 bit 30
	bool unusedBit_8_30 : 1 {};
	// offset 8 bit 31
	bool unusedBit_8_31 : 1 {};
};
static_assert(sizeof(dc_motors_s) == 12);
static_assert(offsetof(dc_motors_s, dcOutput0) == 0);
static_assert(offsetof(dc_motors_s, isEnabled0_int) == 4);

