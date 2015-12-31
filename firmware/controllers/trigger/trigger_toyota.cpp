/*
 * @file trigger_toyota.cpp
 *
 * https://thedeltaecho.wordpress.com/2010/03/14/2jz-ge-cam-crank-signals/
 *
 * @date Dec 14, 2015
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "trigger_toyota.h"

EXTERN_ENGINE;

void initialize2jzGE(TriggerShape *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, false);

	initializeSkippedToothTriggerShapeExt(s, 3, 0,
			FOUR_STROKE_CAM_SENSOR);

	s->isSynchronizationNeeded = false;

}
