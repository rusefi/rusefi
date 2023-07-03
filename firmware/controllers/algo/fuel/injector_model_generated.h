// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/algo/fuel/injector_model.txt Mon Jul 03 16:38:04 UTC 2023
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of injector_model_s
struct injector_model_s {
	/**
	 * @@GAUGE_NAME_INJECTOR_LAG@@
	ms
	 * offset 0
	 */
	float m_deadtime = (float)0;
	/**
	 * fuel: Injector pressure delta
	kPa
	 * offset 4
	 */
	float pressureDelta = (float)0;
	/**
	 * fuel: Injector pressure ratio
	 * offset 8
	 */
	float pressureRatio = (float)0;
};
static_assert(sizeof(injector_model_s) == 12);

// end
// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/algo/fuel/injector_model.txt Mon Jul 03 16:38:04 UTC 2023
