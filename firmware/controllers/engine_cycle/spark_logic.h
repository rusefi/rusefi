/*
 * @file spark_logic.h
 *
 * @date Sep 15, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_ptr.h"
#include "rusefi_types.h"
#include "scheduler.h"

class IgnitionEvent;
class AngleBasedEvent;

void onTriggerEventSparkLogic(bool limitedSpark, uint32_t trgEventIndex, int rpm, efitick_t edgeTimestamp DECLARE_ENGINE_PARAMETER_SUFFIX);
void turnSparkPinHigh(IgnitionEvent *event);
void fireSparkAndPrepareNextSchedule(IgnitionEvent *event);
int getNumberOfSparks(ignition_mode_e mode DECLARE_ENGINE_PARAMETER_SUFFIX);
percent_t getCoilDutyCycle(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);
void initializeIgnitionActions(DECLARE_ENGINE_PARAMETER_SIGNATURE);

int isIgnitionTimingError(void);

#define TRIGGER_EVENT_UNDEFINED INT32_MAX
bool scheduleOrQueue(AngleBasedEvent *event,
		uint32_t trgEventIndex,
		efitick_t edgeTimestamp,
		angle_t angle,
		action_s action
		DECLARE_ENGINE_PARAMETER_SUFFIX);
