/*
 * @file advance_map.h
 *
 * @date Mar 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#pragma once

#include "engine_ptr.h"
#include "rusefi_enums.h"

angle_t getAdvance(int rpm, float engineLoad DECLARE_ENGINE_PARAMETER_SUFFIX);
void initTimingMap(DECLARE_ENGINE_PARAMETER_SIGNATURE);
float getTopAdvanceForBore(chamber_style_e style, int octane, double compression, double bore);
float getInitialAdvance(int rpm, float map, float advanceMax);
void buildTimingMap(float advanceMax DECLARE_CONFIG_PARAMETER_SUFFIX);
angle_t getAdvanceCorrections(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);
size_t getMultiSparkCount(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);
