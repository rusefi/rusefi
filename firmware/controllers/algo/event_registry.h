/**
 * @file	event_registry.h
 *
 * @date Nov 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"
#include "efi_gpio.h"
#include "scheduler.h"
#include "fl_stack.h"
#include "trigger_structure.h"

struct AngleBasedEvent {
	scheduling_s scheduling;
	action_s action;
	/**
	 * Trigger-based scheduler maintains a linked list of all pending tooth-based events.
	 */
	AngleBasedEvent *nextToothEvent = nullptr;

  // angular position of this event
  angle_t getAngle() const {
    return enginePhase;
  }

	void setAngle(angle_t p_enginePhase) {
    	enginePhase = p_enginePhase;
    }

	bool shouldSchedule(float currentPhase, float nextPhase) const;
	float getAngleFromNow(float currentPhase) const;
private:
	angle_t enginePhase;
};

// this is related to wasted spark idea where engines fire each spark twice per 4 stroke 720 degree cycle of operations
// first spark is happens on intake stroke and actually ignites fuel mixture, that's the useful one
// the other spark 360 degrees later happens during exhaust stroke meaning there is nothing to ignite, that spark is known as "wasted" spark
// historically this was about sharing ignition coils between opposite cylinders and having two high voltage wire coming from one physical coil
// more recently same idea happens with two individual physical coils (meaning two outputs) since wasted spark of operation is useful
// while exact engine phase is either not known YET (cranking) or just not known (broken cam sensor)
// so, while in wasted spark we manage half of cylinder count _events_ potentially with each event having two outputs
//
// an interesting corner case is when we transition from wasted spark mode into individual/sequential mode
#define MAX_OUTPUTS_FOR_IGNITION 2

class IgnitionEvent {
public:
	IgnitionEvent();
	// IgnitionEvent to IgnitionOutputPin is either 1 to 1 or 1 to 2 relationship, see large comment at 'MAX_OUTPUTS_FOR_IGNITION'
	IgnitionOutputPin *outputs[MAX_OUTPUTS_FOR_IGNITION];
	scheduling_s dwellStartTimer;
	AngleBasedEvent sparkEvent;

	scheduling_s trailingSparkCharge;
	scheduling_s trailingSparkFire;

	// How many additional sparks should we fire after the first one?
	// For single sparks, this should be zero.
	uint8_t sparksRemaining = 0;

	// Track whether coil charge was intentionally skipped (spark limiter)
	bool wasSparkLimited = false;

	/**
	 * Desired timing advance
	 */
	angle_t sparkAngle = NAN;
	floatms_t sparkDwell = 0;
	/**
	 * this timestamp allows us to measure actual dwell time
	 */
	uint32_t actualStartOfDwellNt = 0;

	float dwellAngle = 0;

	/**
	 * Sequential number of currently processed spark event
	 * @see engineState.globalSparkCounter
	 */
	int sparkCounter = 0;
	/**
	 * [0, cylindersCount)
	 */
	int cylinderIndex = 0;
	// previously known as cylinderNumber
	int8_t coilIndex = 0;
	char *name = nullptr;
	IgnitionOutputPin *getOutputForLoggins();
};

class IgnitionEventList {
public:
	/**
	 * ignition events, per cylinder
	 */
	IgnitionEvent elements[MAX_CYLINDER_COUNT];
	bool isReady = false;
};

class AuxActor {
public:
	int phaseIndex;
	int valveIndex;
	angle_t extra;

	AngleBasedEvent open;
	AngleBasedEvent close;
};


IgnitionEventList *getIgnitionEvents();
