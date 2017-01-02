/*
 * @file	trigger_honda.h
 *
 * @date May 27, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#ifndef CONTROLLERS_TRIGGER_TRIGGER_HONDA_H_
#define CONTROLLERS_TRIGGER_TRIGGER_HONDA_H_

#include "trigger_structure.h"

void configureHondaAccordCDDip(TriggerShape *s DECLARE_ENGINE_PARAMETER_S);
void configureHondaAccordShifter(TriggerShape *s DECLARE_ENGINE_PARAMETER_S);
void configureHondaAccordCD(TriggerShape *s, bool withOneEventSignal, bool withFourEventSignal,
		trigger_wheel_e const oneEventWave,
		trigger_wheel_e const fourEventWave,
		float d DECLARE_ENGINE_PARAMETER_S);

void configureHondaCbr600(TriggerShape *s DECLARE_ENGINE_PARAMETER_S);
void configureHondaCbr600custom(TriggerShape *s DECLARE_ENGINE_PARAMETER_S);

#endif /* CONTROLLERS_TRIGGER_TRIGGER_HONDA_H_ */
