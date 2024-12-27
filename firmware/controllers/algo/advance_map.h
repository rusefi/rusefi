/*
 * @file advance_map.h
 *
 * @date Mar 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

angle_t getCylinderIgnitionTrim(size_t cylinderNumber, float rpm, float ignitionLoad);
/**
 * this method is used to build default advance map
 */
float getInitialAdvance(float rpm, float map, float advanceMax);
// public only for unit tests
angle_t getCrankingAdvance(float rpm, float engineLoad);
angle_t getRunningAdvance(float rpm, float engineLoad);
angle_t getAdvanceCorrections(float engineLoad);

size_t getMultiSparkCount(float rpm);
void initIgnitionAdvanceControl();
