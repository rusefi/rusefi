// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/algo/fuel/injector_model.txt Wed Jan 03 20:30:10 UTC 2024
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of injector_model_s
struct injector_model_s {
	/**
	 * @@GAUGE_NAME_INJECTOR_LAG@@
	 * units: ms
	 * offset 0
	 */
	float m_deadtime = (float)0;
	/**
	 * Fuel: Injector pressure delta
	 * units: kPa
	 * offset 4
	 */
	float pressureDelta = (float)0;
	/**
	 * Fuel: Injector pressure ratio
	 * offset 8
	 */
	float pressureRatio = (float)0;
};
static_assert(sizeof(injector_model_s) == 12);

// end
// this section was generated automatically by rusEFI tool config_definition_base.jar based on (unknown script) controllers/algo/fuel/injector_model.txt Wed Jan 03 20:30:10 UTC 2024
