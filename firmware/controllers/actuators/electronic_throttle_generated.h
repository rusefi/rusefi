// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/electronic_throttle.txt Sun Apr 17 20:27:25 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of electronic_throttle_s
struct electronic_throttle_s {
	/**
	 * offset 0
	 */
	float idlePosition = (float)0;
	/**
	 * offset 4
	 */
	float trim = (float)0;
	/**
	 * offset 8
	 */
	float luaAdjustment = (float)0;
};
static_assert(sizeof(electronic_throttle_s) == 12);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/actuators/electronic_throttle.txt Sun Apr 17 20:27:25 UTC 2022
