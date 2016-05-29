/**
 * @file	trigger_gm.cpp
 *
 * @date Mar 28, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "trigger_gm.h"

// GM7X looks like this is not complete
void configureGmTriggerShape(TriggerShape *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, false);

	// all angles are x2 here - so, 5 degree width is 10
	float w = 10;

	s->addEvent(120 - w, T_PRIMARY, TV_RISE);
	s->addEvent(120.0, T_PRIMARY, TV_FALL);

	s->addEvent(240 - w, T_PRIMARY, TV_RISE);
	s->addEvent(240.0, T_PRIMARY, TV_FALL);

	s->addEvent(360 - w, T_PRIMARY, TV_RISE);
	s->addEvent(360.0, T_PRIMARY, TV_FALL);

	s->addEvent(480 - w, T_PRIMARY, TV_RISE);
	s->addEvent(480.0, T_PRIMARY, TV_FALL);

	s->addEvent(600 - w, T_PRIMARY, TV_RISE);
	s->addEvent(600.0, T_PRIMARY, TV_FALL);

	s->addEvent(700 - w, T_PRIMARY, TV_RISE);
	s->addEvent(700.0, T_PRIMARY, TV_FALL);

	s->addEvent(720 - w, T_PRIMARY, TV_RISE);
	s->addEvent(720.0, T_PRIMARY, TV_FALL);
}

/**
 * https://www.mediafire.com/?40mfgeoe4ctti
 * http://www.ls1gto.com/forums/archive/index.php/t-190549.htm
 * http://www.ls2.com/forums/showthread.php/834483-LS-Timing-Reluctor-Wheels-Explained
 *
 */
void initGmLS24(TriggerShape *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, false);
	trigger_wheel_e ch = T_PRIMARY;

	/**
	 * based on http://rusefi.com/forum/viewtopic.php?f=3&t=936&start=30 logs
	 */
	s->addEvent(22.2733333333334, ch, TV_RISE);
	s->addEvent(27.59333333333338, ch, TV_FALL);
	s->addEvent(51.18333333333338, ch, TV_RISE);
	s->addEvent(57.58333333333338, ch, TV_FALL);
	s->addEvent(81.17333333333337, ch, TV_RISE);
	s->addEvent(87.61333333333339, ch, TV_FALL);
	s->addEvent(111.30666666666671, ch, TV_RISE);
	s->addEvent(117.81000000000004, ch, TV_FALL);
	s->addEvent(141.50000000000006, ch, TV_RISE);
	s->addEvent(148.05666666666673, ch, TV_FALL);
	s->addEvent(153.41333333333338, ch, TV_RISE);
	s->addEvent(178.29333333333338, ch, TV_FALL);
	s->addEvent(183.51000000000005, ch, TV_RISE);
	s->addEvent(208.3266666666667, ch, TV_FALL);
	s->addEvent(213.50000000000006, ch, TV_RISE);
	s->addEvent(238.26000000000005, ch, TV_FALL);
	s->addEvent(243.51000000000005, ch, TV_RISE);
	s->addEvent(268.27000000000004, ch, TV_FALL);
	s->addEvent(273.53666666666675, ch, TV_RISE);
	s->addEvent(298.35, ch, TV_FALL);
	s->addEvent(321.86333333333334, ch, TV_RISE);
	s->addEvent(328.4966666666667, ch, TV_FALL);
	s->addEvent(333.81000000000006, ch, TV_RISE);
	s->addEvent(358.66, ch, TV_FALL);
	s->addEvent(363.8633333333334, ch, TV_RISE);
	s->addEvent(388.7033333333334, ch, TV_FALL);
	s->addEvent(393.88000000000005, ch, TV_RISE);
	s->addEvent(418.62000000000006, ch, TV_FALL);
	s->addEvent(441.9566666666667, ch, TV_RISE);
	s->addEvent(448.6700000000001, ch, TV_FALL);
	s->addEvent(472.1000000000001, ch, TV_RISE);
	s->addEvent(478.9333333333334, ch, TV_FALL);
	s->addEvent(484.08000000000004, ch, TV_RISE);
	s->addEvent(509.1333333333334, ch, TV_FALL);
	s->addEvent(514.2666666666667, ch, TV_RISE);
	s->addEvent(539.1733333333334, ch, TV_FALL);
	s->addEvent(562.5166666666668, ch, TV_RISE);
	s->addEvent(569.1133333333333, ch, TV_FALL);
	s->addEvent(592.5400000000001, ch, TV_RISE);
	s->addEvent(599.1433333333334, ch, TV_FALL);
	s->addEvent(622.6300000000001, ch, TV_RISE);
	s->addEvent(629.2633333333334, ch, TV_FALL);
	s->addEvent(634.6, ch, TV_RISE);
	s->addEvent(659.5, ch, TV_FALL);
	s->addEvent(683.1133333333333, ch, TV_RISE);
	s->addEvent(689.77, ch, TV_FALL);
	s->addEvent(695.0666666666667, ch, TV_RISE);
	s->addEvent(720.0, ch, TV_FALL);

	s->useOnlyPrimaryForSync = true;

	s->setTriggerSynchronizationGap(1.6);
	s->setSecondTriggerSynchronizationGap(1);
	s->setThirdTriggerSynchronizationGap(2.5);
}

