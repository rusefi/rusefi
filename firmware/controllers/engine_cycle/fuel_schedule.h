/**
 * @file fuel_schedule.h
 */

#pragma once

#include "global.h"
#include "efi_gpio.h"
#include "scheduler.h"
#include "fl_stack.h"
#include "trigger_structure.h"
#include "wall_fuel.h"

#define MAX_WIRES_COUNT 2


class InjectionEvent {
public:
	InjectionEvent();

	// Update the injection start angle
	bool updateInjectionAngle(int cylinderIndex);

	// Compute the injection start angle, compensating for injection duration and injection phase settings.
	expected<float> computeInjectionAngle(int cylinderIndex) const;

	// Call this every decoded trigger tooth.  It will schedule any relevant events for this injector.
	void onTriggerTooth(int rpm, efitick_t nowNt, float currentPhase, float nextPhase);

	/**
	 * This is a performance optimization for IM_SIMULTANEOUS fuel strategy.
	 * It's more efficient to handle all injectors together if that's the case
	 */
	bool isSimultaneous = false;
	InjectorOutputPin *outputs[MAX_WIRES_COUNT];
	uint8_t ownIndex = 0;
	uint8_t cylinderNumber = 0;

	float injectionStartAngle = 0;

	scheduling_s signalTimerUp;
	scheduling_s endOfInjectionEvent;

	/**
	 * we need atomic flag so that we do not schedule a new pair of up/down before previous down was executed.
	 *
	 * That's because we want to be sure that no 'down' side callback would be ignored since we are counting to see
	 * overlaps so we need the end counter to always have zero.
	 * TODO: make watchdog decrement relevant counter
	 */
	bool isScheduled = false;

	WallFuel wallFuel;
};

void turnInjectionPinHigh(InjectionEvent *event);


/**
 * This class knows about when to inject fuel
 */
class FuelSchedule {
public:
	FuelSchedule();

	// Call this function if something happens that requires a rebuild, like a change to the trigger pattern
	void invalidate();

	// Call this every trigger tooth.  It will schedule all required injector events.
	void onTriggerTooth(int rpm, efitick_t nowNt, float currentPhase, float nextPhase);

	/**
	 * this method schedules all fuel events for an engine cycle
	 */
	void addFuelEvents();
	bool addFuelEventsForCylinder(int cylinderIndex);

	void resetOverlapping();

	/**
	 * injection events, per cylinder
	 */
	InjectionEvent elements[MAX_CYLINDER_COUNT];
	bool isReady = false;
};

FuelSchedule * getFuelSchedule();
