/**
 * @file	event_registry.cpp
 * @brief	This data structure knows when to do what
 *
 * @date Nov 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
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

InjectionEvent::InjectionEvent() {
	isSimultanious = false;
	memset(outputs, 0, sizeof(outputs));
}

event_trigger_position_s::event_trigger_position_s() {
	eventIndex = 0;
	eventAngle = 0;
	angleOffset = 0;
}

IgnitionEvent::IgnitionEvent() {
	name = NULL;
	next = NULL;
	memset(outputs, 0, sizeof(outputs));
	advance = NAN;
	sparkId = 0;
	cylinderIndex = 0;
}

IgnitionOutputPin * IgnitionEvent::getOutputForLoggins() {
	return outputs[0];
}


//void registerActuatorEventWhat(InjectionEventList *list, int eventIndex, OutputSignal *actuator, float angleOffset) {
//	ActuatorEvent *e = list->getNextActuatorEvent();
//	if (e == NULL)
//		return; // error already reported
//	e->position.eventIndex = eventIndex;
//	e->actuator = actuator;
//	e->position.angleOffset = angleOffset;
//}
