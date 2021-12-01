/**
 * @file	custom_engine.h
 *
 * @date Jan 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_configuration.h"
void setHellenDefaultVrThresholds();
void setFrankensoConfiguration();
void setFrankenso_01_LCD(engine_configuration_s *engineConfiguration);
void disableLCD(engine_configuration_s *engineConfiguration);
void runSchedulingPrecisionTestIfNeeded(void);
void setFrankensoBoardTestConfiguration();
void setEtbTestConfiguration();
void setTle8888TestConfiguration();

void setIssue898();
void setTestQuadCam();

void mreBoardNewTest();
void mreSecondaryCan();
void mreBCM();

/**
 * PROTEUS_QC_TEST_BOARD
 * set engine_type 42
 */
void proteusBoardTest();
void proteusLuaDemo();
void proteusHarley();

void setTest33816EngineConfiguration();
void setBoschHDEV_5_injectors();
void setHellen72etb();
void setRotary();
void setVrThresholdTest();

void setHellen144LedPins();
void setHellen176LedPins();
