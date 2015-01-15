/**
 * @file	idle_thread.h
 * @brief	Idle Valve Control thread
 *
 * @date May 23, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef IDLE_THREAD_H_
#define IDLE_THREAD_H_

#include "engine.h"

void startIdleThread(Logging*sharedLogger, Engine *engine);

#endif /* IDLE_THREAD_H_ */
