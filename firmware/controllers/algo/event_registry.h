/**
 * @file	event_registry.h
 *
 * @date Nov 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef EVENT_REGISTRY_H_
#define EVENT_REGISTRY_H_

#include "global.h"
#include "signal_executor.h"
#include "trigger_structure.h"

#define MAX_EVENT_COUNT 40

typedef struct {
	int eventIndex;
	OutputSignal *actuator;
	float angleOffset;
} ActuatorEvent;

typedef struct {
	int size;
	ActuatorEvent events[MAX_EVENT_COUNT];
} ActuatorEventList;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


void resetEventList(ActuatorEventList *list);

/**
 * this is an intermediate implementation of flexible event handling.
 *
 * In the future implementation we will drop the 'eventIndex' parameter and everything will be
 * angle-driven. But that's just a plan for next iteration.
 *
 * @param	actuator injector or coil OutputSignal
 */
void registerActuatorEvent(ActuatorEventList *list, int eventIndex, OutputSignal *actuator, float angleOffset);

void findEvents(int eventIndex, ActuatorEventList *source, ActuatorEventList *target);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* EVENT_REGISTRY_H_ */
