/*
 * @file advance_map.h
 *
 * @date Mar 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2017
 */

#ifndef ADVANCE_H_
#define ADVANCE_H_

#include "engine.h"

angle_t getAdvance(int rpm, float engineLoad DECLARE_ENGINE_PARAMETER_SUFFIX);
void setDefaultIatTimingCorrection(DECLARE_ENGINE_PARAMETER_SIGNATURE);
void initTimingMap(DECLARE_ENGINE_PARAMETER_SIGNATURE);
float getTopAdvanceForBore(chamber_style_e style, int octane, double compression, double bore);
float getInitialAdvance(int rpm, float map, float advanceMax);
void buildTimingMap(float advanceMax DECLARE_CONFIG_PARAMETER_SUFFIX);
angle_t getAdvanceCorrections(int rpm DECLARE_ENGINE_PARAMETER_SUFFIX);

#endif /* ADVANCE_H_ */
