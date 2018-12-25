/*
 * trigger_misc.cpp
 *
 *  Created on: Oct 30, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "trigger_misc.h"

// TT_FIAT_IAW_P8
void configureFiatIAQ_P8(TriggerShape * s DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, false);

	s->isSynchronizationNeeded = true;


	int width = 60;
	s->tdcPosition = width;

	s->addEvent720(width, T_PRIMARY, TV_RISE);
	s->addEvent720(180, T_PRIMARY, TV_FALL);

	s->addEvent720(180 + width, T_PRIMARY, TV_RISE);
	s->addEvent720(720, T_PRIMARY, TV_FALL);
	s->setTriggerSynchronizationGap(3);
}

