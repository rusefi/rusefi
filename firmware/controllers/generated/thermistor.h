// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/thermistor.txt Mon Jun 17 20:32:33 EDT 2019
// begin
#ifndef CONTROLLERS_GENERATED_THERMISTOR_H
#define CONTROLLERS_GENERATED_THERMISTOR_H
#include "rusefi_types.h"
// start of thermistor_state_s
struct thermistor_state_s {
	/**
	 * offset 0
	 */
	float resistance = 0;
	/**
	 * offset 4
	 */
	float voltageMCU = 0;
	/**
	 * offset 8
	 */
	float voltageBoard = 0;
	/** total size 12*/
};

typedef struct thermistor_state_s thermistor_state_s;

#endif
// end
// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/thermistor.txt Mon Jun 17 20:32:33 EDT 2019
