/**
 * @file	trigger_input.h
 * @brief	Position sensor hardware layer
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CRANK_INPUT_H_
#define CRANK_INPUT_H_

#include "engine.h"

#define TRIGGER_SUPPORTED_CHANNELS 2

void turnOnTriggerInputPins(Logging *sharedLogger);
void applyNewTriggerInputPins(void);
void startTriggerInputPins(void);
void stopTriggerInputPins(void);

#endif /* CRANK_INPUT_H_ */
