/**
 * @file	trigger_chrysler.h
 *
 * @date Mar 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef TRIGGER_CHRYSLER_H_
#define TRIGGER_CHRYSLER_H_

#include "trigger_structure.h"

#define CHRYSLER_NGC4_GAP 3.4390
#define CHRYSLER_NGC6_GAP 1.5

void configureNeon1995TriggerShape(TriggerShape *s);
void configureNeon2003TriggerShape(TriggerShape *s);
void initDodgeRam(TriggerShape *s);

void configureDodgeStratusTriggerShape(TriggerShape *s);

#endif /* TRIGGER_CHRYSLER_H_ */
