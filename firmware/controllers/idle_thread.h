/**
 * @file	idle_thread.h
 * @brief	Idle Valve Control thread
 *
 * @date May 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#ifndef IDLE_THREAD_H_
#define IDLE_THREAD_H_

#include "engine.h"

percent_t getIdlePosition(void);
void startIdleThread(Logging*sharedLogger, Engine *engine);
void setDefaultIdleParameters(void);
void startIdleBench(void);

#endif /* IDLE_THREAD_H_ */
