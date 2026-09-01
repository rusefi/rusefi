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
	InjectionEvent() = default;

	bool update();

	// Call this every decoded trigger tooth.  It will schedule any relevant events for this injector.
	void onTriggerTooth(efitick_t nowNt, float currentPhase, float nextPhase, float nextNextPhase);

	WallFuel& getWallFuel();

	void setIndex(uint8_t index) {
		ownIndex = index;
	}

private:
	// Update the injection start angle
	bool updateInjectionAngle();

	// Compute the injection start angle, compensating for injection duration and injection phase settings.
	expected<float> computeInjectionAngle() const;

	/**
	 * This is a performance optimization for IM_SIMULTANEOUS fuel strategy.
	 * It's more efficient to handle all injectors together if that's the case
	 */
	bool isSimultaneous = false;
	uint8_t ownIndex = 0;
	uint8_t cylinderNumber = 0;
public:
	uint8_t getCylinderNumber() const { return cylinderNumber; }
private:

	WallFuel wallFuel{};

public:
	// TODO: this should be private
	InjectorOutputPin *outputs[MAX_WIRES_COUNT]{};
	InjectorOutputPin *outputsStage2[MAX_WIRES_COUNT]{};
	float injectionStartAngle = 0;

	// True once this cycle's injection start has been scheduled, together
	// with the absolute time it was armed for. A window match is suppressed
	// only while the armed target is still in the future (the one-tooth-ahead
	// arm covering the following tooth's window); once the target has passed,
	// a window match is the NEXT cycle's scheduling - at high duty the next
	// window can arrive before the injection end recomputes the angle, and
	// the pre-existing behavior schedules with the stale angle.
	bool injectionStartArmed = false;
	efitick_t injectionStartArmedAt = 0;

#if EFI_ANGLE_CLOCK
	// Cancellable TIM5 slot for the injection END (stage 1 closing pulse).
	// Populated with a real slot (not nullptr) so turnInjectionPinHigh can
	// cancel the pre-scheduled TIM5 close and re-arm via
	// angleClockArmInjectionFromNow at the actual opening moment.
	scheduling_s injectionEndStage1;

	// Injection close delay in NT ticks (= US2NT(durationUsStage1)), stored
	// by onTriggerTooth so turnInjectionPinHigh can re-arm TMR3 without
	// recomputing the duration from sensor data in ISR context.
	uint32_t injectionEndDelayNt = 0;
#endif // EFI_ANGLE_CLOCK
};

void turnInjectionPinHigh(scheduler_arg_t arg);
void turnInjectionPinLow(InjectionEvent *event);


/**
 * This class knows about when to inject fuel
 */
class FuelSchedule {
public:
	FuelSchedule();

	// Call this function if something happens that requires a rebuild, like a change to the trigger pattern
	void invalidate();

	// Call this every trigger tooth.  It will schedule all required injector events.
	void onTriggerTooth(efitick_t nowNt, float currentPhase, float nextPhase, float nextNextPhase);

	/**
	 * this method schedules all fuel events for an engine cycle
	 * Calculate injector opening angle, pins, and mode for all injectors
	 */
	void addFuelEvents();

	static void resetOverlapping();

	/**
	 * injection events, per cylinder
	 */
	InjectionEvent elements[MAX_CYLINDER_COUNT];
	bool isReady = false;
};

FuelSchedule * getFuelSchedule();
