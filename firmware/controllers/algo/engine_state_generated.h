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
static_assert(sizeof(LuaAdjustments) == 12);
static_assert(offsetof(LuaAdjustments, fuelAdd) == 0);
static_assert(offsetof(LuaAdjustments, fuelMult) == 4);

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
static_assert(offsetof(speed_density_s, tCharge) == 0);
static_assert(offsetof(speed_density_s, tChargeK) == 4);

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
static_assert(offsetof(cranking_fuel_s, coolantTemperatureCoefficient) == 0);
static_assert(offsetof(cranking_fuel_s, tpsCoefficient) == 4);
static_assert(offsetof(cranking_fuel_s, durationCoefficient) == 8);
static_assert(offsetof(cranking_fuel_s, fuel) == 12);

struct engine_state_s {
	// offset 0
	LuaAdjustments lua;
	// offset 12
	speed_density_s sd;
	// offset 20
	cranking_fuel_s crankingFuel;
	// Fuel: Barometric pressure mult
	// offset 36
	float baroCorrection = (float)0;
	// Detected Board ID
	// id
	// offset 40
	int16_t hellenBoardId = (int16_t)0;
	// offset 42
	uint8_t alignmentFill_at_42[2];
	// Clutch: up
	// offset 44 bit 0
	bool clutchUpState : 1 {};
	// Clutch: down
	// offset 44 bit 1
	bool clutchDownState : 1 {};
	// Brake switch
	// offset 44 bit 2
	bool brakePedalState : 1 {};
	// offset 44 bit 3
	bool startStopState : 1 {};
	// offset 44 bit 4
	bool unusedBit_44_4 : 1 {};
	// offset 44 bit 5
	bool unusedBit_44_5 : 1 {};
	// offset 44 bit 6
	bool unusedBit_44_6 : 1 {};
	// offset 44 bit 7
	bool unusedBit_44_7 : 1 {};
	// offset 44 bit 8
	bool unusedBit_44_8 : 1 {};
	// offset 44 bit 9
	bool unusedBit_44_9 : 1 {};
	// offset 44 bit 10
	bool unusedBit_44_10 : 1 {};
	// offset 44 bit 11
	bool unusedBit_44_11 : 1 {};
	// offset 44 bit 12
	bool unusedBit_44_12 : 1 {};
	// offset 44 bit 13
	bool unusedBit_44_13 : 1 {};
	// offset 44 bit 14
	bool unusedBit_44_14 : 1 {};
	// offset 44 bit 15
	bool unusedBit_44_15 : 1 {};
	// offset 44 bit 16
	bool unusedBit_44_16 : 1 {};
	// offset 44 bit 17
	bool unusedBit_44_17 : 1 {};
	// offset 44 bit 18
	bool unusedBit_44_18 : 1 {};
	// offset 44 bit 19
	bool unusedBit_44_19 : 1 {};
	// offset 44 bit 20
	bool unusedBit_44_20 : 1 {};
	// offset 44 bit 21
	bool unusedBit_44_21 : 1 {};
	// offset 44 bit 22
	bool unusedBit_44_22 : 1 {};
	// offset 44 bit 23
	bool unusedBit_44_23 : 1 {};
	// offset 44 bit 24
	bool unusedBit_44_24 : 1 {};
	// offset 44 bit 25
	bool unusedBit_44_25 : 1 {};
	// offset 44 bit 26
	bool unusedBit_44_26 : 1 {};
	// offset 44 bit 27
	bool unusedBit_44_27 : 1 {};
	// offset 44 bit 28
	bool unusedBit_44_28 : 1 {};
	// offset 44 bit 29
	bool unusedBit_44_29 : 1 {};
	// offset 44 bit 30
	bool unusedBit_44_30 : 1 {};
	// offset 44 bit 31
	bool unusedBit_44_31 : 1 {};
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
	// Fuel: Load
	// offset 72
	float fuelingLoad = (float)0;
	// Ign: Load
	// offset 76
	float ignitionLoad = (float)0;
	// %
	// offset 80
	scaled_channel<uint16_t, 100, 1> veTableYAxis = (uint16_t)0;
	// deg
	// offset 82
	scaled_channel<uint16_t, 100, 1> mapAveragingDuration = (uint16_t)0;
};
static_assert(sizeof(engine_state_s) == 84);
static_assert(offsetof(engine_state_s, baroCorrection) == 36);
static_assert(offsetof(engine_state_s, hellenBoardId) == 40);
static_assert(offsetof(engine_state_s, startStopStateToggleCounter) == 48);
static_assert(offsetof(engine_state_s, egtValue1) == 52);
static_assert(offsetof(engine_state_s, egtValue2) == 56);
static_assert(offsetof(engine_state_s, desiredRpmLimit) == 60);
static_assert(offsetof(engine_state_s, fuelInjectionCounter) == 64);
static_assert(offsetof(engine_state_s, sparkCounter) == 68);
static_assert(offsetof(engine_state_s, fuelingLoad) == 72);
static_assert(offsetof(engine_state_s, ignitionLoad) == 76);
static_assert(offsetof(engine_state_s, veTableYAxis) == 80);
static_assert(offsetof(engine_state_s, mapAveragingDuration) == 82);

