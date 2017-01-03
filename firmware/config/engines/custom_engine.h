/**
 * @file	custom_engine.h
 *
 * @date Jan 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef CONFIG_ENGINES_CUSTOM_ENGINE_H_
#define CONFIG_ENGINES_CUSTOM_ENGINE_H_

#include "engine.h"

void setCustomEngineConfiguration(DECLARE_ENGINE_PARAMETER_F);
void setFrankenso_01_LCD(board_configuration_s *boardConfiguration);
void disableLCD(board_configuration_s *boardConfiguration);

#endif /* CONFIG_ENGINES_CUSTOM_ENGINE_H_ */
