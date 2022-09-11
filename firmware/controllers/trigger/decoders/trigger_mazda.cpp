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

#include "pch.h"

#include "trigger_mazda.h"

#define NB_CRANK_MAGIC 70

void initializeMazdaMiataNaShape(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->isSecondWheelCam = true;

	// nominal gap is 0.325
	s->setTriggerSynchronizationGap2(0.1, 0.5);
	// nominal gap is ~1.52
	s->setSecondTriggerSynchronizationGap2(0.5, 2.3);

	s->useRiseEdge = false;

	s->bothFrontsRequired = true;
	s->gapBothDirections = true;

	s->tdcPosition = 5.181;

	/**
	 * http://rusefi.com/forum/viewtopic.php?f=3&t=729&p=12983#p12983
	 */
	s->addEvent720(52.960405, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(122.635956, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(216.897031, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(232.640068, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(288.819688, TriggerWheel::T_PRIMARY, TriggerValue::FALL);		// <-- This edge is the sync point
	s->addEvent720(302.646323, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(412.448056, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(482.816719, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(577.035495, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(592.878113, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(662.899708, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(720.0f, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->useOnlyPrimaryForSync = true;
}

// TT_MIATA_VVT
void initializeMazdaMiataNb2Crank(TriggerWaveform *s) {
	/**
	 * Note how we use 0..180 range while defining FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR trigger
	 * Note that only half of the physical wheel is defined here!
	 */
	s->initialize(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR);

	s->tdcPosition = 60 + 655;

	// Nominal gap 70/110 = 0.636
	s->setTriggerSynchronizationGap2(0.35f, 1.15f);
	// Nominal gap 110/70 = 1.571
	s->setSecondTriggerSynchronizationGap2(0.8f, 1.8f);

	// todo: NB2 fronts are inverted comparing to NB1, life is not perfect :(
	s->addEventAngle(180.0f - NB_CRANK_MAGIC - 4, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEventAngle(180.0f - NB_CRANK_MAGIC, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEventAngle(180.0f - 4, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEventAngle(180.0f, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
}

static void addNBCrankTooth(TriggerWaveform *s, angle_t angle, TriggerWheel const channelIndex) {
	s->addEvent720(angle, channelIndex, TriggerValue::RISE);
	s->addEvent720(angle + 4, channelIndex, TriggerValue::FALL);
}

static void initializeMazdaMiataNb1ShapeWithOffset(TriggerWaveform *s, float offset) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->setTriggerSynchronizationGap3(0, 0.065, 0.17f);
	s->useRiseEdge = false;
	s->useOnlyPrimaryForSync = true;
	efiAssertVoid(OBD_PCM_Processor_Fault, s->gapBothDirections == false, "NB1 trigger measures on FALL events");

	s->tdcPosition = 276;

	/**
	 * cam sensor is primary, crank sensor is secondary
	 */
	s->addEvent720(20.0f, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	addNBCrankTooth(s, offset + 66.0f, TriggerWheel::T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + NB_CRANK_MAGIC, TriggerWheel:: T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 180, TriggerWheel:: T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 180 + NB_CRANK_MAGIC, TriggerWheel:: T_SECONDARY);

	s->addEvent720(340.0f, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(360.0f, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	s->addEvent720(380.0f, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(400.0f, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	addNBCrankTooth(s, offset + 66.0f + 360, TriggerWheel:: T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 360 + NB_CRANK_MAGIC, TriggerWheel:: T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 540, TriggerWheel:: T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 540 + NB_CRANK_MAGIC, TriggerWheel:: T_SECONDARY);

	s->addEvent720(720.0f, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
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
	s->addEvent720(a - z * 720, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(a, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	a += 180;
	s->addEvent720(a - z * 720, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(a, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	a += 180;
	s->addEvent720(a - z * 720, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(a, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	a += 180;
	s->addEvent720(a - z * 720, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(a, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	s->isSynchronizationNeeded = false;
	s->shapeWithoutTdc = true;
}

void configureMazdaProtegeLx(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->isSecondWheelCam = true;
	/**
	 * based on https://svn.code.sf.net/p/rusefi/code/trunk/misc/logs/1993_escort_gt/MAIN_rfi_report_2015-02-01%2017_39.csv
	 */
	s->addEvent720(95.329254, TriggerWheel::T_PRIMARY, TriggerValue::RISE);

	s->addEvent720(95.329254 + 14.876692, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(95.329254 + 82.693557, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(95.329254 + 137.119154, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	s->addEvent720(95.329254 + 192.378308, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(95.329254 + 261.556418, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(95.329254 + 373.060597, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(95.329254 + 443.503184, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(95.329254 + 555.349776, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(720, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->tdcPosition = 137.119154;
	s->isSynchronizationNeeded = false;
}

void initializeMazdaMiataVVtCamShape(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	// Nominal gap is 8.92
	s->setTriggerSynchronizationGap2(6, 20);
	// Nominal gap is 0.128
	s->setSecondTriggerSynchronizationGap2(0.04f, 0.2f);

	s->addEvent720(325, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent720(360, TriggerWheel::T_PRIMARY, TriggerValue::RISE);

	s->addEvent720(641, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent720(679, TriggerWheel::T_PRIMARY, TriggerValue::RISE);

	s->addEvent720(700, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent720(720, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
}

// https://rusefi.com/forum/viewtopic.php?f=17&t=2417
// Cam pattern for intake/exhaust on all Skyactiv-G (and maybe -D/-X)
void initializeMazdaSkyactivCam(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

    // wide
	s->addEvent360(50, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(70, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
    // narrow
	s->addEvent360(80, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(90, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

    // wide
	s->addEvent360(140, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(160, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
    // narrow
	s->addEvent360(170, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(180, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

    // wide
	s->addEvent360(250, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(270, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

    // wide
	s->addEvent360(340, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(360, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	s->setTriggerSynchronizationGap(0.43);
	s->setSecondTriggerSynchronizationGap(0.78);
	s->setThirdTriggerSynchronizationGap(1.12); // 3rd gap is not required but let's have it for some resiliency
}
