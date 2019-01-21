/**
 * @file    injector_central.h
 * @brief	Utility methods related to fuel injection.
 *
 * todo: rename this file
 *
 * @date Sep 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef INJECTOR_CENTRAL_H_
#define INJECTOR_CENTRAL_H_

#include "signal_executor.h"
#include "engine.h"

void fanBench(void);
void fuelPumpBench(void);
void milBench(void);
void initInjectorCentral(Logging *sharedLogger);
bool isRunningBenchTest(void);

void runBenchTest(uint16_t subsystem, uint16_t index);

#endif /* INJECTOR_CENTRAL_H_ */
