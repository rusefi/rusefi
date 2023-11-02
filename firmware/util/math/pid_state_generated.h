// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) util/math/pid_state.txt Wed Jan 05 06:47:26 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
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
	float lastTarget = (float)0;
	/**
	 * only used for logging
	 * offset 12
	 */
	float lastInput = (float)0;
	/**
	 * only used for logging
	 * offset 16
	 */
	float lastOutput = (float)0;
	/**
	 * offset 20
	 */
	float errorAmplificationCoef = (float)0;
	/**
	 * offset 24
	 */
	float previousError = (float)0;
	/** total size 28*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) util/math/pid_state.txt Wed Jan 05 06:47:26 UTC 2022
