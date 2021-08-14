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
void setMazdaNB2VVTSettings(DECLARE_CONFIG_PARAMETER_SIGNATURE);

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
 * OEM mechanical throttle body, with MAP
 * set engine_type 11
 */
void setMiataNB2_MRE_MAP(DECLARE_CONFIG_PARAMETER_SIGNATURE);

/**
 * OEM mechanical throttle body using OEM MAF
 * set engine_type 15
 */
void setMiataNB2_MRE_MAF(DECLARE_CONFIG_PARAMETER_SIGNATURE);

/**
 * https://github.com/rusefi/rusefi/wiki/Mazda-Miata-2001
 * set engine_type 1
 */
void setMiataNB2_Proteus_TCU(DECLARE_CONFIG_PARAMETER_SIGNATURE);

/**
 * set engine_type 67
 */
void setMiataNB2_ProteusEngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE);

/**
 * set engine_type 69
 */
void setMiataNB2_Hellen72(DECLARE_CONFIG_PARAMETER_SIGNATURE);

void setMiataNB2_Hellen72_36(DECLARE_CONFIG_PARAMETER_SIGNATURE);
