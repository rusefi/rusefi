/**
 * @file	custom_engine.h
 *
 * @date Jan 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_configuration.h"
void setHellenDefaultVrThresholds(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setFrankensoConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setFrankenso_01_LCD(engine_configuration_s *engineConfiguration);
void disableLCD(engine_configuration_s *engineConfiguration);
void runSchedulingPrecisionTestIfNeeded(void);
void setFrankensoBoardTestConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setEtbTestConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setTle8888TestConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setIssue898(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setTestQuadCam(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void mreBoardNewTest(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void mreSecondaryCan(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void mreBCM(DECLARE_CONFIG_PARAMETER_SIGNATURE);

/**
 * PROTEUS_QC_TEST_BOARD
 * set engine_type 42
 */
void proteusBoardTest(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setTest33816EngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setBoschHDEV_5_injectors(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setHellen72etb(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setRotary(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setVrThresholdTest(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setHellen144LedPins(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setHellen176LedPins(DECLARE_CONFIG_PARAMETER_SIGNATURE);
