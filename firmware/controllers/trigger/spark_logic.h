/*
 * @file spark_logic.h
 *
 * @date Sep 15, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#pragma once

#include "engine.h"

int isInjectionEnabled(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void onTriggerEventSparkLogic(bool limitedSpark, uint32_t trgEventIndex, int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);
void initSparkLogic(Logging *sharedLogger);
void turnSparkPinHigh(IgnitionEvent *event);
void fireSparkAndPrepareNextSchedule(IgnitionEvent *event);
int getNumberOfSparks(ignition_mode_e mode DECLARE_ENGINE_PARAMETER_SUFFIX);
percent_t getCoilDutyCycle(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);
void initializeIgnitionActions(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#define TRIGGER_EVENT_UNDEFINED -1
bool scheduleOrQueue(AngleBasedEvent *event,
		uint32_t trgEventIndex,
		angle_t angle,
		schfunc_t callback,
		void *param DECLARE_ENGINE_PARAMETER_SUFFIX);
