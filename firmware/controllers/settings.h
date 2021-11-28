/**
 * @file settings.h
 * @brief This file is about configuring engine via the human-readable protocol
 *
 * @date Dec 30, 2012
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_configuration_generated_structures.h"

void initSettings(void);
void printSpiState(const engine_configuration_s *engineConfiguration);
void printConfiguration(const engine_configuration_s *engineConfiguration);
void scheduleStopEngine(void);
void setCallFromPitStop(int durationMs);
void printTPSInfo(void);
void setEngineType(int value);
void readPin(const char *pinName);
