/*
 * @file	mazda_miata_vvt.h
 *
 * @date Oct 4, 2016
 * @author Andrey Belomutskiy, (c) 2012-2017
 *
 * http://rusefi.com/forum/viewtopic.php?f=3&t=1095
 */

#pragma once

#include "engine_configuration.h"

void setMazdaMiataNbInjectorLag(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setMazdaMiataNbTpsTps(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setMazdaMiata2003EngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setMazdaMiata2003EngineConfigurationNaFuelRail(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setMazdaMiata2003EngineConfigurationBoardTest(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setMiataNB2_MRE_ETB(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setMiataNB2_MRE_MTB(DECLARE_CONFIG_PARAMETER_SIGNATURE);
