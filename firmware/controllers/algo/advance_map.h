/*
 * @file advance_map.h
 *
 * @date Mar 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef ADVANCE_H_
#define ADVANCE_H_

#include "engine.h"

angle_t getAdvance(int rpm, float engineLoad DECLARE_ENGINE_PARAMETER_S);
void setDefaultIatTimingCorrection(DECLARE_ENGINE_PARAMETER_F);
void prepareTimingMap(DECLARE_ENGINE_PARAMETER_F);

#endif /* ADVANCE_H_ */
