/**
 * @file	trigger_gm.h
 *
 * @date Mar 28, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef TRIGGER_GM_H_
#define TRIGGER_GM_H_

#include "trigger_structure.h"

void configureGmTriggerShape(TriggerShape *s);
void initGmLS24(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX);

#endif /* TRIGGER_GM_H_ */
