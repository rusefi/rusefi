// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/engine_state.txt
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of LuaAdjustments
struct LuaAdjustments {
	/**
	 * Lua: Fuel add
	 * units: g
	 * offset 0
	 */
	float fuelAdd = (float)0;
	/**
	 * Lua: Fuel mult
	 * offset 4
	 */
	float fuelMult = (float)0;
	/**
	offset 8 bit 0 */
	bool clutchUpState : 1 {};
	/**
	offset 8 bit 1 */
	bool brakePedalState : 1 {};
	/**
	offset 8 bit 2 */
	bool acRequestState : 1 {};
	/**
	offset 8 bit 3 */
	bool luaDisableEtb : 1 {};
	/**
	offset 8 bit 4 */
	bool luaIgnCut : 1 {};
	/**
	offset 8 bit 5 */
	bool luaFuelCut : 1 {};
	/**
	offset 8 bit 6 */
	bool clutchDownState : 1 {};
	/**
	offset 8 bit 7 */
	bool disableDecelerationFuelCutOff : 1 {};
	/**
	offset 8 bit 8 */
	bool torqueReductionState : 1 {};
	/**
	offset 8 bit 9 */
	bool unusedBit_11_9 : 1 {};
	/**
	offset 8 bit 10 */
	bool unusedBit_11_10 : 1 {};
	/**
	offset 8 bit 11 */
	bool unusedBit_11_11 : 1 {};
	/**
	offset 8 bit 12 */
	bool unusedBit_11_12 : 1 {};
	/**
	offset 8 bit 13 */
	bool unusedBit_11_13 : 1 {};
	/**
	offset 8 bit 14 */
	bool unusedBit_11_14 : 1 {};
	/**
	offset 8 bit 15 */
	bool unusedBit_11_15 : 1 {};
	/**
	offset 8 bit 16 */
	bool unusedBit_11_16 : 1 {};
	/**
	offset 8 bit 17 */
	bool unusedBit_11_17 : 1 {};
	/**
	offset 8 bit 18 */
	bool unusedBit_11_18 : 1 {};
	/**
	offset 8 bit 19 */
	bool unusedBit_11_19 : 1 {};
	/**
	offset 8 bit 20 */
	bool unusedBit_11_20 : 1 {};
	/**
	offset 8 bit 21 */
	bool unusedBit_11_21 : 1 {};
	/**
	offset 8 bit 22 */
	bool unusedBit_11_22 : 1 {};
	/**
	offset 8 bit 23 */
	bool unusedBit_11_23 : 1 {};
	/**
	offset 8 bit 24 */
	bool unusedBit_11_24 : 1 {};
	/**
	offset 8 bit 25 */
	bool unusedBit_11_25 : 1 {};
	/**
	offset 8 bit 26 */
	bool unusedBit_11_26 : 1 {};
	/**
	offset 8 bit 27 */
	bool unusedBit_11_27 : 1 {};
	/**
	offset 8 bit 28 */
	bool unusedBit_11_28 : 1 {};
	/**
	offset 8 bit 29 */
	bool unusedBit_11_29 : 1 {};
	/**
	offset 8 bit 30 */
	bool unusedBit_11_30 : 1 {};
	/**
	offset 8 bit 31 */
	bool unusedBit_11_31 : 1 {};
};
static_assert(sizeof(LuaAdjustments) == 12);

// start of speed_density_s
struct speed_density_s {
	/**
	 * Air: Charge temperature estimate
	 * units: deg C
	 * offset 0
	 */
	scaled_channel<int16_t, 100, 1> tCharge = (int16_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 2
	 */
	uint8_t alignmentFill_at_2[2] = {};
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
	 * Fuel: Cranking cycle base mass
	 * units: mg
	 * offset 8
	 */
	scaled_channel<uint16_t, 100, 1> baseFuel = (uint16_t)0;
	/**
	 * Fuel: Cranking cycle mass
	 * units: mg
	 * offset 10
	 */
	scaled_channel<uint16_t, 100, 1> fuel = (uint16_t)0;
};
static_assert(sizeof(cranking_fuel_s) == 12);

// start of engine_state_s
struct engine_state_s {
	/**
	 * offset 0
	 */
	float injectionMass[MAX_CYLINDER_COUNT] = {};
	/**
	 * offset 48
	 */
	LuaAdjustments lua;
	/**
	 * offset 60
	 */
	speed_density_s sd;
	/**
	 * offset 68
	 */
	cranking_fuel_s crankingFuel;
	/**
	 * @@GAUGE_NAME_FUEL_BARO_CORR@@
	 * offset 80
	 */
	float baroCorrection = (float)0;
	/**
	 * Detected Board ID
	 * units: id
	 * offset 84
	 */
	int16_t hellenBoardId = (int16_t)0;
	/**
	 * @@INDICATOR_NAME_CLUTCH_UP@@
	 * offset 86
	 */
	int8_t clutchUpState = (int8_t)0;
	/**
	 * @@INDICATOR_NAME_BRAKE_DOWN@@
	 * offset 87
	 */
	int8_t brakePedalState = (int8_t)0;
	/**
	 * offset 88
	 */
	int8_t startStopState = (int8_t)0;
	/**
	 * offset 89
	 */
	int8_t smartChipState = (int8_t)0;
	/**
	 * offset 90
	 */
	int8_t smartChipRestartCounter = (int8_t)0;
	/**
	 * offset 91
	 */
	int8_t smartChipAliveCounter = (int8_t)0;
	/**
	offset 92 bit 0 */
	bool startStopPhysicalState : 1 {};
	/**
	 * Harley ACR Active
	offset 92 bit 1 */
	bool acrActive : 1 {};
	/**
	offset 92 bit 2 */
	bool acrEngineMovedRecently : 1 {};
	/**
	offset 92 bit 3 */
	bool heaterControlEnabled : 1 {};
	/**
	offset 92 bit 4 */
	bool luaDigitalState0 : 1 {};
	/**
	offset 92 bit 5 */
	bool luaDigitalState1 : 1 {};
	/**
	offset 92 bit 6 */
	bool luaDigitalState2 : 1 {};
	/**
	offset 92 bit 7 */
	bool luaDigitalState3 : 1 {};
	/**
	 * @@INDICATOR_NAME_CLUTCH_DOWN@@
	offset 92 bit 8 */
	bool clutchDownState : 1 {};
	/**
	offset 92 bit 9 */
	bool unusedBit_21_9 : 1 {};
	/**
	offset 92 bit 10 */
	bool unusedBit_21_10 : 1 {};
	/**
	offset 92 bit 11 */
	bool unusedBit_21_11 : 1 {};
	/**
	offset 92 bit 12 */
	bool unusedBit_21_12 : 1 {};
	/**
	offset 92 bit 13 */
	bool unusedBit_21_13 : 1 {};
	/**
	offset 92 bit 14 */
	bool unusedBit_21_14 : 1 {};
	/**
	offset 92 bit 15 */
	bool unusedBit_21_15 : 1 {};
	/**
	offset 92 bit 16 */
	bool unusedBit_21_16 : 1 {};
	/**
	offset 92 bit 17 */
	bool unusedBit_21_17 : 1 {};
	/**
	offset 92 bit 18 */
	bool unusedBit_21_18 : 1 {};
	/**
	offset 92 bit 19 */
	bool unusedBit_21_19 : 1 {};
	/**
	offset 92 bit 20 */
	bool unusedBit_21_20 : 1 {};
	/**
	offset 92 bit 21 */
	bool unusedBit_21_21 : 1 {};
	/**
	offset 92 bit 22 */
	bool unusedBit_21_22 : 1 {};
	/**
	offset 92 bit 23 */
	bool unusedBit_21_23 : 1 {};
	/**
	offset 92 bit 24 */
	bool unusedBit_21_24 : 1 {};
	/**
	offset 92 bit 25 */
	bool unusedBit_21_25 : 1 {};
	/**
	offset 92 bit 26 */
	bool unusedBit_21_26 : 1 {};
	/**
	offset 92 bit 27 */
	bool unusedBit_21_27 : 1 {};
	/**
	offset 92 bit 28 */
	bool unusedBit_21_28 : 1 {};
	/**
	offset 92 bit 29 */
	bool unusedBit_21_29 : 1 {};
	/**
	offset 92 bit 30 */
	bool unusedBit_21_30 : 1 {};
	/**
	offset 92 bit 31 */
	bool unusedBit_21_31 : 1 {};
	/**
	 * offset 96
	 */
	uint32_t startStopStateToggleCounter = (uint32_t)0;
	/**
	 * offset 100
	 */
	float currentVe = (float)0;
	/**
	 * offset 104
	 */
	float luaSoftSparkSkip = (float)0;
	/**
	 * offset 108
	 */
	float luaHardSparkSkip = (float)0;
	/**
	 * offset 112
	 */
	float tractionControlSparkSkip = (float)0;
	/**
	 * Fuel: Injection counter
	 * offset 116
	 */
	uint32_t fuelInjectionCounter = (uint32_t)0;
	/**
	 * Ign: Spark counter
	 * offset 120
	 */
	uint32_t globalSparkCounter = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_LOAD@@
	 * offset 124
	 */
	float fuelingLoad = (float)0;
	/**
	 * @@GAUGE_NAME_IGNITION_LOAD@@
	 * offset 128
	 */
	float ignitionLoad = (float)0;
	/**
	 * units: %
	 * offset 132
	 */
	scaled_channel<uint16_t, 100, 1> veTableYAxis = (uint16_t)0;
	/**
	 * offset 134
	 */
	uint8_t overDwellCounter = (uint8_t)0;
	/**
	 * offset 135
	 */
	uint8_t overDwellNotScheduledCounter = (uint8_t)0;
	/**
	 * offset 136
	 */
	uint8_t sparkOutOfOrderCounter = (uint8_t)0;
	/**
	 * need 4 byte alignment
	 * units: units
	 * offset 137
	 */
	uint8_t alignmentFill_at_137[3] = {};
	/**
	 * STFT: Bank
	 * units: %
	 * offset 140
	 */
	float stftCorrection[FT_BANK_COUNT] = {};
};
static_assert(sizeof(engine_state_s) == 148);

// end
// this section was generated automatically by rusEFI tool config_definition_base-all.jar based on (unknown script) controllers/algo/engine_state.txt
