/*
 * @file bmw_m73.h
 *
 * @date Nov 1, 2019
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_configuration.h"

void m73engine(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setBMW_M73_TwoCoilUnitTest(DECLARE_CONFIG_PARAMETER_SIGNATURE);
// set engine_type 24
void setEngineBMW_M73_Manhattan(DECLARE_CONFIG_PARAMETER_SIGNATURE);
// set engine_type 63
void setEngineBMW_M73_Proteus(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setEngineBMW_M73_microRusEfi(DECLARE_CONFIG_PARAMETER_SIGNATURE);
