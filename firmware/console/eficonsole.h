/**
 * @file    eficonsole.h
 * @brief   Console package entry point header
 *
 * @date Nov 15, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once
#include "datalogging.h"

void initializeConsole(Logging *sharedLogger);
void print(const char *fmt, ...);

