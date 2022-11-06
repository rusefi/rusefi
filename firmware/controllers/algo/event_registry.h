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

struct AngleBasedEventOld;

struct AngleBasedEventBase {
	scheduling_s scheduling;
	action_s action;
	/**
	 * Trigger-based scheduler maintains a linked list of all pending tooth-based events.
	 */
	AngleBasedEventBase *nextToothEvent = nullptr;

	virtual bool shouldSchedule(uint32_t trgEventIndex, float currentPhase, float nextPhase) const = 0;
	virtual float getAngleFromNow(float currentPhase) const = 0;

	virtual AngleBasedEventOld* asOld() { return nullptr; }
};

/**
 * This structure defines an angle position in relation to specific tooth within trigger shape
 */
class event_trigger_position_s {
public:
	size_t triggerEventIndex = 0;

	angle_t angleOffsetFromTriggerEvent = 0;

	void setAngle(angle_t angle);
};

struct AngleBasedEventOld : public AngleBasedEventBase {
	event_trigger_position_s position;

	bool shouldSchedule(uint32_t trgEventIndex, float currentPhase, float nextPhase) const override;
	float getAngleFromNow(float currentPhase) const override;

	AngleBasedEventOld* asOld() override { return this; }
};

struct AngleBasedEventNew : public AngleBasedEventBase {
	float enginePhase;

	bool shouldSchedule(uint32_t trgEventIndex, float currentPhase, float nextPhase) const override;
	float getAngleFromNow(float currentPhase) const override;
};

#define MAX_OUTPUTS_FOR_IGNITION 2

class IgnitionEvent {
public:
	IgnitionEvent();
	IgnitionOutputPin *outputs[MAX_OUTPUTS_FOR_IGNITION];
	scheduling_s dwellStartTimer;
	AngleBasedEventNew sparkEvent;

	scheduling_s trailingSparkCharge;
	scheduling_s trailingSparkFire;

	// How many additional sparks should we fire after the first one?
	// For single sparks, this should be zero.
	uint8_t sparksRemaining = 0;

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
	 * @see engineState.sparkCounter
	 */
	int sparkId = 0;
	/**
	 * [0, specs.cylindersCount)
	 */
	int cylinderIndex = 0;
	int8_t cylinderNumber = 0;
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

	AngleBasedEventOld open;
	AngleBasedEventOld close;
};


IgnitionEventList *getIgnitionEvents();
