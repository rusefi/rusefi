/*
 * @file	mazda_miata_vvt.h
 *
 * @date Oct 4, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 *
 * http://rusefi.com/forum/viewtopic.php?f=3&t=1095
 */

#pragma once

#include "engine_configuration.h"

void setMazdaMiataNbInjectorLag(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setMazdaMiataNbTpsTps(DECLARE_CONFIG_PARAMETER_SIGNATURE);

/**
 * Primary rusEfi test mule https://rusefi.com/forum/viewtopic.php?f=3&t=1095
 * MAZDA_MIATA_2003
 * set engine_type 47
 */
void setMazdaMiata2003EngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);
/**
 * race car - NA body and fuel system with NB2 engine
 */
void setMazdaMiata2003EngineConfigurationNaFuelRail(DECLARE_CONFIG_PARAMETER_SIGNATURE);
void setMazdaMiata2003EngineConfigurationBoardTest(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setMiataNB2_MRE_ETB(DECLARE_CONFIG_PARAMETER_SIGNATURE);

/**
 * OEM mechanical throttle body
 * set engine_type 11
 */
void setMiataNB2_MRE_MTB(DECLARE_CONFIG_PARAMETER_SIGNATURE);
