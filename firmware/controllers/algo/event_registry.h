/**
 * @file	event_registry.h
 *
 * @date Nov 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef EVENT_REGISTRY_H_
#define EVENT_REGISTRY_H_

#include "global.h"
#include "signal_executor.h"
#include "fl_stack.h"

#define MAX_EVENT_COUNT 80

/**
 * This structure defines an angle position within the trigger
 */
class event_trigger_position_s {
public:
	/**
	 * That's trigger event index
	 */
	uint32_t eventIndex;
	angle_t eventAngle;
	/**
	 * Angle offset from the trigger event
	 */
	angle_t angleOffset;
};

typedef struct {
	event_trigger_position_s position;
	OutputSignal *actuator;
	/**
	 * This is a performance optimization - it's more efficient to handle all
	 * injectors together if that's the case
	 */
	bool_t isSimultanious;
} InjectionEvent;

typedef struct IgnitionEvent_struct IgnitionEvent;

struct IgnitionEvent_struct {
	NamedOutputPin *output;
	scheduling_s signalTimerUp;
	scheduling_s signalTimerDown;
	event_trigger_position_s dwellPosition;
	float advance;
	event_trigger_position_s sparkPosition;
	IgnitionEvent *next;
	char *name;
};

typedef ArrayList<InjectionEvent, MAX_EVENT_COUNT> ActuatorEventList;

typedef ArrayList<IgnitionEvent, MAX_EVENT_COUNT> IgnitionEventList;

#endif /* EVENT_REGISTRY_H_ */
