/**
 * @file	trigger_mazda.cpp
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

void initializeMazdaMiataNbShape(trigger_config_s *triggerConfig, trigger_shape_s *s) {
	s->reset(FOUR_STROKE_CAM_SENSOR);

	/**
	 * cam sensor is primary, crank sensor is secondary
	 */
	s->addEvent(20, T_PRIMARY, TV_HIGH);

	s->addEvent(66, T_SECONDARY, TV_LOW);
	s->addEvent(70, T_SECONDARY, TV_HIGH);
	s->addEvent(136, T_SECONDARY, TV_LOW);
	s->addEvent(140, T_SECONDARY, TV_HIGH);
	s->addEvent(246, T_SECONDARY, TV_LOW);
	s->addEvent(250, T_SECONDARY, TV_HIGH);
	s->addEvent(316, T_SECONDARY, TV_LOW);
	s->addEvent(320, T_SECONDARY, TV_HIGH);

	s->addEvent(340, T_PRIMARY, TV_LOW);
	s->addEvent(360, T_PRIMARY, TV_HIGH);

	s->addEvent(380, T_PRIMARY, TV_LOW);
	s->addEvent(400, T_PRIMARY, TV_HIGH);

	s->addEvent(426, T_SECONDARY, TV_LOW);
	s->addEvent(430, T_SECONDARY, TV_HIGH);
	s->addEvent(496, T_SECONDARY, TV_LOW);
	s->addEvent(500, T_SECONDARY, TV_HIGH);
	s->addEvent(606, T_SECONDARY, TV_LOW);
	s->addEvent(610, T_SECONDARY, TV_HIGH);
	s->addEvent(676, T_SECONDARY, TV_LOW);
	s->addEvent(680, T_SECONDARY, TV_HIGH);

	s->addEvent(720, T_PRIMARY, TV_LOW);

	s->shaftPositionEventCount = 6 + 16;
}

void configureMazdaProtegeLx(trigger_config_s *triggerConfig, trigger_shape_s *s) {

	s->reset(FOUR_STROKE_CAM_SENSOR);
//	s->initialState[0] = 1;

	float w = 720 / 4 * 0.215;
	float a = 5;
//	s->addEvent(a, T_SECONDARY, TV_LOW);
//	s->addEvent(a + w, T_SECONDARY, TV_HIGH);
//	a += 180;
//	s->addEvent(a, T_SECONDARY, TV_LOW);
//	s->addEvent(a + w, T_SECONDARY, TV_HIGH);
//	a += 180;
//	s->addEvent(a, T_SECONDARY, TV_LOW);
//	s->addEvent(a + w, T_SECONDARY, TV_HIGH);
//	a += 180;
//	s->addEvent(a, T_SECONDARY, TV_LOW);
//	s->addEvent(a + w, T_SECONDARY, TV_HIGH);

	float z = 0.093;

	a = 180;
	s->addEvent(a - z * 720, T_PRIMARY, TV_HIGH);
	s->addEvent(a, T_PRIMARY, TV_LOW);

	a += 180;
	s->addEvent(a - z * 720, T_PRIMARY, TV_HIGH);
	s->addEvent(a, T_PRIMARY, TV_LOW);

	a += 180;
	s->addEvent(a - z * 720, T_PRIMARY, TV_HIGH);
	s->addEvent(a, T_PRIMARY, TV_LOW);

	a += 180;
	s->addEvent(a - z * 720, T_PRIMARY, TV_HIGH);
	s->addEvent(a, T_PRIMARY, TV_LOW);


//	s->shaftPositionEventCount = 2 + 8;
	s->shaftPositionEventCount = 8;
	triggerConfig->isSynchronizationNeeded = false;
}
