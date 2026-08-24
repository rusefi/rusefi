/**
 * @file	status_loop.h
 *
 * @date Mar 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "rusefi_types.h"

void updateDevConsoleState();
// starts LED blinking; called very early in boot, before settings are loaded - see
// the comment at the implementation for the constraints this puts on the code inside
void startStatusThreads();

void printOverallStatus();
