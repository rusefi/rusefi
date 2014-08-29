/**
 * @file    main_trigger_callback.h
 * @brief   Main logic header
 *
 *
 * @date Feb 9, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef MAIN_LOOP_H_
#define MAIN_LOOP_H_

#include "main.h"

#define MAX_INJECTOR_COUNT 12
#define MAX_IGNITER_COUNT 4

#ifdef __cplusplus
#include "engine_configuration.h"
#include "ec2.h"
#include "event_registry.h"
#include "engine.h"

class MainTriggerCallback {
public:
//	MainTriggerCallback();
	void init(Engine *engine, engine_configuration2_s *engineConfiguration2);

	Engine *engine;
	engine_configuration_s *engineConfiguration;
	engine_configuration2_s *engineConfiguration2;

};
void initMainEventListener(Engine *engine, engine_configuration2_s *engineConfiguration2);
void onTriggerEvent(trigger_event_e ckpSignalType, int eventIndex, MainTriggerCallback *mainTriggerCallback);
#endif


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



void showMainHistogram(void);
void onEveryMillisecondTimerSignal(void);
int isIgnitionTimingError(void);

float getFuel(int rpm, float key);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* MAIN_LOOP_H_ */
