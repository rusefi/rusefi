/**
 * @file	custom_engine.h
 *
 * @date Jan 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_configuration.h"
void setFrankensoConfiguration();
void setDiscoveryPdm();
void setDiscovery33810Test();
void setEtbTestConfiguration();
void setL9779TestConfiguration();

void setEepromTestConfiguration();

void mreSecondaryCan();
void mreBCM();
void setBodyControlUnit();

void fuelBenchMode();

void proteusLuaDemo();
void proteusStimQc();
void proteusDcWastegateTest();

void setTest33816EngineConfiguration();
void setBoschHDEV_5_injectors();

void setRotary();
void setVrThresholdTest();

void detectBoardType();

void testEngine6451();
