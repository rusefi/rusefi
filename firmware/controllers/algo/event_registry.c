/**
 * @file	event_registry.c
 * @brief	This data structure knows when to do what
 *
 * @date Nov 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 *
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#include "event_registry.h"
#include "main.h"
#include "engine_math.h"

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;

void resetEventList(ActuatorEventList *list) {
	list->size = 0;
}

static void copyActuatorEvent(ActuatorEvent *source, ActuatorEvent*target) {
	target->eventIndex = source->eventIndex;
	target->actuator = source->actuator;
	target->angleOffset = source->angleOffset;
}

void registerActuatorEvent(ActuatorEventList *list, int eventIndex, OutputSignal *actuator, float angleOffset) {
	if (list->size == MAX_EVENT_COUNT) {
		fatal("registerActuatorEvent() too many events");
		return;
	}
	ActuatorEvent *e = &list->events[list->size++];
	e->eventIndex = eventIndex;
	e->actuator = actuator;
	e->angleOffset = angleOffset;
}

void findEvents(int eventIndex, ActuatorEventList *source, ActuatorEventList *target) {
	resetEventList(target);
	// todo: implement something faster
	for (int i = 0; i < source->size; i++) {
		ActuatorEvent *s = &source->events[i];
		if (s->eventIndex != eventIndex)
			continue;
		// todo: migrate to pointers instead of copying an object?
		copyActuatorEvent(s, &target->events[target->size++]);
	}
}
