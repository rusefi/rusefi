#pragma once
#include "rusefi_types.h"
struct tps_accel_state_s {
	// Fuel: TPS AE from
	// offset 0
	float tpsFrom = (float)0;
	// Fuel: TPS AE to
	// offset 4
	float tpsTo = (float)0;
	// Fuel: TPS AE change
	// offset 8
	float deltaTps = (float)0;
	// offset 12
	float extraFuel = (float)0;
	// offset 16
	float valueFromTable = (float)0;
	// Fuel: TPS AE Active
	// offset 20 bit 0
	bool isAboveAccelThreshold : 1 {};
	// offset 20 bit 1
	bool isBelowDecelThreshold : 1 {};
	// Accel: reset time
	// offset 20 bit 2
	bool isTimeToResetAccumulator : 1 {};
	// offset 20 bit 3
	bool isFractionalEnrichment : 1 {};
	// Accel: below threshold
	// offset 20 bit 4
	bool belowEpsilon : 1 {};
	// Accel: too short
	// offset 20 bit 5
	bool tooShort : 1 {};
	// offset 20 bit 6
	bool unusedBit_20_6 : 1 {};
	// offset 20 bit 7
	bool unusedBit_20_7 : 1 {};
	// offset 20 bit 8
	bool unusedBit_20_8 : 1 {};
	// offset 20 bit 9
	bool unusedBit_20_9 : 1 {};
	// offset 20 bit 10
	bool unusedBit_20_10 : 1 {};
	// offset 20 bit 11
	bool unusedBit_20_11 : 1 {};
	// offset 20 bit 12
	bool unusedBit_20_12 : 1 {};
	// offset 20 bit 13
	bool unusedBit_20_13 : 1 {};
	// offset 20 bit 14
	bool unusedBit_20_14 : 1 {};
	// offset 20 bit 15
	bool unusedBit_20_15 : 1 {};
	// offset 20 bit 16
	bool unusedBit_20_16 : 1 {};
	// offset 20 bit 17
	bool unusedBit_20_17 : 1 {};
	// offset 20 bit 18
	bool unusedBit_20_18 : 1 {};
	// offset 20 bit 19
	bool unusedBit_20_19 : 1 {};
	// offset 20 bit 20
	bool unusedBit_20_20 : 1 {};
	// offset 20 bit 21
	bool unusedBit_20_21 : 1 {};
	// offset 20 bit 22
	bool unusedBit_20_22 : 1 {};
	// offset 20 bit 23
	bool unusedBit_20_23 : 1 {};
	// offset 20 bit 24
	bool unusedBit_20_24 : 1 {};
	// offset 20 bit 25
	bool unusedBit_20_25 : 1 {};
	// offset 20 bit 26
	bool unusedBit_20_26 : 1 {};
	// offset 20 bit 27
	bool unusedBit_20_27 : 1 {};
	// offset 20 bit 28
	bool unusedBit_20_28 : 1 {};
	// offset 20 bit 29
	bool unusedBit_20_29 : 1 {};
	// offset 20 bit 30
	bool unusedBit_20_30 : 1 {};
	// offset 20 bit 31
	bool unusedBit_20_31 : 1 {};
	// offset 24
	float fractionalInjFuel = (float)0;
	// offset 28
	float accumulatedValue = (float)0;
	// offset 32
	float maxExtraPerCycle = (float)0;
	// offset 36
	float maxExtraPerPeriod = (float)0;
	// offset 40
	float maxInjectedPerPeriod = (float)0;
	// offset 44
	int cycleCnt = (int)0;
};
static_assert(sizeof(tps_accel_state_s) == 48);
static_assert(offsetof(tps_accel_state_s, tpsFrom) == 0);
static_assert(offsetof(tps_accel_state_s, tpsTo) == 4);
static_assert(offsetof(tps_accel_state_s, deltaTps) == 8);
static_assert(offsetof(tps_accel_state_s, extraFuel) == 12);
static_assert(offsetof(tps_accel_state_s, valueFromTable) == 16);
static_assert(offsetof(tps_accel_state_s, fractionalInjFuel) == 24);
static_assert(offsetof(tps_accel_state_s, accumulatedValue) == 28);
static_assert(offsetof(tps_accel_state_s, maxExtraPerCycle) == 32);
static_assert(offsetof(tps_accel_state_s, maxExtraPerPeriod) == 36);
static_assert(offsetof(tps_accel_state_s, maxInjectedPerPeriod) == 40);
static_assert(offsetof(tps_accel_state_s, cycleCnt) == 44);

