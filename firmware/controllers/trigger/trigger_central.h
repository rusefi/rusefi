/*
 * @file	trigger_central.h
 *
 * @date Feb 23, 2014
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef TRIGGER_CENTRAL_H_
#define TRIGGER_CENTRAL_H_

#include "rusefi_enums.h"
#include "listener_array.h"
#include "trigger_decoder.h"

class Engine;
typedef void (*ShaftPositionListener)(trigger_event_e signal, uint32_t index DECLARE_ENGINE_PARAMETER_S);

#ifdef __cplusplus
class Engine;

#define HW_EVENT_TYPES 6

// todo: maybe merge TriggerCentral and TriggerState classes into one class?
class TriggerCentral {
public:
	TriggerCentral();
	void addEventListener(ShaftPositionListener handler, const char *name, Engine *engine);
	void handleShaftSignal(trigger_event_e signal DECLARE_ENGINE_PARAMETER_S);
	int getHwEventCounter(int index);
	void resetCounters();
	TriggerState triggerState;
	efitick_t nowNt;
	angle_t vvtPosition;
	/**
	 * this is similar to TriggerState#startOfCycleNt but with the crank-only sensor magic
	 */
	efitick_t timeAtVirtualZeroNt;

	TriggerShape triggerShape;

	volatile efitime_t previousShaftEventTimeNt;
private:
	IntListenerArray<15> triggerListeneres;
	int hwEventCounters[HW_EVENT_TYPES];
};
#endif

void triggerInfo(void);
efitime_t getCrankEventCounter(DECLARE_ENGINE_PARAMETER_F);
efitime_t getStartOfRevolutionIndex(DECLARE_ENGINE_PARAMETER_F);
void hwHandleShaftSignal(trigger_event_e signal);
void hwHandleVvtCamSignal(trigger_value_e front);
float getTriggerDutyCycle(int index);
void initTriggerCentral(Logging *sharedLogger, Engine *engine);
void printAllCallbacksHistogram(void);
void printAllTriggers();

void addTriggerEventListener(ShaftPositionListener handler, const char *name, Engine *engine);
int isSignalDecoderError(void);

#endif /* TRIGGER_CENTRAL_H_ */
