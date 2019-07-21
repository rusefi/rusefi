// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/engine_state.txt Sun Jul 21 18:03:23 EDT 2019
// by class com.rusefi.output.CHeaderConsumer
// begin
#ifndef CONTROLLERS_GENERATED_ENGINE_STATE_GENERATED_H
#define CONTROLLERS_GENERATED_ENGINE_STATE_GENERATED_H
#include "rusefi_types.h"
// start of pid_state_s
struct pid_state_s {
	/**
	 * offset 0
	 */
	float iTerm = (float)0;
	/**
	 * we are remembering this only for debugging purposes
	 * offset 4
	 */
	float dTerm = (float)0;
	/**
	 * only used for logging
	 * offset 8
	 */
	float target = (float)0;
	/**
	 * only used for logging
	 * offset 12
	 */
	float input = (float)0;
	/**
	 * only used for logging
	 * offset 16
	 */
	float output = (float)0;
	/** total size 20*/
};

typedef struct pid_state_s pid_state_s;

// start of speed_density_s
struct speed_density_s {
	/**
	offset 0 bit 0 */
	bool isTChargeAirModel : 1;
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
	/**
	 * offset 24
	 */
	float manifoldAirPressureAccelerationAdjustment = (float)0;
	/**
	 * offset 28
	 */
	float adjustedManifoldAirPressure = (float)0;
	/** total size 32*/
};

typedef struct speed_density_s speed_density_s;

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
	 * offset 12
	 */
	int throttleUpState = (int)0;
	/** total size 16*/
};

typedef struct idle_state_s idle_state_s;

// start of engine_state2_s
struct engine_state2_s {
	/**
	 * offset 0
	 */
	speed_density_s sd;
	/**
	 * offset 32
	 */
	idle_state_s idle;
	/**
	 * offset 48
	 */
	float targetAFR = (float)0;
	/**
	 * offset 52
	 */
	float engineCycleDurationMs = (float)0;
	/**
	 * offset 56
	 */
	float minRpmKcurrentTPS = (float)0;
	/**
	 * offset 60
	 */
	int currentTpsAdc = (int)0;
	/**
	 * offset 64
	 */
	float tpsVoltageMCU = (float)0;
	/**
	 * offset 68
	 */
	float tpsVoltageBoard = (float)0;
	/**
	 * offset 72
	 */
	float currentBaroCorrectedVE = (float)0;
	/** total size 76*/
};

typedef struct engine_state2_s engine_state2_s;

#endif
// end
// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/engine_state.txt Sun Jul 21 18:03:23 EDT 2019
