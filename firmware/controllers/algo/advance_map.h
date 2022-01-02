/*
 * @file advance_map.h
 *
 * @date Mar 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

angle_t getAdvance(int rpm, float engineLoad);
angle_t getCylinderIgnitionTrim(size_t cylinderNumber, int rpm, float ignitionLoad);
float getTopAdvanceForBore(chamber_style_e style, int octane, double compression, double bore);
float getInitialAdvance(int rpm, float map, float advanceMax);
void buildTimingMap(float advanceMax);
angle_t getAdvanceCorrections(int rpm);
size_t getMultiSparkCount(int rpm);
