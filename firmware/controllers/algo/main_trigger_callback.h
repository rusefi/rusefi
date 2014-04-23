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
extern "C"
{
#endif /* __cplusplus */

#include "event_registry.h"

void initMainEventListener(void);
void showMainHistogram(void);
void onEveryMillisecondTimerSignal(void);
int isIgnitionTimingError(void);

float getFuel(int rpm, float key);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* MAIN_LOOP_H_ */
