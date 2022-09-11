/**
 * @file	trigger_chrysler.cpp
 *
 * @date Mar 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_chrysler.h"
#include "trigger_universal.h"

void initDodgeRam(TriggerWaveform *s) {

	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->useRiseEdge = true;

	s->isSynchronizationNeeded = false;

	addSkippedToothTriggerEvents(TriggerWheel::T_SECONDARY, s, 8, 0, 0.06, -25, 360, 0, 720);

	s->addEvent720(360, TriggerWheel::T_PRIMARY, TriggerValue::RISE);

	addSkippedToothTriggerEvents(TriggerWheel::T_SECONDARY, s, 8, 0, 0.06, 360 - 25, 360, 0, 720);

	s->addEvent720(720, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
}

void configureNeon2003TriggerWaveformCrank(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR);
	s->useRiseEdge = true;

	s->setTriggerSynchronizationGap(3);

	s->addEvent360(25, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(30, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(35, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(40, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(45, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(50, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(55, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(60, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(65, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(70, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(75, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(80, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(85, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(90, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(95, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(100, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(105, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(110, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(115, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(120, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(125, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(130, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(135, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(140, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(145,TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(150, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(155, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(160, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(165, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(170, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(175, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(180, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(185, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	// gap 25
	s->addEvent360(210, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(215, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(220, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(225, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(230, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(235, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(240, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(245, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(250, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(255, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(260, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(265, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(270, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(275, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(280, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(285, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(290, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(295, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(300, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(305, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(310, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(315, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(320, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(325, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(330, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(335, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(340, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(345, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(350, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(355, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->addEvent360(360, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
}

void configureNeon2003TriggerWaveformCam(TriggerWaveform *s) {
// todo: move sync point so that two channel does not have false trigger issues
	bool useOnlyPrimary = true;

	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->useRiseEdge = true;

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
	s->setTriggerSynchronizationGap3(1, 0.65, 1.25);

	float EPS_ANGLE = 0.3;

	s->useOnlyPrimaryForSync = true;

	if (useOnlyPrimary) {
		s->addEvent720(144, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
		s->addEvent720(180, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

		s->addEvent720(216, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
		s->addEvent720(252, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

		s->addEvent720(288, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
		s->addEvent720(324, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

		s->addEvent720(360, TriggerWheel::T_PRIMARY, TriggerValue::RISE); // width = 144
		s->addEvent720(504, TriggerWheel::T_PRIMARY, TriggerValue::FALL); // width = 36
		s->addEvent720(540, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
		s->addEvent720(576, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
		s->addEvent720(612, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
		s->addEvent720(648, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
		s->addEvent720(684, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
		s->addEvent720(720, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	} else {
		/**
		 * 7 primary (four narrow + one wide + two narrow)
		 * 64 secondary (one wide + 15 narrow + 16 narrow + one wide + 15 narrow + 16 narrow)
		 *
		 */

		s->addEvent720(25, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(30, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(35, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(40, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(45, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(50, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(55, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(60, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(65, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(70, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(75, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(80, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(85, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(90, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(95, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(100, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(105, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(110, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(115, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(120, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(125, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(130, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(135, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(140, TriggerWheel::T_SECONDARY, TriggerValue::RISE);

		s->addEvent720(144, TriggerWheel::T_PRIMARY, TriggerValue::RISE);

		s->addEvent720(145,TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(150, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(155, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(160, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(165, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(170, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(175, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(180 - EPS_ANGLE, TriggerWheel::T_SECONDARY, TriggerValue::RISE);

		s->addEvent720(180, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

		s->addEvent720(185, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(210, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(215, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

		s->addEvent720(216, TriggerWheel::T_PRIMARY, TriggerValue::RISE);

		s->addEvent720(220, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(225, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(230, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(235, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(240, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(245, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(250, TriggerWheel::T_SECONDARY, TriggerValue::RISE);

		s->addEvent720(252, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

		s->addEvent720(255, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

		s->addEvent720(260, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(265, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(270, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(275, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(280, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(285, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

		s->addEvent720(288, TriggerWheel::T_PRIMARY, TriggerValue::RISE);

		s->addEvent720(290, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(295, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(300, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(305, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(310, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(315, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(320, TriggerWheel::T_SECONDARY, TriggerValue::RISE);

		s->addEvent720(324, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

		s->addEvent720(325, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(330, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(335, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(340, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(345, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(350, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(355, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(360 - EPS_ANGLE, TriggerWheel::T_SECONDARY, TriggerValue::RISE);

		s->addEvent720(360, TriggerWheel::T_PRIMARY, TriggerValue::RISE);

		s->addEvent720(385, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(390, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(395, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(400, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(405, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(410, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(415, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(420, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(425, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(430, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(435, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(440, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(445, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(450, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(455, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(460, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(465, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(470, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(475, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(480, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(485, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(490, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(495, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(500, TriggerWheel::T_SECONDARY, TriggerValue::RISE);

		s->addEvent720(504, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

		s->addEvent720(505, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(510, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(515, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(520, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(525, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(530, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(535, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(540 - EPS_ANGLE, TriggerWheel::T_SECONDARY, TriggerValue::RISE);

		s->addEvent720(540, TriggerWheel::T_PRIMARY, TriggerValue::RISE);

		s->addEvent720(545, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(570, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(575, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

		s->addEvent720(576, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
		s->addEvent720(580, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(585, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(590, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(595, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(600, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(605, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(610, TriggerWheel::T_SECONDARY, TriggerValue::RISE);

		s->addEvent720(612, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
		s->addEvent720(615, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(620, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(625, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(630, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(635, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(640, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(645, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

		s->addEvent720(648, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
		s->addEvent720(650, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(655, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(660, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(665, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(670, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(675, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(680, TriggerWheel::T_SECONDARY, TriggerValue::RISE);

		s->addEvent720(684, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
		s->addEvent720(685, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(690, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(695, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(700, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(705, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(710, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(715, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
		s->addEvent720(720 - EPS_ANGLE, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(720, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	}
}

void configureDodgeStratusTriggerWaveform(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->tdcPosition = 150;

	float w = 7;
	float g = 20;
	s->setTriggerSynchronizationGap2(CHRYSLER_NGC6_GAP - 0.25,
			CHRYSLER_NGC6_GAP + 0.4);

	float base = 0;
	// 2 teeth
	float angle = base + 120.0 - w;
	s->addEvent720(angle, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(angle + w, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	angle += g;
	s->addEvent720(angle, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(angle + w, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	base += 120;
	// 3 teeth
	angle = base + 120.0 - w;
	s->addEvent720(angle, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(angle + w, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	angle += g;
	s->addEvent720(angle, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(angle + w, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	angle += g;
	s->addEvent720(angle, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(angle + w, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	base += 120;
	// 2 teeth
	angle = base + 120.0 - w;
	s->addEvent720(angle, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(angle + w, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	angle += g;
	s->addEvent720(angle, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(angle + w, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	base += 120;
	// just one
	angle = base + 120.0 - w;
	s->addEvent720(angle, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(angle + w, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	base += 120;
	// 3 teeth
	angle = base + 120.0 - w;
	s->addEvent720(angle, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(angle + w, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	angle += g;
	s->addEvent720(angle, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(angle + w, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	angle += g;
	s->addEvent720(angle, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(angle + w, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	base += 120;
	// just one again
	angle = base + 120.0 - w;
	s->addEvent720(angle, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(angle + w, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
}

static void configureNeon1995TriggerWaveformCommon(bool withCam, TriggerWaveform *s) {
	TriggerWheel crank = withCam ? TriggerWheel::T_SECONDARY : TriggerWheel::T_PRIMARY;

	// voodoo magic - we always need 720 at the end
	int base = withCam ? 720 - 560 : 360 - 135;

	float m = withCam ? 1 : 2;

	s->addEvent720(m * (base - 120), crank, TriggerValue::RISE);
	s->addEvent720(m * (base - 116), crank, TriggerValue::FALL);
	s->addEvent720(m * (base - 720 + 616), crank, TriggerValue::RISE);
	s->addEvent720(m * (base - 100), crank, TriggerValue::FALL);
	s->addEvent720(m * (base - 720 + 643), crank, TriggerValue::RISE);
	s->addEvent720(m * (base - 720 + 648), crank, TriggerValue::FALL);
	s->addEvent720(m * (base - 720 + 671), crank, TriggerValue::RISE);
	s->addEvent720(m * (base - 44), crank, TriggerValue::FALL);

	if (withCam) {
		s->addEvent720(base + 0, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	}
	s->addEvent720(m * (base + 20), crank, TriggerValue::RISE);
	s->addEvent720(m * (base + 60), crank, TriggerValue::FALL);
	s->addEvent720(m * (base + 75), crank, TriggerValue::RISE);
	s->addEvent720(m * (base + 79), crank, TriggerValue::FALL);
	s->addEvent720(m * (base + 101), crank, TriggerValue::RISE);
	s->addEvent720(m * (base + 106), crank, TriggerValue::FALL);
	s->addEvent720(m * (base + 130), crank, TriggerValue::RISE);
	s->addEvent720(m * (base + 135), crank, TriggerValue::FALL);

	if (withCam) {
		s->addEvent720(base + 200, TriggerWheel::T_PRIMARY, TriggerValue::RISE); // width = 150

		s->addEvent720(base + 236, crank, TriggerValue::RISE);
		s->addEvent720(base + 239, crank, TriggerValue::FALL);
		s->addEvent720(base + 250, crank, TriggerValue::RISE);
		s->addEvent720(base + 255, crank, TriggerValue::FALL);
		s->addEvent720(base + 277, crank, TriggerValue::RISE);
		s->addEvent720(base + 282, crank, TriggerValue::FALL);
		s->addEvent720(base + 305, crank, TriggerValue::RISE);
		s->addEvent720(base + 310, crank, TriggerValue::FALL);

	s->addEvent720(base + 374, crank, TriggerValue::RISE);

//	if (withCam)
		s->addEvent720(base + 395, TriggerWheel::T_PRIMARY, TriggerValue::FALL); // width =

	s->addEvent720(base + 418, crank, TriggerValue::FALL);
	s->addEvent720(base + 436, crank, TriggerValue::RISE);
	s->addEvent720(base + 441, crank, TriggerValue::FALL);
	s->addEvent720(base + 463, crank, TriggerValue::RISE);
	s->addEvent720(base + 468, crank, TriggerValue::FALL);
	s->addEvent720(base + 492, crank, TriggerValue::RISE);
	s->addEvent720(base + 497, crank, TriggerValue::FALL);

//	if (withCam)
		s->addEvent720(base + 560, TriggerWheel::T_PRIMARY, TriggerValue::RISE); // width =

	}
}

void configureNeon1995TriggerWaveformOnlyCrank(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR);
	s->setTriggerSynchronizationGap(3.79);

	s->tdcPosition = 279;

	configureNeon1995TriggerWaveformCommon(false, s);
}


void configureNeon1995TriggerWaveform(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	s->setTriggerSynchronizationGap(0.8227);

	s->useRiseEdge = false;


	s->initialState[(int)TriggerWheel::T_PRIMARY] = TriggerValue::RISE;

	configureNeon1995TriggerWaveformCommon(true, s);

	s->useOnlyPrimaryForSync = true;
}

void initJeep18_2_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->isSynchronizationNeeded = false;
	s->tdcPosition = 581;

	float off = 212;

	s->addEvent720(534 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(536 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(554 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(556 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(574 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(576 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(594 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(596 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(654 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(656 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(674 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(676 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(694 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(696 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(714 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(716 - 720 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);


	s->addEvent720(54 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(56 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(74 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(76 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(94 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(96 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(114 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(116 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(148 + off, TriggerWheel::T_PRIMARY, TriggerValue::RISE); // 360

	s->addEvent720(174 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(176 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(194 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(196 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(214 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(216 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(234 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(236 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(294 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(296 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(314 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(316 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(334 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(336 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(354 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(356 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(414 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(416 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(434 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(436 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(454 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(456 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
	s->addEvent720(474 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(476 + off, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(508 + off, TriggerWheel::T_PRIMARY, TriggerValue::FALL); // 720

}

static void add4cylblock(int off, TriggerWaveform *s) {
	s->addEvent720(114 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(114 + off + 2, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(134 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(134 + off + 2, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(154 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(154 + off + 2, TriggerWheel::T_SECONDARY, TriggerValue::FALL);

	s->addEvent720(174 + off, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
	s->addEvent720(174 + off + 2, TriggerWheel::T_SECONDARY, TriggerValue::FALL);
}

// TT_JEEP_4_CYL
void initJeep_XJ_4cyl_2500(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->isSynchronizationNeeded = false;
	s->tdcPosition = 720 - 236;

	float offset = 124;

	add4cylblock(offset - 180, s);
	add4cylblock(offset, s);

	s->addEvent720(236 + offset, TriggerWheel::T_PRIMARY, TriggerValue::RISE); // 360

	add4cylblock(offset + 180, s);
	add4cylblock(offset + 360, s);

	s->addEvent720(596 + offset, TriggerWheel::T_PRIMARY, TriggerValue::FALL); // 720

}

void configureChryslerNGC_36_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	float wide = 30 * 2;
	float narrow = 10 * 2;

	s->setTriggerSynchronizationGap(3.5);
	for (int i = 1; i < 15; i++) {
		s->setTriggerSynchronizationGap3(/*gapIndex*/i, TRIGGER_GAP_DEVIATION_LOW, TRIGGER_GAP_DEVIATION_HIGH);
	}
	s->setTriggerSynchronizationGap3(/*gapIndex*/15, 0.4 * TRIGGER_GAP_DEVIATION_LOW, 0.4 * TRIGGER_GAP_DEVIATION_HIGH);

	float base = 0;

	for (int i = 0; i < 14; i++) {
		s->addEventAngle(base + narrow / 2, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
		s->addEventAngle(base + narrow, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
		base += narrow;
	}

	s->addEventAngle(base + narrow / 2, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	base += narrow / 2;
	s->addEventAngle(base + wide, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	base += wide;

	for (int i = 0; i < 16; i++) {
		s->addEventAngle(base + narrow / 2, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
		s->addEventAngle(base + narrow, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
		base += narrow;
	}

	// one small tooth at the end of the engine cycle
	s->addEventAngle(s->getCycleDuration() - narrow / 2, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEventAngle(s->getCycleDuration(), TriggerWheel::T_PRIMARY, TriggerValue::FALL);
	s->useOnlyPrimaryForSync = true;
}
