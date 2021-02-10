/*
 * @file aux_pid.h
 *
 * @date Jun 26, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "global.h"

void initAuxPid(Logging *sharedLogger);
void startAuxPins();
void stopAuxPins();

