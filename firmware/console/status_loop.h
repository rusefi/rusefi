/**
 * @file	status_loop.h
 *
 * @date Mar 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "rusefi_types.h"

void updateDevConsoleState(void);
void startStatusThreads(void);
void initStatusLoop(void);

struct Writer;

void printOverallStatus();
