/**
 * @file	custom_engine.h
 *
 * @date Jan 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef CONFIG_ENGINES_CUSTOM_ENGINE_H_
#define CONFIG_ENGINES_CUSTOM_ENGINE_H_

#include "engine_configuration.h"

void setFrankensoConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setFrankenso_01_LCD(board_configuration_s *boardConfiguration);
void disableLCD(board_configuration_s *boardConfiguration);
void runSchedulingPrecisionTestIfNeeded(void);
void setFrankensoBoardTestConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setEtbTestConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setTle8888TestConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);


void mreBoardTest(DECLARE_CONFIG_PARAMETER_SIGNATURE);
#endif /* CONFIG_ENGINES_CUSTOM_ENGINE_H_ */
