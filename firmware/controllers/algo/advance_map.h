/*
 * @file advance_map.h
 *
 * @date Mar 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef ADVANCE_H_
#define ADVANCE_H_

#define AD_LOAD_COUNT 16
#define AD_RPM_COUNT 16

#include "engine.h"

float getAdvance(int rpm, float engineLoad DECLATE_ENGINE_PARAMETER);
void prepareTimingMap(void);

#endif /* ADVANCE_H_ */
