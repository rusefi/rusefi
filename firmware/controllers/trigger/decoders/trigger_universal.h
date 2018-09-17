/*
 * @file trigger_universal.h
 *
 *
 * @date Jan 3, 2017
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONTROLLERS_TRIGGER_DECODERS_TRIGGER_UNIVERSAL_H_
#define CONTROLLERS_TRIGGER_DECODERS_TRIGGER_UNIVERSAL_H_

#include "global.h"
#include "trigger_structure.h"
#include "engine_configuration.h"

#define NO_LEFT_FILTER -1
#define NO_RIGHT_FILTER 1000

void addSkippedToothTriggerEvents(trigger_wheel_e wheel, TriggerShape *s,
		int totalTeethCount, int skippedCount,
		float toothWidth,
		float offset, float engineCycle, float filterLeft, float filterRight DECLARE_ENGINE_PARAMETER_SUFFIX);


void initializeSkippedToothTriggerShapeExt(TriggerShape *s, int totalTeethCount, int skippedCount, operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_SUFFIX);;

void configureOnePlus60_2(TriggerShape *s, operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_SUFFIX);

void configure3_1_cam(TriggerShape *s, operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_SUFFIX);

void configureOnePlusOne(TriggerShape *s, operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_SUFFIX);
#endif /* CONTROLLERS_TRIGGER_DECODERS_TRIGGER_UNIVERSAL_H_ */
