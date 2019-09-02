// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/trigger_state.txt Mon Sep 02 13:51:51 EDT 2019
// by class com.rusefi.output.CHeaderConsumer
// begin
#ifndef CONTROLLERS_GENERATED_TRIGGER_GENERATED_H
#define CONTROLLERS_GENERATED_TRIGGER_GENERATED_H
#include "rusefi_types.h"
#define HW_EVENT_TYPES 6
// start of trigger_central_s
struct trigger_central_s {
	trigger_central_s();
	/**
	 * offset 0
	 */
	int hwEventCounters[HW_EVENT_TYPES];
	/**
	 * offset 24
	 */
	int vvtCamCounter = (int)0;
	/** total size 28*/
};

typedef struct trigger_central_s trigger_central_s;

#endif
// end
// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/trigger_state.txt Mon Sep 02 13:51:51 EDT 2019
