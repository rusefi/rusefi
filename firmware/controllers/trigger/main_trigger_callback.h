/**
 * @file    main_trigger_callback.h
 * @brief   Main logic header
 *
 *
 * @date Feb 9, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#ifndef MAIN_LOOP_H_
#define MAIN_LOOP_H_

#include "main.h"

#include "event_registry.h"
#include "engine.h"

void initMainEventListener(Logging *sharedLogger, Engine *engine);
void mainTriggerCallback(trigger_event_e ckpSignalType, uint32_t eventIndex DECLARE_ENGINE_PARAMETER_S);

int isIgnitionTimingError(void);

void showMainHistogram(void);

float getFuel(int rpm, float key);
#endif /* MAIN_LOOP_H_ */
