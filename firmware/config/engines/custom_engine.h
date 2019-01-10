/**
 * @file	custom_engine.h
 *
 * @date Jan 18, 2015
 * @author Andrey Belomutskiy, (c) 2012-2017
 */
#ifndef CONFIG_ENGINES_CUSTOM_ENGINE_H_
#define CONFIG_ENGINES_CUSTOM_ENGINE_H_

#include "engine.h"

void setCustomEngineConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void setFrankenso_01_LCD(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void disableLCD(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void test557init(void);

void setFrankensoBoardTestConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE);

#endif /* CONFIG_ENGINES_CUSTOM_ENGINE_H_ */
