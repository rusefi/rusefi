/**
 * @file    injector_central.h
 * @brief	Utility methods related to fuel injection.
 *
 *
 * @date Sep 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef INJECTOR_CENTRAL_H_
#define INJECTOR_CENTRAL_H_

#include "signal_executor.h"
#include "engine.h"

void initInjectorCentral(Engine *engine);
bool_t isRunningBenchTest(void);
int isInjectorEnabled(int cylinderId);
void assertCylinderId(int cylinderId, const char *msg);

#endif /* INJECTOR_CENTRAL_H_ */
