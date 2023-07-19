#pragma once
#include "rusefi_types.h"
struct LuaAdjustments {
	// Lua: Fuel add
	// g
	// offset 0
	float fuelAdd = (float)0;
	// Lua: Fuel mult
	// offset 4
	float fuelMult = (float)0;
	// offset 8 bit 0
	bool clutchUpState : 1 {};
	// offset 8 bit 1
	bool brakePedalState : 1 {};
	// offset 8 bit 2
	bool acRequestState : 1 {};
	// offset 8 bit 3
	bool luaDisableEtb : 1 {};
	// offset 8 bit 4
	bool luaIgnCut : 1 {};
	// offset 8 bit 5
	bool unusedBit_7_5 : 1 {};
	// offset 8 bit 6
	bool unusedBit_7_6 : 1 {};
	// offset 8 bit 7
	bool unusedBit_7_7 : 1 {};
	// offset 8 bit 8
	bool unusedBit_7_8 : 1 {};
	// offset 8 bit 9
	bool unusedBit_7_9 : 1 {};
	// offset 8 bit 10
	bool unusedBit_7_10 : 1 {};
	// offset 8 bit 11
	bool unusedBit_7_11 : 1 {};
	// offset 8 bit 12
	bool unusedBit_7_12 : 1 {};
	// offset 8 bit 13
	bool unusedBit_7_13 : 1 {};
	// offset 8 bit 14
	bool unusedBit_7_14 : 1 {};
	// offset 8 bit 15
	bool unusedBit_7_15 : 1 {};
	// offset 8 bit 16
	bool unusedBit_7_16 : 1 {};
	// offset 8 bit 17
	bool unusedBit_7_17 : 1 {};
	// offset 8 bit 18
	bool unusedBit_7_18 : 1 {};
	// offset 8 bit 19
	bool unusedBit_7_19 : 1 {};
	// offset 8 bit 20
	bool unusedBit_7_20 : 1 {};
	// offset 8 bit 21
	bool unusedBit_7_21 : 1 {};
	// offset 8 bit 22
	bool unusedBit_7_22 : 1 {};
	// offset 8 bit 23
	bool unusedBit_7_23 : 1 {};
	// offset 8 bit 24
	bool unusedBit_7_24 : 1 {};
	// offset 8 bit 25
	bool unusedBit_7_25 : 1 {};
	// offset 8 bit 26
	bool unusedBit_7_26 : 1 {};
	// offset 8 bit 27
	bool unusedBit_7_27 : 1 {};
	// offset 8 bit 28
	bool unusedBit_7_28 : 1 {};
	// offset 8 bit 29
	bool unusedBit_7_29 : 1 {};
	// offset 8 bit 30
	bool unusedBit_7_30 : 1 {};
	// offset 8 bit 31
	bool unusedBit_7_31 : 1 {};
};
static_assert(sizeof(LuaAdjustments) == 12);

struct speed_density_s {
	// Air: Charge temperature estimate
	// deg C
	// offset 0
	scaled_channel<int16_t, 100, 1> tCharge = (int16_t)0;
	// offset 2
	uint8_t alignmentFill_at_2[2];
	// Air: Charge temperature estimate K
	// offset 4
	float tChargeK = (float)0;
};
static_assert(sizeof(speed_density_s) == 8);

struct cranking_fuel_s {
	// Fuel: cranking CLT mult
	// offset 0
	float coolantTemperatureCoefficient = (float)0;
	// Fuel: cranking TPS mult
	// offset 4
	float tpsCoefficient = (float)0;
	// Fuel: cranking duration mult
	// offset 8
	float durationCoefficient = (float)0;
	// Fuel: Cranking cycle mass
	// mg
	// offset 12
	scaled_channel<uint16_t, 100, 1> fuel = (uint16_t)0;
	// offset 14
	uint8_t alignmentFill_at_14[2];
};
static_assert(sizeof(cranking_fuel_s) == 16);

struct engine_state_s {
	// offset 0
	LuaAdjustments lua;
	// offset 12
	speed_density_s sd;
	// offset 20
	cranking_fuel_s crankingFuel;
	// @@GAUGE_NAME_FUEL_BARO_CORR@@
	// offset 36
	float baroCorrection = (float)0;
	// Detected Board ID
	// id
	// offset 40
	int16_t hellenBoardId = (int16_t)0;
	// offset 42
	uint8_t alignmentFill_at_42[2];
	// @@INDICATOR_NAME_CLUTCH_UP@@
	// offset 44 bit 0
	bool clutchUpState : 1 {};
	// @@INDICATOR_NAME_CLUTCH_DOWN@@
	// offset 44 bit 1
	bool clutchDownState : 1 {};
	// @@INDICATOR_NAME_BRAKE_DOWN@@
	// offset 44 bit 2
	bool brakePedalState : 1 {};
	// offset 44 bit 3
	bool startStopState : 1 {};
	// offset 44 bit 4
	bool unusedBit_10_4 : 1 {};
	// offset 44 bit 5
	bool unusedBit_10_5 : 1 {};
	// offset 44 bit 6
	bool unusedBit_10_6 : 1 {};
	// offset 44 bit 7
	bool unusedBit_10_7 : 1 {};
	// offset 44 bit 8
	bool unusedBit_10_8 : 1 {};
	// offset 44 bit 9
	bool unusedBit_10_9 : 1 {};
	// offset 44 bit 10
	bool unusedBit_10_10 : 1 {};
	// offset 44 bit 11
	bool unusedBit_10_11 : 1 {};
	// offset 44 bit 12
	bool unusedBit_10_12 : 1 {};
	// offset 44 bit 13
	bool unusedBit_10_13 : 1 {};
	// offset 44 bit 14
	bool unusedBit_10_14 : 1 {};
	// offset 44 bit 15
	bool unusedBit_10_15 : 1 {};
	// offset 44 bit 16
	bool unusedBit_10_16 : 1 {};
	// offset 44 bit 17
	bool unusedBit_10_17 : 1 {};
	// offset 44 bit 18
	bool unusedBit_10_18 : 1 {};
	// offset 44 bit 19
	bool unusedBit_10_19 : 1 {};
	// offset 44 bit 20
	bool unusedBit_10_20 : 1 {};
	// offset 44 bit 21
	bool unusedBit_10_21 : 1 {};
	// offset 44 bit 22
	bool unusedBit_10_22 : 1 {};
	// offset 44 bit 23
	bool unusedBit_10_23 : 1 {};
	// offset 44 bit 24
	bool unusedBit_10_24 : 1 {};
	// offset 44 bit 25
	bool unusedBit_10_25 : 1 {};
	// offset 44 bit 26
	bool unusedBit_10_26 : 1 {};
	// offset 44 bit 27
	bool unusedBit_10_27 : 1 {};
	// offset 44 bit 28
	bool unusedBit_10_28 : 1 {};
	// offset 44 bit 29
	bool unusedBit_10_29 : 1 {};
	// offset 44 bit 30
	bool unusedBit_10_30 : 1 {};
	// offset 44 bit 31
	bool unusedBit_10_31 : 1 {};
	// offset 48
	uint32_t startStopStateToggleCounter = (uint32_t)0;
	// offset 52
	float egtValue1 = (float)0;
	// offset 56
	float egtValue2 = (float)0;
	// User-defined RPM hard limit
	// rpm
	// offset 60
	int16_t desiredRpmLimit = (int16_t)0;
	// offset 62
	uint8_t alignmentFill_at_62[2];
	// Fuel: Injection counter
	// offset 64
	uint32_t fuelInjectionCounter = (uint32_t)0;
	// Ign: Spark counter
	// offset 68
	uint32_t sparkCounter = (uint32_t)0;
	// @@GAUGE_NAME_FUEL_LOAD@@
	// offset 72
	float fuelingLoad = (float)0;
	// @@GAUGE_NAME_IGNITION_LOAD@@
	// offset 76
	float ignitionLoad = (float)0;
	// %
	// offset 80
	scaled_channel<uint16_t, 100, 1> veTableYAxis = (uint16_t)0;
	// offset 82
	uint8_t alignmentFill_at_82[2];
};
static_assert(sizeof(engine_state_s) == 84);

