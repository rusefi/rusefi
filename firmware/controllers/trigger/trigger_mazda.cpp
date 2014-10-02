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

void initializeMazdaMiataNaShape(trigger_shape_s *s) {
	s->reset(FOUR_STROKE_CAM_SENSOR);
	setTriggerSynchronizationGap(s, 1.68f);
	s->useRiseEdge = false;

	s->isSynchronizationNeeded = true;

	s->addEvent(49.15363636, T_SECONDARY, TV_HIGH);
	s->addEvent(114.1581818, T_SECONDARY, TV_LOW);

	s->addEvent(213.0222222, T_PRIMARY, TV_HIGH);
	s->addEvent(225.759, T_SECONDARY, TV_HIGH);
	s->addEvent(279.54375, T_PRIMARY, TV_LOW);
	s->addEvent(294.786, T_SECONDARY, TV_LOW);

	s->addEvent(410.318, T_SECONDARY, TV_HIGH);
	s->addEvent(477.6911111, T_SECONDARY, TV_LOW);

	s->addEvent(576.4975, T_PRIMARY, TV_HIGH);
	s->addEvent(590.39625, T_SECONDARY, TV_HIGH);
	s->addEvent(656.5125, T_SECONDARY, TV_LOW);
	s->addEvent(720.0f, T_PRIMARY, TV_LOW);

	s->assignSize();
}

void initializeMazdaMiataNbShape(trigger_shape_s *s) {
	setTriggerSynchronizationGap(s, 0.11f);
	s->useRiseEdge = false;

	s->reset(FOUR_STROKE_CAM_SENSOR);

	/**
	 * cam sensor is primary, crank sensor is secondary
	 */
	s->addEvent(20.0f, T_PRIMARY, TV_HIGH);

	s->addEvent(66.0f, T_SECONDARY, TV_LOW);
	s->addEvent(70.0f, T_SECONDARY, TV_HIGH);
	s->addEvent(136.0f, T_SECONDARY, TV_LOW);
	s->addEvent(140.0f, T_SECONDARY, TV_HIGH);
	s->addEvent(246.0f, T_SECONDARY, TV_LOW);
	s->addEvent(250.0f, T_SECONDARY, TV_HIGH);
	s->addEvent(316.0f, T_SECONDARY, TV_LOW);
	s->addEvent(320.0f, T_SECONDARY, TV_HIGH);

	s->addEvent(340.0f, T_PRIMARY, TV_LOW);
	s->addEvent(360.0f, T_PRIMARY, TV_HIGH);

	s->addEvent(380.0f, T_PRIMARY, TV_LOW);
	s->addEvent(400.0f, T_PRIMARY, TV_HIGH);

	s->addEvent(426.0f, T_SECONDARY, TV_LOW);
	s->addEvent(430.0f, T_SECONDARY, TV_HIGH);
	s->addEvent(496.0f, T_SECONDARY, TV_LOW);
	s->addEvent(500.0f, T_SECONDARY, TV_HIGH);
	s->addEvent(606.0f, T_SECONDARY, TV_LOW);
	s->addEvent(610.0f, T_SECONDARY, TV_HIGH);
	s->addEvent(676.0f, T_SECONDARY, TV_LOW);
	s->addEvent(680.0f, T_SECONDARY, TV_HIGH);

	s->addEvent(720.0f, T_PRIMARY, TV_LOW);

	s->assignSize();
}

void configureMazdaProtegeLx(trigger_shape_s *s) {

	// todo: move to into configuration definition s->needSecondTriggerInput = FALSE;


	s->reset(FOUR_STROKE_CAM_SENSOR);
//	s->initialState[0] = 1;

//	float w = 720 / 4 * 0.215;
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


	s->assignSize();
	s->isSynchronizationNeeded = false;
}
