// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/engine_state.txt Mon Jun 17 20:32:33 EDT 2019
// begin
#ifndef CONTROLLERS_GENERATED_ENGINE_STATE_GENERATED_H
#define CONTROLLERS_GENERATED_ENGINE_STATE_GENERATED_H
#include "rusefi_types.h"
// start of engine_state2_s
struct engine_state2_s {
	/**
	offset 0 bit 0 */
	bool isTChargeAirModel : 1;
	/**
	 * speed-density logic, calculated air mass in grams
	 * offset 4
	 */
	float airMass = 0;
	/**
	 * offset 8
	 */
	float engineCycleDurationMs = 0;
	/**
	 * offset 12
	 */
	float Tcharge_coff = 0;
	/**
	 * offset 16
	 */
	floatms_t airFlow = 0;
	/**
	 * offset 20
	 */
	float minRpmKcurrentTPS = 0;
	/**
	 * offset 24
	 */
	int currentTpsAdc = 0;
	/**
	 * offset 28
	 */
	float tpsVoltageMCU = 0;
	/**
	 * offset 32
	 */
	float tpsVoltageBoard = 0;
	/** total size 36*/
};

typedef struct engine_state2_s engine_state2_s;

#endif
// end
// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/engine_state.txt Mon Jun 17 20:32:33 EDT 2019
