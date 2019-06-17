// this section was generated automatically by ConfigDefinition.jar based on integration/engine_state.txt Sun Jun 16 23:32:38 EDT 2019
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
	/** total size 24*/
};

typedef struct engine_state2_s engine_state2_s;

#endif
// end
// this section was generated automatically by ConfigDefinition.jar based on integration/engine_state.txt Sun Jun 16 23:32:38 EDT 2019
