/**
 * @file	event_registry.cpp
 * @brief	This data structure knows when to do what
 *
 * @date Nov 27, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
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

#include "pch.h"

#include "event_registry.h"

static void findTriggerPosition(TriggerWaveform *triggerShape,
		TriggerFormDetails *details,
		event_trigger_position_s *position,
		angle_t angle) {
	efiAssertVoid(CUSTOM_ERR_6574, !cisnan(angle), "findAngle#1");
	assertAngleRange(angle, "findAngle#a1", CUSTOM_ERR_6545);

	efiAssertVoid(CUSTOM_ERR_6575, !cisnan(triggerShape->tdcPosition), "tdcPos#1")
	assertAngleRange(triggerShape->tdcPosition, "tdcPos#a1", CUSTOM_UNEXPECTED_TDC_ANGLE);

	efiAssertVoid(CUSTOM_ERR_6576, !cisnan(engineConfiguration->globalTriggerAngleOffset), "tdcPos#2")
	assertAngleRange(engineConfiguration->globalTriggerAngleOffset, "tdcPos#a2", CUSTOM_INVALID_GLOBAL_OFFSET);

	// convert engine cycle angle into trigger cycle angle
	angle += triggerShape->tdcPosition + engineConfiguration->globalTriggerAngleOffset;
	efiAssertVoid(CUSTOM_ERR_6577, !cisnan(angle), "findAngle#2");
	wrapAngle2(angle, "addFuel#2", CUSTOM_ERR_6555, getEngineCycle(triggerShape->getWheelOperationMode()));

	int triggerEventIndex = triggerShape->findAngleIndex(details, angle);
	angle_t triggerEventAngle = details->eventAngles[triggerEventIndex];
	angle_t offsetFromTriggerEvent = angle - triggerEventAngle;

	// Guarantee that we aren't going to try and schedule an event prior to the tooth
	if (offsetFromTriggerEvent < 0) {
		warning(CUSTOM_OBD_ANGLE_CONSTRAINT_VIOLATION, "angle constraint violation in findTriggerPosition(): %.2f/%.2f", angle, triggerEventAngle);
		return;
	}

	{
		// This must happen under lock so that the tooth and offset don't get partially read and mismatched
		chibios_rt::CriticalSectionLocker csl;

		position->triggerEventIndex = triggerEventIndex;
		position->angleOffsetFromTriggerEvent = offsetFromTriggerEvent;
	}
}

void event_trigger_position_s::setAngle(angle_t angle) {
	findTriggerPosition(&engine->triggerCentral.triggerShape,
			&engine->triggerCentral.triggerFormDetails,
			this, angle);
}

IgnitionEvent::IgnitionEvent() {
	memset(outputs, 0, sizeof(outputs));
}

IgnitionOutputPin * IgnitionEvent::getOutputForLoggins() {
	return outputs[0];
}

