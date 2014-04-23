/**
 * @file	trigger_mazda.c
 *
 * @date Feb 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
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

#include "trigger_mazda.h"

void initializeMazdaMiataNbShape(trigger_shape_s *s) {
	triggerShapeInit(s);

	/**
	 * cam sensor is primary, crank sensor is secondary
	 */
	triggerAddEvent(s, 20, T_PRIMARY, 1);

	triggerAddEvent(s, 66, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, 70, T_SECONDARY, 1);
	triggerAddEvent(s, 136, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, 140, T_SECONDARY, 1);
	triggerAddEvent(s, 246, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, 250, T_SECONDARY, 1);
	triggerAddEvent(s, 316, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, 320, T_SECONDARY, 1);

	triggerAddEvent(s, 340, T_PRIMARY, TV_LOW);
	triggerAddEvent(s, 360, T_PRIMARY, 1);

	triggerAddEvent(s, 380, T_PRIMARY, TV_LOW);
	triggerAddEvent(s, 400, T_PRIMARY, 1);

	triggerAddEvent(s, 426, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, 430, T_SECONDARY, 1);
	triggerAddEvent(s, 496, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, 500, T_SECONDARY, 1);
	triggerAddEvent(s, 606, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, 610, T_SECONDARY, 1);
	triggerAddEvent(s, 676, T_SECONDARY, TV_LOW);
	triggerAddEvent(s, 680, T_SECONDARY, 1);

	triggerAddEvent(s, 720, T_PRIMARY, TV_LOW);

	s->shaftPositionEventCount = 6 + 16;
}

void configureMazdaProtegeLx(engine_configuration_s *engineConfiguration,
		engine_configuration2_s *engineConfiguration2) {

	trigger_shape_s *s = &engineConfiguration2->triggerShape;

	triggerShapeInit(s);
//	s->initialState[0] = 1;

	float w = 720 / 4 * 0.215;
	float a = 5;
//	triggerAddEvent(s, a, T_SECONDARY, TV_LOW);
//	triggerAddEvent(s, a + w, T_SECONDARY, TV_HIGH);
//	a += 180;
//	triggerAddEvent(s, a, T_SECONDARY, TV_LOW);
//	triggerAddEvent(s, a + w, T_SECONDARY, TV_HIGH);
//	a += 180;
//	triggerAddEvent(s, a, T_SECONDARY, TV_LOW);
//	triggerAddEvent(s, a + w, T_SECONDARY, TV_HIGH);
//	a += 180;
//	triggerAddEvent(s, a, T_SECONDARY, TV_LOW);
//	triggerAddEvent(s, a + w, T_SECONDARY, TV_HIGH);

	float z = 0.093;

	a = 180;
	triggerAddEvent(s, a - z * 720, T_PRIMARY, TV_HIGH);
	triggerAddEvent(s, a, T_PRIMARY, TV_LOW);

	a += 180;
	triggerAddEvent(s, a - z * 720, T_PRIMARY, TV_HIGH);
	triggerAddEvent(s, a, T_PRIMARY, TV_LOW);

	a += 180;
	triggerAddEvent(s, a - z * 720, T_PRIMARY, TV_HIGH);
	triggerAddEvent(s, a, T_PRIMARY, TV_LOW);

	a += 180;
	triggerAddEvent(s, a - z * 720, T_PRIMARY, TV_HIGH);
	triggerAddEvent(s, a, T_PRIMARY, TV_LOW);


//	s->shaftPositionEventCount = 2 + 8;
	s->shaftPositionEventCount = 8;
	engineConfiguration->triggerConfig.isSynchronizationNeeded = FALSE;
}
