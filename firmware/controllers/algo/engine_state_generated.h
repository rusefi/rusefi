// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/engine_state.txt Tue Nov 15 14:33:13 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of LuaAdjustments
struct LuaAdjustments {
	/**
	 * offset 0
	 */
	float fuelAdd = (float)0;
	/**
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
	bool unusedBit_6_4 : 1 {};
	/**
	offset 8 bit 5 */
	bool unusedBit_6_5 : 1 {};
	/**
	offset 8 bit 6 */
	bool unusedBit_6_6 : 1 {};
	/**
	offset 8 bit 7 */
	bool unusedBit_6_7 : 1 {};
	/**
	offset 8 bit 8 */
	bool unusedBit_6_8 : 1 {};
	/**
	offset 8 bit 9 */
	bool unusedBit_6_9 : 1 {};
	/**
	offset 8 bit 10 */
	bool unusedBit_6_10 : 1 {};
	/**
	offset 8 bit 11 */
	bool unusedBit_6_11 : 1 {};
	/**
	offset 8 bit 12 */
	bool unusedBit_6_12 : 1 {};
	/**
	offset 8 bit 13 */
	bool unusedBit_6_13 : 1 {};
	/**
	offset 8 bit 14 */
	bool unusedBit_6_14 : 1 {};
	/**
	offset 8 bit 15 */
	bool unusedBit_6_15 : 1 {};
	/**
	offset 8 bit 16 */
	bool unusedBit_6_16 : 1 {};
	/**
	offset 8 bit 17 */
	bool unusedBit_6_17 : 1 {};
	/**
	offset 8 bit 18 */
	bool unusedBit_6_18 : 1 {};
	/**
	offset 8 bit 19 */
	bool unusedBit_6_19 : 1 {};
	/**
	offset 8 bit 20 */
	bool unusedBit_6_20 : 1 {};
	/**
	offset 8 bit 21 */
	bool unusedBit_6_21 : 1 {};
	/**
	offset 8 bit 22 */
	bool unusedBit_6_22 : 1 {};
	/**
	offset 8 bit 23 */
	bool unusedBit_6_23 : 1 {};
	/**
	offset 8 bit 24 */
	bool unusedBit_6_24 : 1 {};
	/**
	offset 8 bit 25 */
	bool unusedBit_6_25 : 1 {};
	/**
	offset 8 bit 26 */
	bool unusedBit_6_26 : 1 {};
	/**
	offset 8 bit 27 */
	bool unusedBit_6_27 : 1 {};
	/**
	offset 8 bit 28 */
	bool unusedBit_6_28 : 1 {};
	/**
	offset 8 bit 29 */
	bool unusedBit_6_29 : 1 {};
	/**
	offset 8 bit 30 */
	bool unusedBit_6_30 : 1 {};
	/**
	offset 8 bit 31 */
	bool unusedBit_6_31 : 1 {};
};
static_assert(sizeof(LuaAdjustments) == 12);

// start of speed_density_s
struct speed_density_s {
	/**
	 * speed density
	 * Rate-of-change limiter is applied to degrees, so we store both Kelvin and degrees.
	 * offset 0
	 */
	float tCharge = (float)0;
	/**
	 * offset 4
	 */
	float tChargeK = (float)0;
	/**
	 * offset 8
	 */
	floatms_t airFlow = (floatms_t)0;
};
static_assert(sizeof(speed_density_s) == 12);

// start of cranking_fuel_s
struct cranking_fuel_s {
	/**
	 * Fuel: base duration
	 * Duration of injection, in ms. During cranking we do not account for injector flow, so if you change injectors you would need to change settings.
	 * Deprecated. Please use '1'.
	 * TODO: maybe account for injector flow?
	 * offset 0
	 */
	floatms_t baseFuel = (floatms_t)0;
	/**
	 * offset 4
	 */
	float coolantTemperatureCoefficient = (float)0;
	/**
	 * offset 8
	 */
	float tpsCoefficient = (float)0;
	/**
	 * offset 12
	 */
	float durationCoefficient = (float)0;
	/**
	 * Fuel: Actual injection
	 * duration based on all coefficients.
	 * offset 16
	 */
	floatms_t fuel = (floatms_t)0;
};
static_assert(sizeof(cranking_fuel_s) == 20);

// start of running_fuel_s
struct running_fuel_s {
	/**
	 * offset 0
	 */
	float postCrankingFuelCorrection = (float)0;
	/**
	 * @@GAUGE_NAME_FUEL_IAT_CORR@@
	 * offset 4
	 */
	float intakeTemperatureCoefficient = (float)0;
	/**
	 * @@GAUGE_NAME_FUEL_CLT_CORR@@
	 * offset 8
	 */
	float coolantTemperatureCoefficient = (float)0;
	/**
	 * offset 12
	 */
	float timeSinceCrankingInSecs = (float)0;
	/**
	 * offset 16
	 */
	floatms_t baseFuel = (floatms_t)0;
	/**
	 * Fuel: actual
	 * Actual injection duration with CLT, IAT and TPS acceleration corrections per cycle, as squirt duration.
	 * Without injector lag.
	 * @see baseFuel
	 * @see actualLastInjection
	 * offset 20
	 */
	floatms_t fuel = (floatms_t)0;
};
static_assert(sizeof(running_fuel_s) == 24);

// start of engine_state_s
struct engine_state_s {
	/**
	 * offset 0
	 */
	LuaAdjustments lua;
	/**
	 * offset 12
	 */
	speed_density_s sd;
	/**
	 * offset 24
	 */
	float engineCycleDurationMs = (float)0;
	/**
	 * offset 28
	 */
	float minRpmKcurrentTPS = (float)0;
	/**
	 * offset 32
	 */
	uint32_t currentTpsAdc = (uint32_t)0;
	/**
	 * offset 36
	 */
	float tpsVoltageMCU = (float)0;
	/**
	 * offset 40
	 */
	float tpsVoltageBoard = (float)0;
	/**
	 * offset 44
	 */
	float currentBaroCorrectedVE = (float)0;
	/**
	 * offset 48
	 */
	float injectorFlowCorrection = (float)0;
	/**
	 * @@GAUGE_NAME_FUEL_BARO_CORR@@
	 * offset 52
	 */
	float baroCorrection = (float)0;
	/**
	 * offset 56
	 */
	cranking_fuel_s crankingFuel;
	/**
	 * offset 76
	 */
	running_fuel_s running;
	/**
	 * ignition dwell duration in ms
	 * See also dwellAngle
	 * offset 100
	 */
	floatms_t sparkDwell = (floatms_t)0;
	/**
	 * ignition dwell duration
	 * as crankshaft angle
	 * NAN if engine is stopped
	 * See also sparkDwell
	 * offset 104
	 */
	angle_t dwellAngle = (angle_t)0;
	/**
	deg
	 * offset 108
	 */
	scaled_channel<int16_t, 100, 1> cltTimingCorrection = (int16_t)0;
	/**
	deg
	 * offset 110
	 */
	scaled_channel<int16_t, 100, 1> timingIatCorrection = (int16_t)0;
	/**
	deg
	 * offset 112
	 */
	scaled_channel<int16_t, 100, 1> timingPidCorrection = (int16_t)0;
	/**
	 * Detected Board ID
	id
	 * offset 114
	 */
	int16_t hellenBoardId = (int16_t)0;
	/**
	 * @@INDICATOR_NAME_CLUTCH_UP@@
	offset 116 bit 0 */
	bool clutchUpState : 1 {};
	/**
	 * @@INDICATOR_NAME_CLUTCH_DOWN@@
	offset 116 bit 1 */
	bool clutchDownState : 1 {};
	/**
	 * @@INDICATOR_NAME_BRAKE_DOWN@@
	offset 116 bit 2 */
	bool brakePedalState : 1 {};
	/**
	offset 116 bit 3 */
	bool startStopState : 1 {};
	/**
	offset 116 bit 4 */
	bool unusedBit_22_4 : 1 {};
	/**
	offset 116 bit 5 */
	bool unusedBit_22_5 : 1 {};
	/**
	offset 116 bit 6 */
	bool unusedBit_22_6 : 1 {};
	/**
	offset 116 bit 7 */
	bool unusedBit_22_7 : 1 {};
	/**
	offset 116 bit 8 */
	bool unusedBit_22_8 : 1 {};
	/**
	offset 116 bit 9 */
	bool unusedBit_22_9 : 1 {};
	/**
	offset 116 bit 10 */
	bool unusedBit_22_10 : 1 {};
	/**
	offset 116 bit 11 */
	bool unusedBit_22_11 : 1 {};
	/**
	offset 116 bit 12 */
	bool unusedBit_22_12 : 1 {};
	/**
	offset 116 bit 13 */
	bool unusedBit_22_13 : 1 {};
	/**
	offset 116 bit 14 */
	bool unusedBit_22_14 : 1 {};
	/**
	offset 116 bit 15 */
	bool unusedBit_22_15 : 1 {};
	/**
	offset 116 bit 16 */
	bool unusedBit_22_16 : 1 {};
	/**
	offset 116 bit 17 */
	bool unusedBit_22_17 : 1 {};
	/**
	offset 116 bit 18 */
	bool unusedBit_22_18 : 1 {};
	/**
	offset 116 bit 19 */
	bool unusedBit_22_19 : 1 {};
	/**
	offset 116 bit 20 */
	bool unusedBit_22_20 : 1 {};
	/**
	offset 116 bit 21 */
	bool unusedBit_22_21 : 1 {};
	/**
	offset 116 bit 22 */
	bool unusedBit_22_22 : 1 {};
	/**
	offset 116 bit 23 */
	bool unusedBit_22_23 : 1 {};
	/**
	offset 116 bit 24 */
	bool unusedBit_22_24 : 1 {};
	/**
	offset 116 bit 25 */
	bool unusedBit_22_25 : 1 {};
	/**
	offset 116 bit 26 */
	bool unusedBit_22_26 : 1 {};
	/**
	offset 116 bit 27 */
	bool unusedBit_22_27 : 1 {};
	/**
	offset 116 bit 28 */
	bool unusedBit_22_28 : 1 {};
	/**
	offset 116 bit 29 */
	bool unusedBit_22_29 : 1 {};
	/**
	offset 116 bit 30 */
	bool unusedBit_22_30 : 1 {};
	/**
	offset 116 bit 31 */
	bool unusedBit_22_31 : 1 {};
	/**
	 * offset 120
	 */
	uint32_t startStopStateToggleCounter = (uint32_t)0;
	/**
	 * offset 124
	 */
	float egtValue1 = (float)0;
	/**
	 * offset 128
	 */
	float egtValue2 = (float)0;
	/**
	 * User-defined RPM hard limit
	rpm
	 * offset 132
	 */
	int16_t desiredRpmLimit = (int16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 134
	 */
	uint8_t alignmentFill_at_134[2];
	/**
	 * offset 136
	 */
	uint32_t fuelInjectionCounter = (uint32_t)0;
	/**
	 * offset 140
	 */
	uint32_t sparkCounter = (uint32_t)0;
	/**
	 * @@GAUGE_NAME_FUEL_LOAD@@
	 * offset 144
	 */
	float fuelingLoad = (float)0;
	/**
	 * @@GAUGE_NAME_IGNITION_LOAD@@
	 * offset 148
	 */
	float ignitionLoad = (float)0;
	/**
	%
	 * offset 152
	 */
	scaled_channel<uint16_t, 100, 1> veTableYAxis = (uint16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 154
	 */
	uint8_t alignmentFill_at_154[2];
};
static_assert(sizeof(engine_state_s) == 156);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/engine_state.txt Tue Nov 15 14:33:13 UTC 2022
