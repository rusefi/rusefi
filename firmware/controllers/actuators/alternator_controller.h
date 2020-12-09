/**
 * @file    alternator_controller.h
 * @brief   alternator controller
 *
 * @date Apr 6, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 */

#pragma once

#include "engine.h"
void initAlternatorCtrl(Logging *sharedLogger);
void startAlternatorPin(void);
void stopAlternatorPin(void);
void setAltPFactor(float p);
void setAltIFactor(float p);
void setAltDFactor(float p);
void showAltInfo(void);
void setDefaultAlternatorParameters(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void onConfigurationChangeAlternatorCallback(engine_configuration_s *previousConfiguration);
