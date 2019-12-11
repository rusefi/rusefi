/*
 * @file	trigger_honda.h
 *
 * @date May 27, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONTROLLERS_TRIGGER_TRIGGER_HONDA_H_
#define CONTROLLERS_TRIGGER_TRIGGER_HONDA_H_

#include "trigger_structure.h"

void configureHondaAccordCDDip(TriggerWaveform *s);
void configureHondaAccordShifted(TriggerWaveform *s);

void configureHonda_1_4_24(TriggerWaveform *s, bool withOneEventSignal, bool withFourEventSignal,
		trigger_wheel_e const oneEventWave,
		trigger_wheel_e const fourEventWave,
		float d);

void configureOnePlus16(TriggerWaveform *s, operation_mode_e operationMode);

void configureHondaCbr600(TriggerWaveform *s);
void configureHondaCbr600custom(TriggerWaveform *s);

#endif /* CONTROLLERS_TRIGGER_TRIGGER_HONDA_H_ */
