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

/**
 * Primary rusEfi test mule https://rusefi.com/forum/viewtopic.php?f=3&t=1095
 * MAZDA_MIATA_2003
 * set engine_type 47
 */
void setMazdaMiata2003EngineConfiguration();
/**
 * race car - NA body and fuel system with NB2 engine
 */
void setMazdaMiata2003EngineConfigurationNaFuelRail();
void setMazdaMiata2003EngineConfigurationBoardTest();

/**
 * https://github.com/rusefi/rusefi/wiki/Mazda-Miata-2001
 * set engine_type 1
 */
void setMiataNB2_Proteus_TCU();

void setMiataNB1_Polygonus();
void setMiataNB2_Polygonus();
void setMiataNB_MSM_Polygonus();

/**
 * set engine_type 69
 */
void setMiataNB2_Hellen72();

void setMiataNB2_Hellen72_36();


void setHellenNB1();
