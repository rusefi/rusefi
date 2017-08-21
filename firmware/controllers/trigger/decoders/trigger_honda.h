/*
 * @file	trigger_honda.h
 *
 * @date May 27, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONTROLLERS_TRIGGER_TRIGGER_HONDA_H_
#define CONTROLLERS_TRIGGER_TRIGGER_HONDA_H_

#include "trigger_structure.h"

void configureHondaAccordCDDip(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX);
void configureHondaAccordShifted(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX);

void configureHonda_1_4_24(TriggerShape *s, bool withOneEventSignal, bool withFourEventSignal,
		trigger_wheel_e const oneEventWave,
		trigger_wheel_e const fourEventWave,
		float d DECLARE_ENGINE_PARAMETER_SUFFIX);

void configureOnePlus16(TriggerShape *s, operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_SUFFIX);

void configureHondaCbr600(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX);
void configureHondaCbr600custom(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX);

#endif /* CONTROLLERS_TRIGGER_TRIGGER_HONDA_H_ */
