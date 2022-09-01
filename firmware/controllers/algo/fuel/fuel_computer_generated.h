// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/fuel_computer.txt Thu Sep 01 23:17:10 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of fuel_computer_s
struct fuel_computer_s {
	/**
	 * offset 0
	 */
	float currentAfrLoad = (float)0;
	/**
	 * offset 4
	 */
	float targetLambda = (float)0;
	/**
	 * offset 8
	 */
	float stoichiometricRatio = (float)0;
	/**
	 * offset 12
	 */
	float targetAFR_t = (float)0;
};
static_assert(sizeof(fuel_computer_s) == 16);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/fuel_computer.txt Thu Sep 01 23:17:10 UTC 2022
