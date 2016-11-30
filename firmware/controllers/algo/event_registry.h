/**
 * @file	event_registry.h
 *
 * @date Nov 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#ifndef EVENT_REGISTRY_H_
#define EVENT_REGISTRY_H_

#include "global.h"
#include "signal_executor.h"
#include "fl_stack.h"

/**
 * This structure defines an angle position within the trigger
 */
class event_trigger_position_s {
public:
	event_trigger_position_s();
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

class InjectionEvent {
public:
	InjectionEvent();
	/**
	 * This is a performance optimization for IM_SIMULTANEOUS fuel strategy.
	 * It's more efficient to handle all injectors together if that's the case
	 */
	bool isSimultanious;
	InjectorOutputPin *output;
	bool isOverlapping;

	event_trigger_position_s injectionStart;
};

#define MAX_OUTPUTS_FOR_IGNITION 2

class Engine;

class IgnitionEvent {
public:
	IgnitionEvent();
	IgnitionOutputPin *outputs[MAX_OUTPUTS_FOR_IGNITION];
	scheduling_s signalTimerUp;
	scheduling_s signalTimerDown;
	angle_t advance;
	event_trigger_position_s dwellPosition;
	event_trigger_position_s sparkPosition;
	IgnitionEvent *next;
	/**
	 * @see globalSparkIdCoutner
	 */
	int sparkId;
	int cylinderIndex;
	char *name;
#if EFI_UNIT_TEST
	Engine *engine;
#endif
	IgnitionOutputPin *getOutputForLoggins();
};


/**
 * two wire batch mode would generate twice the number of cylinders events, that's the max
 */
#define MAX_INJECTION_OUTPUT_COUNT 2 * INJECTION_PIN_COUNT
#define MAX_IGNITION_EVENT_COUNT IGNITION_PIN_COUNT

typedef ArrayList<InjectionEvent, MAX_INJECTION_OUTPUT_COUNT> InjectionEventList;

class IgnitionEventList {
public:
	IgnitionEventList();
	IgnitionEvent elements[MAX_IGNITION_EVENT_COUNT];
	bool isReady;
};

#endif /* EVENT_REGISTRY_H_ */
