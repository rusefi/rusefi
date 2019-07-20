// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/trigger.txt Sat Jul 20 12:29:51 EDT 2019
// by class com.rusefi.output.CHeaderConsumer
// begin
#ifndef CONTROLLERS_GENERATED_TRIGGER_STRUCTS_H
#define CONTROLLERS_GENERATED_TRIGGER_STRUCTS_H
#include "rusefi_types.h"
#define HW_EVENT_TYPES 6
// start of trigger_central_s
struct trigger_central_s {
	trigger_central_s();
	/**
	 * offset 0
	 */
	int hwEventCounters[HW_EVENT_TYPES];
	/** total size 24*/
};

typedef struct trigger_central_s trigger_central_s;

#endif
// end
// this section was generated automatically by rusEfi tool ConfigDefinition.jar based on integration/trigger.txt Sat Jul 20 12:29:51 EDT 2019
