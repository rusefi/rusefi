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
	void init(Engine *engine);

	Engine *engine;

};
void initMainEventListener(Engine *engine);
void mainTriggerCallback(trigger_event_e ckpSignalType, uint32_t eventIndex DECLARE_ENGINE_PARAMETER_S);
#endif

int isIgnitionTimingError(void);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void showMainHistogram(void);
void onEveryMillisecondTimerSignal(void);

float getFuel(int rpm, float key);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* MAIN_LOOP_H_ */
