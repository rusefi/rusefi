/*
 * @file trigger_universal.h
 *
 *
 * @date Jan 3, 2017
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#ifndef CONTROLLERS_TRIGGER_DECODERS_TRIGGER_UNIVERSAL_H_
#define CONTROLLERS_TRIGGER_DECODERS_TRIGGER_UNIVERSAL_H_

#include "trigger_structure.h"

#define NO_LEFT_FILTER -1
#define NO_RIGHT_FILTER 1000

void addSkippedToothTriggerEvents(trigger_wheel_e wheel, TriggerWaveform *s,
		int totalTeethCount, int skippedCount,
		float toothWidth,
		float offset, float engineCycle, float filterLeft, float filterRight);


void initializeSkippedToothTriggerWaveformExt(TriggerWaveform *s, int totalTeethCount, int skippedCount, operation_mode_e operationMode);;

void configureOnePlus60_2(TriggerWaveform *s, operation_mode_e operationMode);

void configure3_1_cam(TriggerWaveform *s, operation_mode_e operationMode);

void configureOnePlusOne(TriggerWaveform *s, operation_mode_e operationMode);
#endif /* CONTROLLERS_TRIGGER_DECODERS_TRIGGER_UNIVERSAL_H_ */
