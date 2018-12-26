/**
 * @file	trigger_chrysler.h
 *
 * @date Mar 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef TRIGGER_CHRYSLER_H_
#define TRIGGER_CHRYSLER_H_

#include "trigger_decoder.h"

#define CHRYSLER_NGC4_GAP 1
#define CHRYSLER_NGC6_GAP 1.5

void configureNeon1995TriggerShape(TriggerShape *s);
void configureNeon1995TriggerShapeOnlyCrank(TriggerShape *s);

void configureNeon2003TriggerShapeCam(TriggerShape *s);
void configureNeon2003TriggerShapeCrank(TriggerShape *s);
void initDodgeRam(TriggerShape *s);

void configureDodgeStratusTriggerShape(TriggerShape *s);

void initJeep18_2_2_2(TriggerShape *s);
void initJeep_XJ_4cyl_2500(TriggerShape *s);

#endif /* TRIGGER_CHRYSLER_H_ */
