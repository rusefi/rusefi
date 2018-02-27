/**
 * @file	trigger_chrysler.h
 *
 * @date Mar 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef TRIGGER_CHRYSLER_H_
#define TRIGGER_CHRYSLER_H_

#include "trigger_structure.h"

#define CHRYSLER_NGC4_GAP 1
#define CHRYSLER_NGC6_GAP 1.5

void configureNeon1995TriggerShape(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX);
void configureNeon1995TriggerShapeOnlyCrank(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX);

void configureNeon2003TriggerShapeCam(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX);
void configureNeon2003TriggerShapeCrank(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX);
void initDodgeRam(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX);

void configureDodgeStratusTriggerShape(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX);

void initJeep18_2_2_2(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX);

#endif /* TRIGGER_CHRYSLER_H_ */
