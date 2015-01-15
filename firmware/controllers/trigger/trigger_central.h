/*
 * @file	trigger_central.h
 *
 * @date Feb 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef TRIGGER_CENTRAL_H_
#define TRIGGER_CENTRAL_H_

#include "rusefi_enums.h"
#include "listener_array.h"
#include "trigger_decoder.h"

class Engine;
typedef void (*ShaftPositionListener)(trigger_event_e signal, uint32_t index DECLARE_ENGINE_PARAMETER_S);

#ifdef __cplusplus
#include "engine.h"

#define HW_EVENT_TYPES 6

class TriggerCentral {
public:
	TriggerCentral();
	void addEventListener(ShaftPositionListener handler, const char *name, Engine *engine);
	void handleShaftSignal(trigger_event_e signal DECLARE_ENGINE_PARAMETER_S);
	int getHwEventCounter(int index);
	TriggerState triggerState;
	uint64_t nowNt;
private:
	IntListenerArray triggerListeneres;
	int hwEventCounters[HW_EVENT_TYPES];
};
#endif

void triggerInfo(Engine *engine);
uint64_t getCrankEventCounter(void);
uint64_t getStartOfRevolutionIndex(void);
void hwHandleShaftSignal(trigger_event_e signal);
float getTriggerDutyCycle(int index);
void initTriggerCentral(Logging *sharedLogger, Engine *engine);
void printAllCallbacksHistogram(void);

void addTriggerEventListener(ShaftPositionListener handler, const char *name, Engine *engine);
int isSignalDecoderError(void);

#endif /* TRIGGER_CENTRAL_H_ */
