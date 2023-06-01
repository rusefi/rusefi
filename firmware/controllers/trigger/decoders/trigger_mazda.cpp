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
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Both);
	s->isSecondWheelCam = true;

	// nominal gap is 0.325
	s->setTriggerSynchronizationGap2(0.1, 0.5);
	// nominal gap is ~1.52
	s->setSecondTriggerSynchronizationGap2(0.5, 2.3);

	s->tdcPosition = 5.181;

	/**
	 * http://rusefi.com/forum/viewtopic.php?f=3&t=729&p=12983#p12983
	 */
	s->addEvent720(52.960405, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(122.635956, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(216.897031, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(232.640068, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(288.819688, false, TriggerWheel::T_PRIMARY);		// <-- This edge is the sync point
	s->addEvent720(302.646323, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(412.448056, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(482.816719, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(577.035495, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(592.878113, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(662.899708, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(720.0f, false, TriggerWheel::T_PRIMARY);
	s->useOnlyPrimaryForSync = true;
}

// trigger_type_e::TT_MIATA_VVT
void initializeMazdaMiataNb2Crank(TriggerWaveform *s) {
	/**
	 * Note how we use 0..180 range while defining FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR trigger
	 * Note that only half of the physical wheel is defined here!
	 */
	s->initialize(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR, SyncEdge::RiseOnly);

	s->tdcPosition = 60 + 655;

	// Nominal gap 70/110 = 0.636
	s->setTriggerSynchronizationGap2(0.35f, 1.15f);
	// Nominal gap 110/70 = 1.571
	s->setSecondTriggerSynchronizationGap2(0.8f, 1.8f);

	// todo: NB2 fronts are inverted comparing to NB1, life is not perfect :(
	s->addEventAngle(180.0f - NB_CRANK_MAGIC - 4, false, TriggerWheel::T_PRIMARY);
	s->addEventAngle(180.0f - NB_CRANK_MAGIC, true, TriggerWheel::T_PRIMARY);
	s->addEventAngle(180.0f - 4, false, TriggerWheel::T_PRIMARY);
	s->addEventAngle(180.0f, true, TriggerWheel::T_PRIMARY);
}

static void addNBCrankTooth(TriggerWaveform *s, angle_t angle, TriggerWheel const channelIndex) {
	s->addEvent720(angle, true, channelIndex);
	s->addEvent720(angle + 4, false, channelIndex);
}

static void initializeMazdaMiataNb1ShapeWithOffset(TriggerWaveform *s, float offset) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Fall);
	s->setTriggerSynchronizationGap3(0, 0.065, 0.17f);
	s->useOnlyPrimaryForSync = true;

	s->tdcPosition = 276;

	/**
	 * cam sensor is primary, crank sensor is secondary
	 */
	s->addEvent720(20.0f, false, TriggerWheel::T_PRIMARY);

	addNBCrankTooth(s, offset + 66.0f, TriggerWheel::T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + NB_CRANK_MAGIC, TriggerWheel:: T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 180, TriggerWheel:: T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 180 + NB_CRANK_MAGIC, TriggerWheel:: T_SECONDARY);

	s->addEvent720(340.0f, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(360.0f, false, TriggerWheel::T_PRIMARY);

	s->addEvent720(380.0f, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(400.0f, false, TriggerWheel::T_PRIMARY);

	addNBCrankTooth(s, offset + 66.0f + 360, TriggerWheel:: T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 360 + NB_CRANK_MAGIC, TriggerWheel:: T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 540, TriggerWheel:: T_SECONDARY);
	addNBCrankTooth(s, offset + 66.0f + 540 + NB_CRANK_MAGIC, TriggerWheel:: T_SECONDARY);

	s->addEvent720(720.0f, true, TriggerWheel::T_PRIMARY);
}

void initializeMazdaMiataVVtTestShape(TriggerWaveform *s) {
	initializeMazdaMiataNb1ShapeWithOffset(s, -22);
}

void configureMazdaProtegeSOHC(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Both);

//	float w = 720 / 4 * 0.215;
	float a = 5;

	float z = 0.093;
	a = 180;
	s->addEvent720(a - z * 720, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(a, false, TriggerWheel::T_PRIMARY);

	a += 180;
	s->addEvent720(a - z * 720, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(a, false, TriggerWheel::T_PRIMARY);
	a += 180;
	s->addEvent720(a - z * 720, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(a, false, TriggerWheel::T_PRIMARY);
	a += 180;
	s->addEvent720(a - z * 720, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(a, false, TriggerWheel::T_PRIMARY);

	s->isSynchronizationNeeded = false;
	s->shapeWithoutTdc = true;
}

void configureMazdaProtegeLx(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	s->isSecondWheelCam = true;
	/**
	 * based on https://svn.code.sf.net/p/rusefi/code/trunk/misc/logs/1993_escort_gt/MAIN_rfi_report_2015-02-01%2017_39.csv
	 */
	s->addEvent720(95.329254, true, TriggerWheel::T_PRIMARY);

	s->addEvent720(95.329254 + 14.876692, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(95.329254 + 82.693557, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(95.329254 + 137.119154, false, TriggerWheel::T_PRIMARY);

	s->addEvent720(95.329254 + 192.378308, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(95.329254 + 261.556418, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(95.329254 + 373.060597, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(95.329254 + 443.503184, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(95.329254 + 555.349776, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(720, false, TriggerWheel::T_SECONDARY);

	s->tdcPosition = 137.119154;
	s->isSynchronizationNeeded = false;
}

void initializeMazdaMiataVVtCamShape(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	// Nominal gap is 8.92
	s->setTriggerSynchronizationGap2(6, 20);
	// Nominal gap is 0.128
	s->setSecondTriggerSynchronizationGap2(0.04f, 0.2f);

	s->addEvent720(325, false, TriggerWheel::T_PRIMARY);
	s->addEvent720(360, true, TriggerWheel::T_PRIMARY);

	s->addEvent720(641, false, TriggerWheel::T_PRIMARY);
	s->addEvent720(679, true, TriggerWheel::T_PRIMARY);

	s->addEvent720(700, false, TriggerWheel::T_PRIMARY);
	s->addEvent720(720, true, TriggerWheel::T_PRIMARY);
}

// https://rusefi.com/forum/viewtopic.php?f=17&t=2417
// Cam pattern for intake/exhaust on all Skyactiv-G (and maybe -D/-X)
void initializeMazdaSkyactivCam(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

    // wide
	s->addEvent360(50, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(70, false, TriggerWheel::T_PRIMARY);
    // narrow
	s->addEvent360(80, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(90, false, TriggerWheel::T_PRIMARY);

    // wide
	s->addEvent360(140, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(160, false, TriggerWheel::T_PRIMARY);
    // narrow
	s->addEvent360(170, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(180, false, TriggerWheel::T_PRIMARY);

    // wide
	s->addEvent360(250, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(270, false, TriggerWheel::T_PRIMARY);

    // wide
	s->addEvent360(340, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(360, false, TriggerWheel::T_PRIMARY);

	s->setTriggerSynchronizationGap(0.43);
	s->setSecondTriggerSynchronizationGap(0.78);
	s->setThirdTriggerSynchronizationGap(1.12); // 3rd gap is not required but let's have it for some resiliency
}
