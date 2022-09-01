// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/injector_model.txt Thu Sep 01 15:02:30 UTC 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of injector_model_s
struct injector_model_s {
	/**
	 * injectorLag(VBatt)
	 * this value depends on a slow-changing VBatt value, so
	 * we update it once in a while
	 * offset 0
	 */
	float m_deadtime = (float)0;
	/**
	 * fuel: injector pressureDelta
	kPa
	 * offset 4
	 */
	float pressureDelta = (float)0;
	/**
	 * fuel: injector pressureRatio
	 * offset 8
	 */
	float pressureRatio = (float)0;
};
static_assert(sizeof(injector_model_s) == 12);

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/injector_model.txt Thu Sep 01 15:02:30 UTC 2022
