/**
 * @file	status_loop.h
 *
 * @date Mar 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine.h"

void updateDevConsoleState(void);
void prepareTunerStudioOutputs(void);
void startStatusThreads(void);
void initStatusLoop(void);
void writeLogLine(void);
void printOverallStatus(systime_t nowSeconds);
