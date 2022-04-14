// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger\trigger_central.txt Thu Apr 14 15:43:13 EDT 2022
// by class com.rusefi.output.CHeaderConsumer
// begin
#pragma once
#include "rusefi_types.h"
// start of trigger_central_s
struct trigger_central_s {
	/**
	 * Counter of hardware events since ECU start
	 * offset 0
	 */
	int hwEventCounters[HW_EVENT_TYPES];
	/**
	 * offset 24
	 */
	int vvtCamCounter = (int)0;
	/** total size 28*/
};

// end
// this section was generated automatically by rusEFI tool ConfigDefinition.jar based on (unknown script) controllers/trigger\trigger_central.txt Thu Apr 14 15:43:13 EDT 2022
