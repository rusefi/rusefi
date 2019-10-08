/**
 * @file	event_registry.h
 *
 * @date Nov 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#pragma once

#include "global.h"
#include "signal_executor.h"
#include "fl_stack.h"
#include "trigger_structure.h"

class Engine;

class InjectionEvent {
public:
	InjectionEvent();
	/**
	 * This is a performance optimization for IM_SIMULTANEOUS fuel strategy.
	 * It's more efficient to handle all injectors together if that's the case
	 */
	bool isSimultanious;
	InjectorOutputPin *outputs[MAX_WIRES_COUNT];
	bool isOverlapping;
	int ownIndex;
#if EFI_UNIT_TEST
	Engine *engine;
#endif
	event_trigger_position_s injectionStart;
};

#define MAX_OUTPUTS_FOR_IGNITION 2

class IgnitionEvent {
public:
	IgnitionEvent();
	IgnitionOutputPin *outputs[MAX_OUTPUTS_FOR_IGNITION];
	scheduling_s dwellStartTimer;
	scheduling_s signalTimerDown;
	/**
	 * Desired timing advance
	 */
	angle_t advance = NAN;
	floatms_t sparkDwell;
	/**
	 * this timestamp allows us to measure actual dwell time
	 */
	uint32_t actualStartOfDwellNt;
	event_trigger_position_s dwellPosition;
	event_trigger_position_s sparkPosition;
	/**
	 * Ignition scheduler maintains a linked list of all pending ignition events.
	 */
	IgnitionEvent *next = nullptr;
	/**
	 * Sequential number of currently processed spark event
	 * @see globalSparkIdCounter
	 */
	int sparkId = 0;
	/**
	 * [0, specs.cylindersCount)
	 */
	int cylinderIndex = 0;
	char *name = nullptr;
#if EFI_UNIT_TEST
	Engine *engine;
#endif
	IgnitionOutputPin *getOutputForLoggins();
};

#define MAX_IGNITION_EVENT_COUNT IGNITION_PIN_COUNT

class IgnitionEventList {
public:
	IgnitionEvent elements[MAX_IGNITION_EVENT_COUNT];
	bool isReady = false;
};
