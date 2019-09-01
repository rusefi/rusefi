// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/pid_state.txt Sun Sep 01 11:57:50 EDT 2019
// by class com.rusefi.output.CHeaderConsumer
// begin
#ifndef CONTROLLERS_GENERATED_PID_STATE_GENERATED_H
#define CONTROLLERS_GENERATED_PID_STATE_GENERATED_H
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
	/**
	 * offset 20
	 */
	float errorAmplificationCoef = (float)0;
	/** total size 24*/
};

typedef struct pid_state_s pid_state_s;

#endif
// end
// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/pid_state.txt Sun Sep 01 11:57:50 EDT 2019
