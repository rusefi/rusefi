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

#include "engine_configuration_generated_structures.h"

void initAlternatorCtrl();
void startAlternatorPin(void);
void stopAlternatorPin(void);
void setAltPFactor(float p);
void setAltIFactor(float p);
void setAltDFactor(float p);
void showAltInfo(void);

void onConfigurationChangeAlternatorCallback(engine_configuration_s *previousConfiguration);
