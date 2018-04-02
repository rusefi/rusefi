/**
 * @file	status_loop.h
 *
 * @date Mar 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef CONSOLE_LOOP_H_
#define CONSOLE_LOOP_H_

#include "engine.h"

void updateDevConsoleState(void);
void prepareTunerStudioOutputs(void);
void startStatusThreads(void);
void initStatusLoop(void);
void writeLogLine(void);
bool getFullLog(void);
void setFullLog(int value);
void printOverallStatus(systime_t nowSeconds);

#endif /* CONSOLE_LOOP_H_ */
