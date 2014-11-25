/*
 * @file	trigger_central.h
 *
 * @date Feb 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef TRIGGER_CENTRAL_H_
#define TRIGGER_CENTRAL_H_

#include "rusefi_enums.h"
#include "listener_array.h"
#include "trigger_decoder.h"

class Engine;
typedef void (*ShaftPositionListener)(trigger_event_e signal, uint32_t index DECLARE_ENGINE_PARAMETER_S);

#ifdef __cplusplus
#include "ec2.h"
#include "engine.h"

#define HW_EVENT_TYPES 6

class TriggerCentral {
public:
	TriggerCentral();
	void addEventListener(ShaftPositionListener handler, const char *name, Engine *engine);
	void handleShaftSignal(trigger_event_e signal, Engine *engine, engine_configuration_s *engineConfiguration);
	int getHwEventCounter(int index);
	TriggerState triggerState;
	uint64_t nowNt;
private:
	IntListenerArray triggerListeneres;
	int hwEventCounters[HW_EVENT_TYPES];
};
#endif

uint64_t getCrankEventCounter(void);
uint64_t getStartOfRevolutionIndex(void);
void hwHandleShaftSignal(trigger_event_e signal);
float getTriggerDutyCycle(int index);
void initTriggerCentral(Engine *engine);
void printAllCallbacksHistogram(void);

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void addTriggerEventListener(ShaftPositionListener handler, const char *name, Engine *engine);
int isSignalDecoderError(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TRIGGER_CENTRAL_H_ */
