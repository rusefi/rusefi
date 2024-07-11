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
void setFrankensoBoardTestConfiguration();
void setDiscovery33810Test();
void setDiscoveryTLE9104Test();
void setEtbTestConfiguration();
void setL9779TestConfiguration();

void setEepromTestConfiguration();

void mreBoardNewTest();
void mreSecondaryCan();
void mreBCM();
void setBodyControlUnit();

void fuelBenchMode();

/**
 * PROTEUS_QC_TEST_BOARD
 * set engine_type 42
 */
void proteusBoardTest();
void proteusLuaDemo();
void proteusStimQc();
void proteusDcWastegateTest();

void setTest33816EngineConfiguration();
void setBoschHDEV_5_injectors();

void setRotary();
void setVrThresholdTest();

void detectBoardType();

void alphax4chanStimQc();
void testEngine6451();
