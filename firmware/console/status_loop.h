/**
 * @file	status_loop.h
 *
 * @date Mar 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef CONSOLE_LOOP_H_
#define CONSOLE_LOOP_H_

#ifdef __cplusplus
#include "engine.h"
void updateDevConsoleState(Engine *engine);
void printSensors(Engine *engine);
void printState(Engine *engine, int currentCkpEventCounter);

#endif /* __cplusplus */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

void initStatusLoop(void);
bool getFullLog(void);
void setFullLog(int value);
void startStatusThreads(void);
void sayOsHello(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CONSOLE_LOOP_H_ */
