/**
 * @file	trigger_subaru.cpp
 *
 * @date Sep 10, 2015
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "trigger_subaru.h"

void initialize36_2_2_2(TriggerShape *s) {
	float wide = 30;

	s->isSynchronizationNeeded = false;

	s->addEvent(720 - wide / 2, T_SECONDARY, TV_HIGH);
	s->addEvent(720, T_SECONDARY, TV_LOW);
}
