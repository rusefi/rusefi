/**
 * @file	status_loop.h
 *
 * @date Mar 15, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "rusefi_types.h"

void updateDevConsoleState(void);
void prepareTunerStudioOutputs(void);
void updateCurrentEnginePhase(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void startStatusThreads(void);
void initStatusLoop(void);

struct Writer;
void writeLogLine(Writer& buffer);
void printOverallStatus(efitimesec_t nowSeconds);
