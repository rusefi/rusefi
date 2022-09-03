// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/engine_state.txt Sat Sep 03 21:34:04 UTC 2022
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
	bool unusedBit_5_3 : 1 {};
	/**
	offset 8 bit 4 */
	bool unusedBit_5_4 : 1 {};
	/**
	offset 8 bit 5 */
	bool unusedBit_5_5 : 1 {};
	/**
	offset 8 bit 6 */
	bool unusedBit_5_6 : 1 {};
	/**
	offset 8 bit 7 */
	bool unusedBit_5_7 : 1 {};
	/**
	offset 8 bit 8 */
	bool unusedBit_5_8 : 1 {};
	/**
	offset 8 bit 9 */
	bool unusedBit_5_9 : 1 {};
	/**
	offset 8 bit 10 */
	bool unusedBit_5_10 : 1 {};
	/**
	offset 8 bit 11 */
	bool unusedBit_5_11 : 1 {};
	/**
	offset 8 bit 12 */
	bool unusedBit_5_12 : 1 {};
	/**
	offset 8 bit 13 */
	bool unusedBit_5_13 : 1 {};
	/**
	offset 8 bit 14 */
	bool unusedBit_5_14 : 1 {};
	/**
	offset 8 bit 15 */
	bool unusedBit_5_15 : 1 {};
	/**
	offset 8 bit 16 */
	bool unusedBit_5_16 : 1 {};
	/**
	offset 8 bit 17 */
	bool unusedBit_5_17 : 1 {};
	/**
	offset 8 bit 18 */
	bool unusedBit_5_18 : 1 {};
	/**
	offset 8 bit 19 */
	bool unusedBit_5_19 : 1 {};
	/**
	offset 8 bit 20 */
	bool unusedBit_5_20 : 1 {};
	/**
	offset 8 bit 21 */
	bool unusedBit_5_21 : 1 {};
	/**
	offset 8 bit 22 */
	bool unusedBit_5_22 : 1 {};
	/**
	offset 8 bit 23 */
	bool unusedBit_5_23 : 1 {};
	/**
	offset 8 bit 24 */
	bool unusedBit_5_24 : 1 {};
	/**
	offset 8 bit 25 */
	bool unusedBit_5_25 : 1 {};
	/**
	offset 8 bit 26 */
	bool unusedBit_5_26 : 1 {};
	/**
	offset 8 bit 27 */
	bool unusedBit_5_27 : 1 {};
	/**
	offset 8 bit 28 */
	bool unusedBit_5_28 : 1 {};
	/**
	offset 8 bit 29 */
	bool unusedBit_5_29 : 1 {};
	/**
	offset 8 bit 30 */
	bool unusedBit_5_30 : 1 {};
	/**
	offset 8 bit 31 */
	bool unusedBit_5_31 : 1 {};
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
	 * Actual injection duration based on all above coefficients.
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
	int currentTpsAdc = (int)0;
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
	cranking_fuel_s cranking;
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
	 * ignition dwell duration as crankshaft angle
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
	bool unusedBit_21_3 : 1 {};
	/**
	offset 116 bit 4 */
	bool unusedBit_21_4 : 1 {};
	/**
	offset 116 bit 5 */
	bool unusedBit_21_5 : 1 {};
	/**
	offset 116 bit 6 */
	bool unusedBit_21_6 : 1 {};
	/**
	offset 116 bit 7 */
	bool unusedBit_21_7 : 1 {};
	/**
	offset 116 bit 8 */
	bool unusedBit_21_8 : 1 {};
	/**
	offset 116 bit 9 */
	bool unusedBit_21_9 : 1 {};
	/**
	offset 116 bit 10 */
	bool unusedBit_21_10 : 1 {};
	/**
	offset 116 bit 11 */
	bool unusedBit_21_11 : 1 {};
	/**
	offset 116 bit 12 */
	bool unusedBit_21_12 : 1 {};
	/**
	offset 116 bit 13 */
	bool unusedBit_21_13 : 1 {};
	/**
	offset 116 bit 14 */
	bool unusedBit_21_14 : 1 {};
	/**
	offset 116 bit 15 */
	bool unusedBit_21_15 : 1 {};
	/**
	offset 116 bit 16 */
	bool unusedBit_21_16 : 1 {};
	/**
	offset 116 bit 17 */
	bool unusedBit_21_17 : 1 {};
	/**
	offset 116 bit 18 */
	bool unusedBit_21_18 : 1 {};
	/**
	offset 116 bit 19 */
	bool unusedBit_21_19 : 1 {};
	/**
	offset 116 bit 20 */
	bool unusedBit_21_20 : 1 {};
	/**
	offset 116 bit 21 */
	bool unusedBit_21_21 : 1 {};
	/**
	offset 116 bit 22 */
	bool unusedBit_21_22 : 1 {};
	/**
	offset 116 bit 23 */
	bool unusedBit_21_23 : 1 {};
	/**
	offset 116 bit 24 */
	bool unusedBit_21_24 : 1 {};
	/**
	offset 116 bit 25 */
	bool unusedBit_21_25 : 1 {};
	/**
	offset 116 bit 26 */
	bool unusedBit_21_26 : 1 {};
	/**
	offset 116 bit 27 */
	bool unusedBit_21_27 : 1 {};
	/**
	offset 116 bit 28 */
	bool unusedBit_21_28 : 1 {};
	/**
	offset 116 bit 29 */
	bool unusedBit_21_29 : 1 {};
	/**
	offset 116 bit 30 */
	bool unusedBit_21_30 : 1 {};
	/**
	offset 116 bit 31 */
	bool unusedBit_21_31 : 1 {};
	/**
	 * offset 120
	 */
	float egt1 = (float)0;
	/**
	 * offset 124
	 */
	float egt2 = (float)0;
	/**
	 * User-defined RPM hard limit
	rpm
	 * offset 128
	 */
	int16_t desiredRpmLimit = (int16_t)0;
	/**
	 * need 4 byte alignment
	units
	 * offset 130
	 */
	uint8_t alignmentFill_at_130[2];
	/**
	 * offset 132
	 */
	int fuelInjectionCounter = (int)0;
	/**
	 * offset 136
	 */
	int sparkCounter = (int)0;
};
static_assert(sizeof(engine_state_s) == 140);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/engine_state.txt Sat Sep 03 21:34:04 UTC 2022
