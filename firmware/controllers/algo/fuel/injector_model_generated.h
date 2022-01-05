// this section null
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of injector_model_s
struct injector_model_s {
	/**
	 * offset 0
	 */
	float m_deadtime = (float)0;
	/**
	 * offset 4
	 */
	float pressureDelta = (float)0;
	/**
	 * offset 8
	 */
	float pressureRatio = (float)0;
	/** total size 12*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/algo/fuel/injector_model.txt Wed Jan 05 05:06:31 UTC 2022
