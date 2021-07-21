/**
 * @file	trigger_mazda.cpp
 *
 * @date Feb 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
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
#include "error_handling.h"

#define NB_CRANK_MAGIC 70

void initializeMazdaMiataNaShape(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	// nominal gap is 0.325
	s->setTriggerSynchronizationGap2(0.1, 0.4);
	// nominal gap is ~1.6
	s->setSecondTriggerSynchronizationGap2(1.2, 1.8);

	s->useRiseEdge = false;

	s->bothFrontsRequired = true;
	s->gapBothDirections = true;

	s->tdcPosition = 5.181;

	/**
	 * http://rusefi.com/forum/viewtopic.php?f=3&t=729&p=12983#p12983
	 */
	s->addEvent720(52.960405, T_SECONDARY, TV_RISE);
	s->addEvent720(122.635956, T_SECONDARY, TV_FALL);

	s->addEvent720(216.897031, T_PRIMARY, TV_RISE);
	s->addEvent720(232.640068, T_SECONDARY, TV_RISE);
	s->addEvent720(288.819688, T_PRIMARY, TV_FALL);		// <-- This edge is the sync point
	s->addEvent720(302.646323, T_SECONDARY, TV_FALL);

	s->addEvent720(412.448056, T_SECONDARY, TV_RISE);
	s->addEvent720(482.816719, T_SECONDARY, TV_FALL);

	s->addEvent720(577.035495, T_PRIMARY, TV_RISE);
	s->addEvent720(592.878113, T_SECONDARY, TV_RISE);
	s->addEvent720(662.899708, T_SECONDARY, TV_FALL);
	s->addEvent720(720.0f, T_PRIMARY, TV_FALL);
	s->useOnlyPrimaryForSync = true;
}

// TT_MIATA_VVT
void initializeMazdaMiataNb2Crank(TriggerWaveform *s) {
	/**
	 * Note how we use 0..180 range while defining FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR trigger
	 * Note that only half of the physical wheel is defined here!
	 */
	s->initialize(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR);

	s->useRiseEdge = true;

	s->tdcPosition = 60 + 655;

	// Nominal gap 0.63
	s->setTriggerSynchronizationGap2(0.5f, 0.9f);
	// Nominal gap 1.58
	s->setSecondTriggerSynchronizationGap2(1.05f, 1.7f);

	// todo: NB2 fronts are inverted comparing to NB1, life is not perfect :(
	s->addEventAngle(180.0f - NB_CRANK_MAGIC - 4, T_PRIMARY, TV_FALL);
	s->addEventAngle(180.0f - NB_CRANK_MAGIC, T_PRIMARY, TV_RISE);
	s->addEventAngle(180.0f - 4, T_PRIMARY, TV_FALL);
	s->addEventAngle(180.0f, T_PRIMARY, TV_RISE);
}

static void addNBCrankTooth(TriggerWaveform *s, angle_t angle, trigger_wheel_e const channelIndex) {
	s->addEvent720(angle, channelIndex, TV_RISE);
	s->addEvent720(angle + 4, channelIndex, TV_FALL);
}

static void initializeMazdaMiataNb1ShapeWithOffset(TriggerWaveform *s, float offset) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->setTriggerSynchronizationGap3(0, 0.065, 0.17f);
	s->useRiseEdge = false;
	s->bothFrontsRequired = true;
	s->useOnlyPrimaryForSync = true;
	efiAssertVoid(OBD_PCM_Processor_Fault, s->gapBothDirections == false, "NB1 trigger measures on FALL events");

	s->tdcPosition = 276;

	/**
	 * cam sensor is primary, crank sensor is secondary
	 */
	s->addEvent720(20.0f, T_PRIMARY, TV_FALL);

	addNBCrankTooth(s, offset + 66.0f, T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + NB_CRANK_MAGIC, T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 180, T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 180 + NB_CRANK_MAGIC, T_SECONDARY);

	s->addEvent720(340.0f, T_PRIMARY, TV_RISE);
	s->addEvent720(360.0f, T_PRIMARY, TV_FALL);

	s->addEvent720(380.0f, T_PRIMARY, TV_RISE);
	s->addEvent720(400.0f, T_PRIMARY, TV_FALL);

	addNBCrankTooth(s, offset + 66.0f + 360, T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 360 + NB_CRANK_MAGIC, T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 540, T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 540 + NB_CRANK_MAGIC, T_SECONDARY);

	s->addEvent720(720.0f, T_PRIMARY, TV_RISE);
}

void initializeMazdaMiataNb1Shape(TriggerWaveform *s) {
	initializeMazdaMiataNb1ShapeWithOffset(s, 0);
}

void initializeMazdaMiataVVtTestShape(TriggerWaveform *s) {
	initializeMazdaMiataNb1ShapeWithOffset(s, -22);
}

void configureMazdaProtegeSOHC(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
//	s->initialState[0] = 1;

//	float w = 720 / 4 * 0.215;
	float a = 5;

	s->bothFrontsRequired = true;

	float z = 0.093;
	a = 180;
	s->addEvent720(a - z * 720, T_PRIMARY, TV_RISE);
	s->addEvent720(a, T_PRIMARY, TV_FALL);

	a += 180;
	s->addEvent720(a - z * 720, T_PRIMARY, TV_RISE);
	s->addEvent720(a, T_PRIMARY, TV_FALL);
	a += 180;
	s->addEvent720(a - z * 720, T_PRIMARY, TV_RISE);
	s->addEvent720(a, T_PRIMARY, TV_FALL);
	a += 180;
	s->addEvent720(a - z * 720, T_PRIMARY, TV_RISE);
	s->addEvent720(a, T_PRIMARY, TV_FALL);

	s->isSynchronizationNeeded = false;
	s->shapeWithoutTdc = true;
}

void configureMazdaProtegeLx(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	/**
	 * based on https://svn.code.sf.net/p/rusefi/code/trunk/misc/logs/1993_escort_gt/MAIN_rfi_report_2015-02-01%2017_39.csv
	 */
	s->addEvent720(95.329254, T_PRIMARY, TV_RISE);

	s->addEvent720(95.329254 + 14.876692, T_SECONDARY, TV_RISE);
	s->addEvent720(95.329254 + 82.693557, T_SECONDARY, TV_FALL);

	s->addEvent720(95.329254 + 137.119154, T_PRIMARY, TV_FALL);

	s->addEvent720(95.329254 + 192.378308, T_SECONDARY, TV_RISE);
	s->addEvent720(95.329254 + 261.556418, T_SECONDARY, TV_FALL);

	s->addEvent720(95.329254 + 373.060597, T_SECONDARY, TV_RISE);
	s->addEvent720(95.329254 + 443.503184, T_SECONDARY, TV_FALL);

	s->addEvent720(95.329254 + 555.349776, T_SECONDARY, TV_RISE);
	s->addEvent720(720, T_SECONDARY, TV_FALL);

	s->tdcPosition = 137.119154;
	s->isSynchronizationNeeded = false;
}

void initializeMazdaMiataVVtCamShape(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	// Nominal gap is 8.92
	s->setTriggerSynchronizationGap2(7, 13);
	// Nominal gap is 0.128
	s->setSecondTriggerSynchronizationGap2(0.06f, 0.16f);

	s->addEvent720(325, T_PRIMARY, TV_FALL);
	s->addEvent720(360, T_PRIMARY, TV_RISE);

	s->addEvent720(641, T_PRIMARY, TV_FALL);
	s->addEvent720(679, T_PRIMARY, TV_RISE);

	s->addEvent720(700, T_PRIMARY, TV_FALL);
	s->addEvent720(720, T_PRIMARY, TV_RISE);
}
