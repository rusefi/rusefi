// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/engine_state.txt Sat Oct 16 21:46:56 EDT 2021
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of speed_density_s
struct speed_density_s {
	/**
	offset 0 bit 0 */
	bool isTChargeAirModel : 1;
	/**
	offset 0 bit 1 */
	bool unusedBit_1_1 : 1;
	/**
	offset 0 bit 2 */
	bool unusedBit_1_2 : 1;
	/**
	offset 0 bit 3 */
	bool unusedBit_1_3 : 1;
	/**
	offset 0 bit 4 */
	bool unusedBit_1_4 : 1;
	/**
	offset 0 bit 5 */
	bool unusedBit_1_5 : 1;
	/**
	offset 0 bit 6 */
	bool unusedBit_1_6 : 1;
	/**
	offset 0 bit 7 */
	bool unusedBit_1_7 : 1;
	/**
	offset 0 bit 8 */
	bool unusedBit_1_8 : 1;
	/**
	offset 0 bit 9 */
	bool unusedBit_1_9 : 1;
	/**
	offset 0 bit 10 */
	bool unusedBit_1_10 : 1;
	/**
	offset 0 bit 11 */
	bool unusedBit_1_11 : 1;
	/**
	offset 0 bit 12 */
	bool unusedBit_1_12 : 1;
	/**
	offset 0 bit 13 */
	bool unusedBit_1_13 : 1;
	/**
	offset 0 bit 14 */
	bool unusedBit_1_14 : 1;
	/**
	offset 0 bit 15 */
	bool unusedBit_1_15 : 1;
	/**
	offset 0 bit 16 */
	bool unusedBit_1_16 : 1;
	/**
	offset 0 bit 17 */
	bool unusedBit_1_17 : 1;
	/**
	offset 0 bit 18 */
	bool unusedBit_1_18 : 1;
	/**
	offset 0 bit 19 */
	bool unusedBit_1_19 : 1;
	/**
	offset 0 bit 20 */
	bool unusedBit_1_20 : 1;
	/**
	offset 0 bit 21 */
	bool unusedBit_1_21 : 1;
	/**
	offset 0 bit 22 */
	bool unusedBit_1_22 : 1;
	/**
	offset 0 bit 23 */
	bool unusedBit_1_23 : 1;
	/**
	offset 0 bit 24 */
	bool unusedBit_1_24 : 1;
	/**
	offset 0 bit 25 */
	bool unusedBit_1_25 : 1;
	/**
	offset 0 bit 26 */
	bool unusedBit_1_26 : 1;
	/**
	offset 0 bit 27 */
	bool unusedBit_1_27 : 1;
	/**
	offset 0 bit 28 */
	bool unusedBit_1_28 : 1;
	/**
	offset 0 bit 29 */
	bool unusedBit_1_29 : 1;
	/**
	offset 0 bit 30 */
	bool unusedBit_1_30 : 1;
	/**
	offset 0 bit 31 */
	bool unusedBit_1_31 : 1;
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
	/** total size 24*/
};

// start of idle_state_s
struct idle_state_s {
	/**
	 * offset 0
	 */
	idle_state_e idleState = (idle_state_e)0;
	/**
	 * that's current position with CLT and IAT corrections
	 * offset 4
	 */
	percent_t currentIdlePosition = (percent_t)0;
	/**
	 * current position without adjustments (iacByTpsTaper, afterCrankingIACtaperDuration)
	 * offset 8
	 */
	percent_t baseIdlePosition = (percent_t)0;
	/**
	 * true in IDLE throttle pedal state, false if driver is touching the pedal
	 * todo: better name for this field?
	 * offset 12
	 */
	int throttlePedalUpState = (int)0;
	/**
	 * ETB position adjustment related to idle RPM control
	 * offset 16
	 */
	percent_t etbIdleAddition = (percent_t)0;
	/** total size 20*/
};

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
	/** total size 20*/
};

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
	/** total size 28*/
};

// start of engine_state2_s
struct engine_state2_s {
	/**
	 * offset 0
	 */
	speed_density_s sd;
	/**
	 * offset 24
	 */
	idle_state_s idle;
	/**
	 * offset 44
	 */
	float targetAFR = (float)0;
	/**
	 * offset 48
	 */
	float engineCycleDurationMs = (float)0;
	/**
	 * offset 52
	 */
	float minRpmKcurrentTPS = (float)0;
	/**
	 * offset 56
	 */
	int currentTpsAdc = (int)0;
	/**
	 * offset 60
	 */
	float tpsVoltageMCU = (float)0;
	/**
	 * offset 64
	 */
	float tpsVoltageBoard = (float)0;
	/**
	 * offset 68
	 */
	float currentBaroCorrectedVE = (float)0;
	/**
	 * offset 72
	 */
	float injectorFlowCorrection = (float)0;
	/**
	 * offset 76
	 */
	float baroCorrection = (float)0;
	/**
	offset 80 bit 0 */
	bool isCrankingState : 1;
	/**
	offset 80 bit 1 */
	bool isAutomaticIdle : 1;
	/**
	offset 80 bit 2 */
	bool unusedBit_13_2 : 1;
	/**
	offset 80 bit 3 */
	bool unusedBit_13_3 : 1;
	/**
	offset 80 bit 4 */
	bool unusedBit_13_4 : 1;
	/**
	offset 80 bit 5 */
	bool unusedBit_13_5 : 1;
	/**
	offset 80 bit 6 */
	bool unusedBit_13_6 : 1;
	/**
	offset 80 bit 7 */
	bool unusedBit_13_7 : 1;
	/**
	offset 80 bit 8 */
	bool unusedBit_13_8 : 1;
	/**
	offset 80 bit 9 */
	bool unusedBit_13_9 : 1;
	/**
	offset 80 bit 10 */
	bool unusedBit_13_10 : 1;
	/**
	offset 80 bit 11 */
	bool unusedBit_13_11 : 1;
	/**
	offset 80 bit 12 */
	bool unusedBit_13_12 : 1;
	/**
	offset 80 bit 13 */
	bool unusedBit_13_13 : 1;
	/**
	offset 80 bit 14 */
	bool unusedBit_13_14 : 1;
	/**
	offset 80 bit 15 */
	bool unusedBit_13_15 : 1;
	/**
	offset 80 bit 16 */
	bool unusedBit_13_16 : 1;
	/**
	offset 80 bit 17 */
	bool unusedBit_13_17 : 1;
	/**
	offset 80 bit 18 */
	bool unusedBit_13_18 : 1;
	/**
	offset 80 bit 19 */
	bool unusedBit_13_19 : 1;
	/**
	offset 80 bit 20 */
	bool unusedBit_13_20 : 1;
	/**
	offset 80 bit 21 */
	bool unusedBit_13_21 : 1;
	/**
	offset 80 bit 22 */
	bool unusedBit_13_22 : 1;
	/**
	offset 80 bit 23 */
	bool unusedBit_13_23 : 1;
	/**
	offset 80 bit 24 */
	bool unusedBit_13_24 : 1;
	/**
	offset 80 bit 25 */
	bool unusedBit_13_25 : 1;
	/**
	offset 80 bit 26 */
	bool unusedBit_13_26 : 1;
	/**
	offset 80 bit 27 */
	bool unusedBit_13_27 : 1;
	/**
	offset 80 bit 28 */
	bool unusedBit_13_28 : 1;
	/**
	offset 80 bit 29 */
	bool unusedBit_13_29 : 1;
	/**
	offset 80 bit 30 */
	bool unusedBit_13_30 : 1;
	/**
	offset 80 bit 31 */
	bool unusedBit_13_31 : 1;
	/**
	 * offset 84
	 */
	cranking_fuel_s cranking;
	/**
	 * offset 104
	 */
	running_fuel_s running;
	/**
	 * offset 132
	 */
	percent_t etbFeedForward = (percent_t)0;
	/**
	 * offset 136
	 */
	percent_t targetFromTable = (percent_t)0;
	/**
	 * ignition dwell duration in ms
	 * See also dwellAngle
	 * offset 140
	 */
	floatms_t sparkDwell = (floatms_t)0;
	/**
	 * ignition dwell duration as crankshaft angle
	 * NAN if engine is stopped
	 * See also sparkDwell
	 * offset 144
	 */
	angle_t dwellAngle = (angle_t)0;
	/**
	 * offset 148
	 */
	angle_t cltTimingCorrection = (angle_t)0;
	/** total size 152*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/engine_state.txt Sat Oct 16 21:46:56 EDT 2021
