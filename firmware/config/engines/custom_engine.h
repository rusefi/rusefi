/**
 * @file	custom_engine.h
 *
 * @date Jan 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_configuration.h"

void setFrankensoConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setFrankenso_01_LCD(engine_configuration_s *engineConfiguration);
void disableLCD(engine_configuration_s *engineConfiguration);
void runSchedulingPrecisionTestIfNeeded(void);
void setFrankensoBoardTestConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setEtbTestConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setTle8888TestConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setIssue898(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void mreBoardTest(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setTest33816EngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
