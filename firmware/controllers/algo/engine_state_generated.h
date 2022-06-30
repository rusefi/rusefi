// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/engine_state.txt Wed Jun 29 22:14:57 UTC 2022
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
	offset 4 bit 0 */
	bool clutchUpState : 1 {};
	/**
	offset 4 bit 1 */
	bool brakePedalState : 1 {};
	/**
	offset 4 bit 2 */
	bool unusedBit_3_2 : 1 {};
	/**
	offset 4 bit 3 */
	bool unusedBit_3_3 : 1 {};
	/**
	offset 4 bit 4 */
	bool unusedBit_3_4 : 1 {};
	/**
	offset 4 bit 5 */
	bool unusedBit_3_5 : 1 {};
	/**
	offset 4 bit 6 */
	bool unusedBit_3_6 : 1 {};
	/**
	offset 4 bit 7 */
	bool unusedBit_3_7 : 1 {};
	/**
	offset 4 bit 8 */
	bool unusedBit_3_8 : 1 {};
	/**
	offset 4 bit 9 */
	bool unusedBit_3_9 : 1 {};
	/**
	offset 4 bit 10 */
	bool unusedBit_3_10 : 1 {};
	/**
	offset 4 bit 11 */
	bool unusedBit_3_11 : 1 {};
	/**
	offset 4 bit 12 */
	bool unusedBit_3_12 : 1 {};
	/**
	offset 4 bit 13 */
	bool unusedBit_3_13 : 1 {};
	/**
	offset 4 bit 14 */
	bool unusedBit_3_14 : 1 {};
	/**
	offset 4 bit 15 */
	bool unusedBit_3_15 : 1 {};
	/**
	offset 4 bit 16 */
	bool unusedBit_3_16 : 1 {};
	/**
	offset 4 bit 17 */
	bool unusedBit_3_17 : 1 {};
	/**
	offset 4 bit 18 */
	bool unusedBit_3_18 : 1 {};
	/**
	offset 4 bit 19 */
	bool unusedBit_3_19 : 1 {};
	/**
	offset 4 bit 20 */
	bool unusedBit_3_20 : 1 {};
	/**
	offset 4 bit 21 */
	bool unusedBit_3_21 : 1 {};
	/**
	offset 4 bit 22 */
	bool unusedBit_3_22 : 1 {};
	/**
	offset 4 bit 23 */
	bool unusedBit_3_23 : 1 {};
	/**
	offset 4 bit 24 */
	bool unusedBit_3_24 : 1 {};
	/**
	offset 4 bit 25 */
	bool unusedBit_3_25 : 1 {};
	/**
	offset 4 bit 26 */
	bool unusedBit_3_26 : 1 {};
	/**
	offset 4 bit 27 */
	bool unusedBit_3_27 : 1 {};
	/**
	offset 4 bit 28 */
	bool unusedBit_3_28 : 1 {};
	/**
	offset 4 bit 29 */
	bool unusedBit_3_29 : 1 {};
	/**
	offset 4 bit 30 */
	bool unusedBit_3_30 : 1 {};
	/**
	offset 4 bit 31 */
	bool unusedBit_3_31 : 1 {};
};
static_assert(sizeof(LuaAdjustments) == 8);

// start of speed_density_s
struct speed_density_s {
	/**
	offset 0 bit 0 */
	bool isTChargeAirModel : 1 {};
	/**
	offset 0 bit 1 */
	bool unusedBit_1_1 : 1 {};
	/**
	offset 0 bit 2 */
	bool unusedBit_1_2 : 1 {};
	/**
	offset 0 bit 3 */
	bool unusedBit_1_3 : 1 {};
	/**
	offset 0 bit 4 */
	bool unusedBit_1_4 : 1 {};
	/**
	offset 0 bit 5 */
	bool unusedBit_1_5 : 1 {};
	/**
	offset 0 bit 6 */
	bool unusedBit_1_6 : 1 {};
	/**
	offset 0 bit 7 */
	bool unusedBit_1_7 : 1 {};
	/**
	offset 0 bit 8 */
	bool unusedBit_1_8 : 1 {};
	/**
	offset 0 bit 9 */
	bool unusedBit_1_9 : 1 {};
	/**
	offset 0 bit 10 */
	bool unusedBit_1_10 : 1 {};
	/**
	offset 0 bit 11 */
	bool unusedBit_1_11 : 1 {};
	/**
	offset 0 bit 12 */
	bool unusedBit_1_12 : 1 {};
	/**
	offset 0 bit 13 */
	bool unusedBit_1_13 : 1 {};
	/**
	offset 0 bit 14 */
	bool unusedBit_1_14 : 1 {};
	/**
	offset 0 bit 15 */
	bool unusedBit_1_15 : 1 {};
	/**
	offset 0 bit 16 */
	bool unusedBit_1_16 : 1 {};
	/**
	offset 0 bit 17 */
	bool unusedBit_1_17 : 1 {};
	/**
	offset 0 bit 18 */
	bool unusedBit_1_18 : 1 {};
	/**
	offset 0 bit 19 */
	bool unusedBit_1_19 : 1 {};
	/**
	offset 0 bit 20 */
	bool unusedBit_1_20 : 1 {};
	/**
	offset 0 bit 21 */
	bool unusedBit_1_21 : 1 {};
	/**
	offset 0 bit 22 */
	bool unusedBit_1_22 : 1 {};
	/**
	offset 0 bit 23 */
	bool unusedBit_1_23 : 1 {};
	/**
	offset 0 bit 24 */
	bool unusedBit_1_24 : 1 {};
	/**
	offset 0 bit 25 */
	bool unusedBit_1_25 : 1 {};
	/**
	offset 0 bit 26 */
	bool unusedBit_1_26 : 1 {};
	/**
	offset 0 bit 27 */
	bool unusedBit_1_27 : 1 {};
	/**
	offset 0 bit 28 */
	bool unusedBit_1_28 : 1 {};
	/**
	offset 0 bit 29 */
	bool unusedBit_1_29 : 1 {};
	/**
	offset 0 bit 30 */
	bool unusedBit_1_30 : 1 {};
	/**
	offset 0 bit 31 */
	bool unusedBit_1_31 : 1 {};
	/**
	 * Speed-density logic: calculated air mass in one cylinder, in grams
	 * offset 4
	 */
	float airMassInOneCylinder = (float)0;
	/**
	 * speed density
	 * Rate-of-change limiter is applied to degrees, so we store both Kelvin and degrees.
	 * offset 8
	 */
	float tCharge = (float)0;
	/**
	 * offset 12
	 */
	float tChargeK = (float)0;
	/**
	 * offset 16
	 */
	float Tcharge_coff = (float)0;
	/**
	 * offset 20
	 */
	floatms_t airFlow = (floatms_t)0;
};
static_assert(sizeof(speed_density_s) == 24);

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
	 * offset 4
	 */
	float intakeTemperatureCoefficient = (float)0;
	/**
	 * offset 8
	 */
	float coolantTemperatureCoefficient = (float)0;
	/**
	 * offset 12
	 */
	float timeSinceCrankingInSecs = (float)0;
	/**
	 * injectorLag(VBatt)
	 * this value depends on a slow-changing VBatt value, so
	 * we update it once in a while
	 * offset 16
	 */
	floatms_t injectorLag = (floatms_t)0;
	/**
	 * offset 20
	 */
	floatms_t baseFuel = (floatms_t)0;
	/**
	 * Actual injection duration with CLT, IAT and TPS acceleration corrections per cycle, as squirt duration.
	 * Without injector lag.
	 * @see baseFuel
	 * @see actualLastInjection
	 * offset 24
	 */
	floatms_t fuel = (floatms_t)0;
};
static_assert(sizeof(running_fuel_s) == 28);

// start of engine_state_s
struct engine_state_s {
	/**
	 * offset 0
	 */
	LuaAdjustments lua;
	/**
	 * offset 8
	 */
	speed_density_s sd;
	/**
	 * offset 32
	 */
	float targetAFR = (float)0;
	/**
	 * offset 36
	 */
	float engineCycleDurationMs = (float)0;
	/**
	 * offset 40
	 */
	float minRpmKcurrentTPS = (float)0;
	/**
	 * offset 44
	 */
	int currentTpsAdc = (int)0;
	/**
	 * offset 48
	 */
	float tpsVoltageMCU = (float)0;
	/**
	 * offset 52
	 */
	float tpsVoltageBoard = (float)0;
	/**
	 * offset 56
	 */
	float currentBaroCorrectedVE = (float)0;
	/**
	 * offset 60
	 */
	float injectorFlowCorrection = (float)0;
	/**
	 * offset 64
	 */
	float baroCorrection = (float)0;
	/**
	 * offset 68
	 */
	cranking_fuel_s cranking;
	/**
	 * offset 88
	 */
	running_fuel_s running;
	/**
	 * ignition dwell duration in ms
	 * See also dwellAngle
	 * offset 116
	 */
	floatms_t sparkDwell = (floatms_t)0;
	/**
	 * ignition dwell duration as crankshaft angle
	 * NAN if engine is stopped
	 * See also sparkDwell
	 * offset 120
	 */
	angle_t dwellAngle = (angle_t)0;
	/**
	deg
	 * offset 124
	 */
	scaled_channel<int16_t, 100, 1> cltTimingCorrection = (int16_t)0;
	/**
	deg
	 * offset 126
	 */
	scaled_channel<int16_t, 100, 1> timingIatCorrection = (int16_t)0;
	/**
	deg
	 * offset 128
	 */
	scaled_channel<int16_t, 100, 1> timingPidCorrection = (int16_t)0;
	/**
	 * Detected Board ID
	id
	 * offset 130
	 */
	int16_t hellenBoardId = (int16_t)0;
	/**
	 * @@INDICATOR_NAME_CLUTCH_UP@@
	offset 132 bit 0 */
	bool clutchUpState : 1 {};
	/**
	 * @@INDICATOR_NAME_CLUTCH_DOWN@@
	offset 132 bit 1 */
	bool clutchDownState : 1 {};
	/**
	 * @@INDICATOR_NAME_BRAKE_DOWN@@
	offset 132 bit 2 */
	bool brakePedalState : 1 {};
	/**
	offset 132 bit 3 */
	bool unusedBit_22_3 : 1 {};
	/**
	offset 132 bit 4 */
	bool unusedBit_22_4 : 1 {};
	/**
	offset 132 bit 5 */
	bool unusedBit_22_5 : 1 {};
	/**
	offset 132 bit 6 */
	bool unusedBit_22_6 : 1 {};
	/**
	offset 132 bit 7 */
	bool unusedBit_22_7 : 1 {};
	/**
	offset 132 bit 8 */
	bool unusedBit_22_8 : 1 {};
	/**
	offset 132 bit 9 */
	bool unusedBit_22_9 : 1 {};
	/**
	offset 132 bit 10 */
	bool unusedBit_22_10 : 1 {};
	/**
	offset 132 bit 11 */
	bool unusedBit_22_11 : 1 {};
	/**
	offset 132 bit 12 */
	bool unusedBit_22_12 : 1 {};
	/**
	offset 132 bit 13 */
	bool unusedBit_22_13 : 1 {};
	/**
	offset 132 bit 14 */
	bool unusedBit_22_14 : 1 {};
	/**
	offset 132 bit 15 */
	bool unusedBit_22_15 : 1 {};
	/**
	offset 132 bit 16 */
	bool unusedBit_22_16 : 1 {};
	/**
	offset 132 bit 17 */
	bool unusedBit_22_17 : 1 {};
	/**
	offset 132 bit 18 */
	bool unusedBit_22_18 : 1 {};
	/**
	offset 132 bit 19 */
	bool unusedBit_22_19 : 1 {};
	/**
	offset 132 bit 20 */
	bool unusedBit_22_20 : 1 {};
	/**
	offset 132 bit 21 */
	bool unusedBit_22_21 : 1 {};
	/**
	offset 132 bit 22 */
	bool unusedBit_22_22 : 1 {};
	/**
	offset 132 bit 23 */
	bool unusedBit_22_23 : 1 {};
	/**
	offset 132 bit 24 */
	bool unusedBit_22_24 : 1 {};
	/**
	offset 132 bit 25 */
	bool unusedBit_22_25 : 1 {};
	/**
	offset 132 bit 26 */
	bool unusedBit_22_26 : 1 {};
	/**
	offset 132 bit 27 */
	bool unusedBit_22_27 : 1 {};
	/**
	offset 132 bit 28 */
	bool unusedBit_22_28 : 1 {};
	/**
	offset 132 bit 29 */
	bool unusedBit_22_29 : 1 {};
	/**
	offset 132 bit 30 */
	bool unusedBit_22_30 : 1 {};
	/**
	offset 132 bit 31 */
	bool unusedBit_22_31 : 1 {};
};
static_assert(sizeof(engine_state_s) == 136);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/engine_state.txt Wed Jun 29 22:14:57 UTC 2022
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/engine_state.txt Wed Jun 29 22:24:49 EDT 2022
