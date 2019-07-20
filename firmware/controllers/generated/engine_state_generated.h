// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/engine_state.txt Sat Jul 20 12:28:04 EDT 2019
// by class com.rusefi.output.CHeaderConsumer
// begin
#ifndef CONTROLLERS_GENERATED_ENGINE_STATE_GENERATED_H
#define CONTROLLERS_GENERATED_ENGINE_STATE_GENERATED_H
#include "rusefi_types.h"
// start of speed_density_s
struct speed_density_s {
	/**
	offset 0 bit 0 */
	bool isTChargeAirModel : 1;
	/**
	 * Speed-density logic: calculated air mass in one cylinder, in grams
	 * offset 4
	 */
	float airMassInOneCylinder = 0;
	/**
	 * speed density
	 * Rate-of-change limiter is applied to degrees, so we store both Kelvin and degrees.
	 * offset 8
	 */
	float tCharge = 0;
	/**
	 * offset 12
	 */
	float tChargeK = 0;
	/**
	 * offset 16
	 */
	float Tcharge_coff = 0;
	/**
	 * offset 20
	 */
	floatms_t airFlow = 0;
	/**
	 * offset 24
	 */
	float manifoldAirPressureAccelerationAdjustment = 0;
	/**
	 * offset 28
	 */
	float adjustedManifoldAirPressure = 0;
	/** total size 32*/
};

typedef struct speed_density_s speed_density_s;

// start of engine_state2_s
struct engine_state2_s {
	/**
	 * offset 0
	 */
	speed_density_s sd;
	/**
	 * offset 32
	 */
	float targetAFR = 0;
	/**
	 * offset 36
	 */
	float engineCycleDurationMs = 0;
	/**
	 * offset 40
	 */
	float minRpmKcurrentTPS = 0;
	/**
	 * offset 44
	 */
	int currentTpsAdc = 0;
	/**
	 * offset 48
	 */
	float tpsVoltageMCU = 0;
	/**
	 * offset 52
	 */
	float tpsVoltageBoard = 0;
	/**
	 * offset 56
	 */
	float currentBaroCorrectedVE = 0;
	/** total size 60*/
};

typedef struct engine_state2_s engine_state2_s;

#endif
// end
// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/engine_state.txt Sat Jul 20 12:28:04 EDT 2019
