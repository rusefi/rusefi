/**
 * @file    injector_central.h
 * @brief	Utility methods related to fuel injection.
 *
 * todo: rename this file
 *
 * @date Sep 8, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

void fanBench(void);
void fuelPumpBench(void);
void acRelayBench(void);
void milBench(void);
void initInjectorCentral(Logging *sharedLogger);
bool isRunningBenchTest(void);

void executeTSCommand(uint16_t subsystem, uint16_t index);
