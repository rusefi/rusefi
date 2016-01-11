/**
 * @file	trigger_chrysler.cpp
 *
 * @date Mar 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "trigger_chrysler.h"
#include "trigger_decoder.h"

void initDodgeRam(TriggerShape *s DECLARE_ENGINE_PARAMETER_S) {

	s->initialize(FOUR_STROKE_CAM_SENSOR, true);
	s->useRiseEdge = true;

	s->isSynchronizationNeeded = false;

	addSkippedToothTriggerEvents(T_SECONDARY, s, 8, 0, 0.06, -25, 360, 0, 720);

	s->addEvent(360, T_PRIMARY, TV_RISE);

	addSkippedToothTriggerEvents(T_SECONDARY, s, 8, 0, 0.06, 360 - 25, 360, 0,
			720);

	s->addEvent(720, T_PRIMARY, TV_FALL);
}

void configureNeon2003TriggerShape(TriggerShape *s DECLARE_ENGINE_PARAMETER_S) {
	bool useOnlyPrimary = false;

	s->initialize(FOUR_STROKE_CAM_SENSOR, !useOnlyPrimary);
	s->useRiseEdge = true;
	s->gapBothDirections = false;

	s->tdcPosition = 496;

	/**
	 * ratio sequence is:
gap=2.17/1.46
gap=0.33/2.17
gap=1.06/0.33
gap=2.73/1.06
gap=0.47/2.73
gap=0.71/0.47
gap=1.43/0.71
	 *
	 */

	// these values only work well for 'useFronOnly' mode!!!
	s->setTriggerSynchronizationGap2(0.75, 1.5);
	s->setSecondTriggerSynchronizationGap2(0.65, 1.25);

	float EPS_ANGLE = 0.3;

	s->useOnlyPrimaryForSync = true;

	if (useOnlyPrimary) {
		s->addEvent(144, T_PRIMARY, TV_RISE);
		s->addEvent(180, T_PRIMARY, TV_FALL);

		s->addEvent(216, T_PRIMARY, TV_RISE);
		s->addEvent(252, T_PRIMARY, TV_FALL);

		s->addEvent(288, T_PRIMARY, TV_RISE);
		s->addEvent(324, T_PRIMARY, TV_FALL);

		s->addEvent(360, T_PRIMARY, TV_RISE); // width = 144
		s->addEvent(504, T_PRIMARY, TV_FALL); // width = 36
		s->addEvent(540, T_PRIMARY, TV_RISE);
		s->addEvent(576, T_PRIMARY, TV_FALL);
		s->addEvent(612, T_PRIMARY, TV_RISE);
		s->addEvent(648, T_PRIMARY, TV_FALL);
		s->addEvent(684, T_PRIMARY, TV_RISE);
		s->addEvent(720, T_PRIMARY, TV_FALL);

	} else {
		/**
		 * 7 primary (four narrow + one wide + two narrow)
		 * 64 secondary (one wide + 15 narrow + 16 narrow + one wide + 15 narrow + 16 narrow)
		 *
		 */

		s->addEvent(25, T_SECONDARY, TV_FALL);
		s->addEvent(30, T_SECONDARY, TV_RISE);
		s->addEvent(35, T_SECONDARY, TV_FALL);
		s->addEvent(40, T_SECONDARY, TV_RISE);
		s->addEvent(45, T_SECONDARY, TV_FALL);
		s->addEvent(50, T_SECONDARY, TV_RISE);
		s->addEvent(55, T_SECONDARY, TV_FALL);
		s->addEvent(60, T_SECONDARY, TV_RISE);
		s->addEvent(65, T_SECONDARY, TV_FALL);
		s->addEvent(70, T_SECONDARY, TV_RISE);
		s->addEvent(75, T_SECONDARY, TV_FALL);
		s->addEvent(80, T_SECONDARY, TV_RISE);
		s->addEvent(85, T_SECONDARY, TV_FALL);
		s->addEvent(90, T_SECONDARY, TV_RISE);
		s->addEvent(95, T_SECONDARY, TV_FALL);
		s->addEvent(100, T_SECONDARY, TV_RISE);
		s->addEvent(105, T_SECONDARY, TV_FALL);
		s->addEvent(110, T_SECONDARY, TV_RISE);
		s->addEvent(115, T_SECONDARY, TV_FALL);
		s->addEvent(120, T_SECONDARY, TV_RISE);
		s->addEvent(125, T_SECONDARY, TV_FALL);
		s->addEvent(130, T_SECONDARY, TV_RISE);
		s->addEvent(135, T_SECONDARY, TV_FALL);
		s->addEvent(140, T_SECONDARY, TV_RISE);

		s->addEvent(144, T_PRIMARY, TV_RISE);

		s->addEvent(145,T_SECONDARY, TV_FALL);
		s->addEvent(150, T_SECONDARY, TV_RISE);
		s->addEvent(155, T_SECONDARY, TV_FALL);
		s->addEvent(160, T_SECONDARY, TV_RISE);
		s->addEvent(165, T_SECONDARY, TV_FALL);
		s->addEvent(170, T_SECONDARY, TV_RISE);
		s->addEvent(175, T_SECONDARY, TV_FALL);
		s->addEvent(180 - EPS_ANGLE, T_SECONDARY, TV_RISE);

		s->addEvent(180, T_PRIMARY, TV_FALL);

		s->addEvent(185, T_SECONDARY, TV_FALL);
		s->addEvent(210, T_SECONDARY, TV_RISE);
		s->addEvent(215, T_SECONDARY, TV_FALL);

		s->addEvent(216, T_PRIMARY, TV_RISE);

		s->addEvent(220, T_SECONDARY, TV_RISE);
		s->addEvent(225, T_SECONDARY, TV_FALL);
		s->addEvent(230, T_SECONDARY, TV_RISE);
		s->addEvent(235, T_SECONDARY, TV_FALL);
		s->addEvent(240, T_SECONDARY, TV_RISE);
		s->addEvent(245, T_SECONDARY, TV_FALL);
		s->addEvent(250, T_SECONDARY, TV_RISE);

		s->addEvent(252, T_PRIMARY, TV_FALL);

		s->addEvent(255, T_SECONDARY, TV_FALL);

		s->addEvent(260, T_SECONDARY, TV_RISE);
		s->addEvent(265, T_SECONDARY, TV_FALL);
		s->addEvent(270, T_SECONDARY, TV_RISE);
		s->addEvent(275, T_SECONDARY, TV_FALL);
		s->addEvent(280, T_SECONDARY, TV_RISE);
		s->addEvent(285, T_SECONDARY, TV_FALL);

		s->addEvent(288, T_PRIMARY, TV_RISE);

		s->addEvent(290, T_SECONDARY, TV_RISE);
		s->addEvent(295, T_SECONDARY, TV_FALL);
		s->addEvent(300, T_SECONDARY, TV_RISE);
		s->addEvent(305, T_SECONDARY, TV_FALL);
		s->addEvent(310, T_SECONDARY, TV_RISE);
		s->addEvent(315, T_SECONDARY, TV_FALL);
		s->addEvent(320, T_SECONDARY, TV_RISE);

		s->addEvent(324, T_PRIMARY, TV_FALL);

		s->addEvent(325, T_SECONDARY, TV_FALL);
		s->addEvent(330, T_SECONDARY, TV_RISE);
		s->addEvent(335, T_SECONDARY, TV_FALL);
		s->addEvent(340, T_SECONDARY, TV_RISE);
		s->addEvent(345, T_SECONDARY, TV_FALL);
		s->addEvent(350, T_SECONDARY, TV_RISE);
		s->addEvent(355, T_SECONDARY, TV_FALL);
		s->addEvent(360 - EPS_ANGLE, T_SECONDARY, TV_RISE);

		s->addEvent(360, T_PRIMARY, TV_RISE);

		s->addEvent(385, T_SECONDARY, TV_FALL);
		s->addEvent(390, T_SECONDARY, TV_RISE);
		s->addEvent(395, T_SECONDARY, TV_FALL);
		s->addEvent(400, T_SECONDARY, TV_RISE);
		s->addEvent(405, T_SECONDARY, TV_FALL);
		s->addEvent(410, T_SECONDARY, TV_RISE);
		s->addEvent(415, T_SECONDARY, TV_FALL);
		s->addEvent(420, T_SECONDARY, TV_RISE);
		s->addEvent(425, T_SECONDARY, TV_FALL);
		s->addEvent(430, T_SECONDARY, TV_RISE);
		s->addEvent(435, T_SECONDARY, TV_FALL);
		s->addEvent(440, T_SECONDARY, TV_RISE);
		s->addEvent(445, T_SECONDARY, TV_FALL);
		s->addEvent(450, T_SECONDARY, TV_RISE);
		s->addEvent(455, T_SECONDARY, TV_FALL);
		s->addEvent(460, T_SECONDARY, TV_RISE);
		s->addEvent(465, T_SECONDARY, TV_FALL);
		s->addEvent(470, T_SECONDARY, TV_RISE);
		s->addEvent(475, T_SECONDARY, TV_FALL);
		s->addEvent(480, T_SECONDARY, TV_RISE);
		s->addEvent(485, T_SECONDARY, TV_FALL);
		s->addEvent(490, T_SECONDARY, TV_RISE);
		s->addEvent(495, T_SECONDARY, TV_FALL);
		s->addEvent(500, T_SECONDARY, TV_RISE);

		s->addEvent(504, T_PRIMARY, TV_FALL);

		s->addEvent(505, T_SECONDARY, TV_FALL);
		s->addEvent(510, T_SECONDARY, TV_RISE);
		s->addEvent(515, T_SECONDARY, TV_FALL);
		s->addEvent(520, T_SECONDARY, TV_RISE);
		s->addEvent(525, T_SECONDARY, TV_FALL);
		s->addEvent(530, T_SECONDARY, TV_RISE);
		s->addEvent(535, T_SECONDARY, TV_FALL);
		s->addEvent(540 - EPS_ANGLE, T_SECONDARY, TV_RISE);

		s->addEvent(540, T_PRIMARY, TV_RISE);

		s->addEvent(545, T_SECONDARY, TV_FALL);
		s->addEvent(570, T_SECONDARY, TV_RISE);
		s->addEvent(575, T_SECONDARY, TV_FALL);

		s->addEvent(576, T_PRIMARY, TV_FALL);
		s->addEvent(580, T_SECONDARY, TV_RISE);
		s->addEvent(585, T_SECONDARY, TV_FALL);
		s->addEvent(590, T_SECONDARY, TV_RISE);
		s->addEvent(595, T_SECONDARY, TV_FALL);
		s->addEvent(600, T_SECONDARY, TV_RISE);
		s->addEvent(605, T_SECONDARY, TV_FALL);
		s->addEvent(610, T_SECONDARY, TV_RISE);

		s->addEvent(612, T_PRIMARY, TV_RISE);
		s->addEvent(615, T_SECONDARY, TV_FALL);
		s->addEvent(620, T_SECONDARY, TV_RISE);
		s->addEvent(625, T_SECONDARY, TV_FALL);
		s->addEvent(630, T_SECONDARY, TV_RISE);
		s->addEvent(635, T_SECONDARY, TV_FALL);
		s->addEvent(640, T_SECONDARY, TV_RISE);
		s->addEvent(645, T_SECONDARY, TV_FALL);

		s->addEvent(648, T_PRIMARY, TV_FALL);
		s->addEvent(650, T_SECONDARY, TV_RISE);
		s->addEvent(655, T_SECONDARY, TV_FALL);
		s->addEvent(660, T_SECONDARY, TV_RISE);
		s->addEvent(665, T_SECONDARY, TV_FALL);
		s->addEvent(670, T_SECONDARY, TV_RISE);
		s->addEvent(675, T_SECONDARY, TV_FALL);
		s->addEvent(680, T_SECONDARY, TV_RISE);

		s->addEvent(684, T_PRIMARY, TV_RISE);
		s->addEvent(685, T_SECONDARY, TV_FALL);
		s->addEvent(690, T_SECONDARY, TV_RISE);
		s->addEvent(695, T_SECONDARY, TV_FALL);
		s->addEvent(700, T_SECONDARY, TV_RISE);
		s->addEvent(705, T_SECONDARY, TV_FALL);
		s->addEvent(710, T_SECONDARY, TV_RISE);
		s->addEvent(715, T_SECONDARY, TV_FALL);
		s->addEvent(720 - EPS_ANGLE, T_SECONDARY, TV_RISE);
		s->addEvent(720, T_PRIMARY, TV_FALL);
	}
}

void configureDodgeStratusTriggerShape(TriggerShape *s DECLARE_ENGINE_PARAMETER_S) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, false);
	s->isSynchronizationNeeded = true;

	s->tdcPosition = 150;

	float w = 7;
	float g = 20;
	s->setTriggerSynchronizationGap2(CHRYSLER_NGC6_GAP - 0.25,
			CHRYSLER_NGC6_GAP + 0.4);

	float base = 0;
	// 2 teeth
	float angle = base + 120.0 - w;
	s->addEvent(angle, T_PRIMARY, TV_RISE);
	s->addEvent(angle + w, T_PRIMARY, TV_FALL);
	angle += g;
	s->addEvent(angle, T_PRIMARY, TV_RISE);
	s->addEvent(angle + w, T_PRIMARY, TV_FALL);

	base += 120;
	// 3 teeth
	angle = base + 120.0 - w;
	s->addEvent(angle, T_PRIMARY, TV_RISE);
	s->addEvent(angle + w, T_PRIMARY, TV_FALL);
	angle += g;
	s->addEvent(angle, T_PRIMARY, TV_RISE);
	s->addEvent(angle + w, T_PRIMARY, TV_FALL);
	angle += g;
	s->addEvent(angle, T_PRIMARY, TV_RISE);
	s->addEvent(angle + w, T_PRIMARY, TV_FALL);

	base += 120;
	// 2 teeth
	angle = base + 120.0 - w;
	s->addEvent(angle, T_PRIMARY, TV_RISE);
	s->addEvent(angle + w, T_PRIMARY, TV_FALL);
	angle += g;
	s->addEvent(angle, T_PRIMARY, TV_RISE);
	s->addEvent(angle + w, T_PRIMARY, TV_FALL);

	base += 120;
	// just one
	angle = base + 120.0 - w;
	s->addEvent(angle, T_PRIMARY, TV_RISE);
	s->addEvent(angle + w, T_PRIMARY, TV_FALL);

	base += 120;
	// 3 teeth
	angle = base + 120.0 - w;
	s->addEvent(angle, T_PRIMARY, TV_RISE);
	s->addEvent(angle + w, T_PRIMARY, TV_FALL);
	angle += g;
	s->addEvent(angle, T_PRIMARY, TV_RISE);
	s->addEvent(angle + w, T_PRIMARY, TV_FALL);
	angle += g;
	s->addEvent(angle, T_PRIMARY, TV_RISE);
	s->addEvent(angle + w, T_PRIMARY, TV_FALL);

	base += 120;
	// just one again
	angle = base + 120.0 - w;
	s->addEvent(angle, T_PRIMARY, TV_RISE);
	s->addEvent(angle + w, T_PRIMARY, TV_FALL);
}

void configureNeon1995TriggerShape(TriggerShape *s DECLARE_ENGINE_PARAMETER_S) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	s->setTriggerSynchronizationGap(0.72);

	s->useRiseEdge = false;

	// voodoo magic - we always need 720 at the end
	int base = 720 - 560;

	s->initialState[T_PRIMARY] = 1;

	s->addEvent(base - 720 + 600, T_SECONDARY, TV_RISE);
	s->addEvent(base - 720 + 604, T_SECONDARY, TV_FALL);
	s->addEvent(base - 720 + 616, T_SECONDARY, TV_RISE);
	s->addEvent(base - 720 + 620, T_SECONDARY, TV_FALL);
	s->addEvent(base - 720 + 643, T_SECONDARY, TV_RISE);
	s->addEvent(base - 720 + 648, T_SECONDARY, TV_FALL);
	s->addEvent(base - 720 + 671, T_SECONDARY, TV_RISE);
	s->addEvent(base - 720 + 676, T_SECONDARY, TV_FALL);

	s->addEvent(base + 0, T_PRIMARY, TV_FALL);

	s->addEvent(base + 20, T_SECONDARY, TV_RISE);
	s->addEvent(base + 60, T_SECONDARY, TV_FALL);
	s->addEvent(base + 75, T_SECONDARY, TV_RISE);
	s->addEvent(base + 79, T_SECONDARY, TV_FALL);
	s->addEvent(base + 101, T_SECONDARY, TV_RISE);
	s->addEvent(base + 106, T_SECONDARY, TV_FALL);
	s->addEvent(base + 130, T_SECONDARY, TV_RISE);
	s->addEvent(base + 135, T_SECONDARY, TV_FALL);

	s->addEvent(base + 200, T_PRIMARY, TV_RISE); // width = 150

	s->addEvent(base + 236, T_SECONDARY, TV_RISE);
	s->addEvent(base + 239, T_SECONDARY, TV_FALL);
	s->addEvent(base + 250, T_SECONDARY, TV_RISE);
	s->addEvent(base + 255, T_SECONDARY, TV_FALL);
	s->addEvent(base + 277, T_SECONDARY, TV_RISE);
	s->addEvent(base + 282, T_SECONDARY, TV_FALL);
	s->addEvent(base + 305, T_SECONDARY, TV_RISE);
	s->addEvent(base + 310, T_SECONDARY, TV_FALL);

	s->addEvent(base + 374, T_SECONDARY, TV_RISE);

	s->addEvent(base + 395, T_PRIMARY, TV_FALL); // width =

	s->addEvent(base + 418, T_SECONDARY, TV_FALL);
	s->addEvent(base + 436, T_SECONDARY, TV_RISE);
	s->addEvent(base + 441, T_SECONDARY, TV_FALL);
	s->addEvent(base + 463, T_SECONDARY, TV_RISE);
	s->addEvent(base + 468, T_SECONDARY, TV_FALL);
	s->addEvent(base + 492, T_SECONDARY, TV_RISE);
	s->addEvent(base + 497, T_SECONDARY, TV_FALL);

	s->addEvent(base + 560, T_PRIMARY, TV_RISE); // width =
}

