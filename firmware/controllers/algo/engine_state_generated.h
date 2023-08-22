// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/algo/engine_state.txt Tue Aug 22 18:19:47 UTC 2023
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of LuaAdjustments
struct LuaAdjustments {
	/**
	 * Lua: Fuel add
	g
	 * offset 0
	 */
	float fuelAdd = (float)0;
	/**
	 * Lua: Fuel mult
	 * offset 4
	 */
	float fuelMult = (float)0;
	/**
	 * offset 8
	 */
	int8_t clutchUpState = (int8_t)0;
	/**
	 * offset 9
	 */
	int8_t brakePedalState = (int8_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 10
	 */
	uint8_t alignmentFill_at_10[2];
	/**
	offset 12 bit 0 */
	bool acRequestState : 1 {};
	/**
	offset 12 bit 1 */
	bool luaDisableEtb : 1 {};
	/**
	offset 12 bit 2 */
	bool luaIgnCut : 1 {};
	/**
	offset 12 bit 3 */
	bool unusedBit_8_3 : 1 {};
	/**
	offset 12 bit 4 */
	bool unusedBit_8_4 : 1 {};
	/**
	offset 12 bit 5 */
	bool unusedBit_8_5 : 1 {};
	/**
	offset 12 bit 6 */
	bool unusedBit_8_6 : 1 {};
	/**
	offset 12 bit 7 */
	bool unusedBit_8_7 : 1 {};
	/**
	offset 12 bit 8 */
	bool unusedBit_8_8 : 1 {};
	/**
	offset 12 bit 9 */
	bool unusedBit_8_9 : 1 {};
	/**
	offset 12 bit 10 */
	bool unusedBit_8_10 : 1 {};
	/**
	offset 12 bit 11 */
	bool unusedBit_8_11 : 1 {};
	/**
	offset 12 bit 12 */
	bool unusedBit_8_12 : 1 {};
	/**
	offset 12 bit 13 */
	bool unusedBit_8_13 : 1 {};
	/**
	offset 12 bit 14 */
	bool unusedBit_8_14 : 1 {};
	/**
	offset 12 bit 15 */
	bool unusedBit_8_15 : 1 {};
	/**
	offset 12 bit 16 */
	bool unusedBit_8_16 : 1 {};
	/**
	offset 12 bit 17 */
	bool unusedBit_8_17 : 1 {};
	/**
	offset 12 bit 18 */
	bool unusedBit_8_18 : 1 {};
	/**
	offset 12 bit 19 */
	bool unusedBit_8_19 : 1 {};
	/**
	offset 12 bit 20 */
	bool unusedBit_8_20 : 1 {};
	/**
	offset 12 bit 21 */
	bool unusedBit_8_21 : 1 {};
	/**
	offset 12 bit 22 */
	bool unusedBit_8_22 : 1 {};
	/**
	offset 12 bit 23 */
	bool unusedBit_8_23 : 1 {};
	/**
	offset 12 bit 24 */
	bool unusedBit_8_24 : 1 {};
	/**
	offset 12 bit 25 */
	bool unusedBit_8_25 : 1 {};
	/**
	offset 12 bit 26 */
	bool unusedBit_8_26 : 1 {};
	/**
	offset 12 bit 27 */
	bool unusedBit_8_27 : 1 {};
	/**
	offset 12 bit 28 */
	bool unusedBit_8_28 : 1 {};
	/**
	offset 12 bit 29 */
	bool unusedBit_8_29 : 1 {};
	/**
	offset 12 bit 30 */
	bool unusedBit_8_30 : 1 {};
	/**
	offset 12 bit 31 */
	bool unusedBit_8_31 : 1 {};
};
static_assert(sizeof(LuaAdjustments) == 16);

// start of speed_density_s
struct speed_density_s {
	/**
	 * Air: Charge temperature estimate
	deg C
	 * offset 0
	 */
	scaled_channel<int16_t, 100, 1> tCharge = (int16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 2
	 */
	uint8_t alignmentFill_at_2[2];
	/**
	 * Air: Charge temperature estimate K
	 * offset 4
	 */
	float tChargeK = (float)0;
};
static_assert(sizeof(speed_density_s) == 8);

// start of cranking_fuel_s
struct cranking_fuel_s {
	/**
	 * Fuel: cranking CLT mult
	 * offset 0
	 */
	float coolantTemperatureCoefficient = (float)0;
	/**
	 * Fuel: cranking TPS mult
	 * offset 4
	 */
	float tpsCoefficient = (float)0;
	/**
	 * Fuel: cranking duration mult
	 * offset 8
	 */
	float durationCoefficient = (float)0;
	/**
	 * Fuel: Cranking cycle mass
	mg
	 * offset 12
	 */
	scaled_channel<uint16_t, 100, 1> fuel = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 14
	 */
	uint8_t alignmentFill_at_14[2];
};
static_assert(sizeof(cranking_fuel_s) == 16);

// start of engine_state_s
struct engine_state_s {
	/**
	 * offset 0
	 */
	LuaAdjustments lua;
	/**
	 * offset 16
	 */
	speed_density_s sd;
	/**
	 * offset 24
	 */
	cranking_fuel_s crankingFuel;
	/**
	 * @@GAUGE_NAME_FUEL_BARO_CORR@@
	 * offset 40
	 */
	float baroCorrection = (float)0;
	/**
	 * Detected Board ID
	id
	 * offset 44
	 */
	int16_t hellenBoardId = (int16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 46
	 */
	uint8_t alignmentFill_at_46[2];
	/**
	 * @@INDICATOR_NAME_CLUTCH_UP@@
	offset 48 bit 0 */
	bool clutchUpState : 1 {};
	/**
	 * @@INDICATOR_NAME_CLUTCH_DOWN@@
	offset 48 bit 1 */
	bool clutchDownState : 1 {};
	/**
	 * @@INDICATOR_NAME_BRAKE_DOWN@@
	offset 48 bit 2 */
	bool brakePedalState : 1 {};
	/**
	offset 48 bit 3 */
	bool startStopState : 1 {};
	/**
	offset 48 bit 4 */
	bool unusedBit_10_4 : 1 {};
	/**
	offset 48 bit 5 */
	bool unusedBit_10_5 : 1 {};
	/**
	offset 48 bit 6 */
	bool unusedBit_10_6 : 1 {};
	/**
	offset 48 bit 7 */
	bool unusedBit_10_7 : 1 {};
	/**
	offset 48 bit 8 */
	bool unusedBit_10_8 : 1 {};
	/**
	offset 48 bit 9 */
	bool unusedBit_10_9 : 1 {};
	/**
	offset 48 bit 10 */
	bool unusedBit_10_10 : 1 {};
	/**
	offset 48 bit 11 */
	bool unusedBit_10_11 : 1 {};
	/**
	offset 48 bit 12 */
	bool unusedBit_10_12 : 1 {};
	/**
	offset 48 bit 13 */
	bool unusedBit_10_13 : 1 {};
	/**
	offset 48 bit 14 */
	bool unusedBit_10_14 : 1 {};
	/**
	offset 48 bit 15 */
	bool unusedBit_10_15 : 1 {};
	/**
	offset 48 bit 16 */
	bool unusedBit_10_16 : 1 {};
	/**
	offset 48 bit 17 */
	bool unusedBit_10_17 : 1 {};
	/**
	offset 48 bit 18 */
	bool unusedBit_10_18 : 1 {};
	/**
	offset 48 bit 19 */
	bool unusedBit_10_19 : 1 {};
	/**
	offset 48 bit 20 */
	bool unusedBit_10_20 : 1 {};
	/**
	offset 48 bit 21 */
	bool unusedBit_10_21 : 1 {};
	/**
	offset 48 bit 22 */
	bool unusedBit_10_22 : 1 {};
	/**
	offset 48 bit 23 */
	bool unusedBit_10_23 : 1 {};
	/**
	offset 48 bit 24 */
	bool unusedBit_10_24 : 1 {};
	/**
	offset 48 bit 25 */
	bool unusedBit_10_25 : 1 {};
	/**
	offset 48 bit 26 */
	bool unusedBit_10_26 : 1 {};
	/**
	offset 48 bit 27 */
	bool unusedBit_10_27 : 1 {};
	/**
	offset 48 bit 28 */
	bool unusedBit_10_28 : 1 {};
	/**
	offset 48 bit 29 */
	bool unusedBit_10_29 : 1 {};
	/**
	offset 48 bit 30 */
	bool unusedBit_10_30 : 1 {};
	/**
	offset 48 bit 31 */
	bool unusedBit_10_31 : 1 {};
	/**
	 * offset 52
	 */
	uint32_t startStopStateToggleCounter = (uint32_t)0;
	/**
	 * offset 56
	 */
	float egtValue1 = (float)0;
	/**
	 * offset 60
	 */
	float egtValue2 = (float)0;
	/**
	 * User-defined RPM hard limit
	rpm
	 * offset 64
	 */
	int16_t desiredRpmLimit = (int16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 66
	 */
	uint8_t alignmentFill_at_66[2];
	/**
	 * Fuel: Injection counter
	 * offset 68
	 */
	uint32_t fuelInjectionCounter = (uint32_t)0;
	/**
	 * Ign: Spark counter
	 * offset 72
	 */
	uint32_t sparkCounter = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_LOAD@@
	 * offset 76
	 */
	float fuelingLoad = (float)0;
	/**
	 * @@GAUGE_NAME_IGNITION_LOAD@@
	 * offset 80
	 */
	float ignitionLoad = (float)0;
	/**
	%
	 * offset 84
	 */
	scaled_channel<uint16_t, 100, 1> veTableYAxis = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 86
	 */
	uint8_t alignmentFill_at_86[2];
};
static_assert(sizeof(engine_state_s) == 88);

// end
// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/algo/engine_state.txt Tue Aug 22 18:19:47 UTC 2023
