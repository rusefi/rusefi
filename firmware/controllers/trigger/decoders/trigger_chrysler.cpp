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

	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Rise);

	s->isSynchronizationNeeded = false;

	addSkippedToothTriggerEvents(TriggerWheel::T_SECONDARY, s, 8, 0, 0.06, -25, 360, 0, 720);

	s->addEvent720(360, TriggerValue::RISE, TriggerWheel::T_PRIMARY);

	addSkippedToothTriggerEvents(TriggerWheel::T_SECONDARY, s, 8, 0, 0.06, 360 - 25, 360, 0, 720);

	s->addEvent720(720, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
}

void configureNeon2003TriggerWaveformCrank(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

	s->setTriggerSynchronizationGap(3);

	s->addEvent360(25, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(30, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(35, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(40, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(45, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(50, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(55, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(60, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(65, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(70, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(75, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(80, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(85, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(90, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(95, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(100, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(105, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(110, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(115, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(120, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(125, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(130, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(135, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(140, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(145,TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(150, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(155, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(160, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(165, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(170, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(175, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(180, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(185, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	// gap 25
	s->addEvent360(210, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(215, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(220, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(225, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(230, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(235, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(240, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(245, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(250, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(255, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(260, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(265, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(270, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(275, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(280, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(285, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(290, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(295, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(300, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(305, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(310, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(315, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(320, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(325, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(330, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(335, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(340, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(345, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(350, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent360(355, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(360, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
}

void configureNeon2003TriggerWaveformCam(TriggerWaveform *s) {
// todo: move sync point so that two channel does not have false trigger issues
	bool useOnlyPrimary = true;

	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Rise);

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


	if (useOnlyPrimary) {
		s->addEvent720(144, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
		s->addEvent720(180, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

		s->addEvent720(216, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
		s->addEvent720(252, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

		s->addEvent720(288, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
		s->addEvent720(324, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

		s->addEvent720(360, TriggerValue::RISE, TriggerWheel::T_PRIMARY); // width = 144
		s->addEvent720(504, TriggerValue::FALL, TriggerWheel::T_PRIMARY); // width = 36
		s->addEvent720(540, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
		s->addEvent720(576, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
		s->addEvent720(612, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
		s->addEvent720(648, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
		s->addEvent720(684, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
		s->addEvent720(720, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	} else {
		/**
		 * 7 primary (four narrow + one wide + two narrow)
		 * 64 secondary (one wide + 15 narrow + 16 narrow + one wide + 15 narrow + 16 narrow)
		 *
		 */

		s->useOnlyPrimaryForSync = true;

		s->addEvent720(25, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(30, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(35, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(40, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(45, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(50, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(55, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(60, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(65, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(70, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(75, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(80, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(85, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(90, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(95, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(100, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(105, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(110, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(115, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(120, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(125, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(130, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(135, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(140, TriggerValue::RISE, TriggerWheel::T_SECONDARY);

		s->addEvent720(144, TriggerValue::RISE, TriggerWheel::T_PRIMARY);

		s->addEvent720(145,TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(150, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(155, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(160, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(165, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(170, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(175, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(180 - EPS_ANGLE, TriggerValue::RISE, TriggerWheel::T_SECONDARY);

		s->addEvent720(180, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

		s->addEvent720(185, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(210, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(215, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

		s->addEvent720(216, TriggerValue::RISE, TriggerWheel::T_PRIMARY);

		s->addEvent720(220, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(225, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(230, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(235, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(240, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(245, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(250, TriggerValue::RISE, TriggerWheel::T_SECONDARY);

		s->addEvent720(252, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

		s->addEvent720(255, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

		s->addEvent720(260, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(265, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(270, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(275, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(280, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(285, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

		s->addEvent720(288, TriggerValue::RISE, TriggerWheel::T_PRIMARY);

		s->addEvent720(290, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(295, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(300, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(305, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(310, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(315, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(320, TriggerValue::RISE, TriggerWheel::T_SECONDARY);

		s->addEvent720(324, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

		s->addEvent720(325, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(330, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(335, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(340, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(345, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(350, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(355, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(360 - EPS_ANGLE, TriggerValue::RISE, TriggerWheel::T_SECONDARY);

		s->addEvent720(360, TriggerValue::RISE, TriggerWheel::T_PRIMARY);

		s->addEvent720(385, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(390, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(395, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(400, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(405, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(410, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(415, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(420, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(425, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(430, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(435, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(440, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(445, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(450, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(455, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(460, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(465, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(470, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(475, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(480, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(485, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(490, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(495, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(500, TriggerValue::RISE, TriggerWheel::T_SECONDARY);

		s->addEvent720(504, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

		s->addEvent720(505, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(510, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(515, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(520, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(525, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(530, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(535, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(540 - EPS_ANGLE, TriggerValue::RISE, TriggerWheel::T_SECONDARY);

		s->addEvent720(540, TriggerValue::RISE, TriggerWheel::T_PRIMARY);

		s->addEvent720(545, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(570, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(575, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

		s->addEvent720(576, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
		s->addEvent720(580, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(585, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(590, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(595, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(600, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(605, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(610, TriggerValue::RISE, TriggerWheel::T_SECONDARY);

		s->addEvent720(612, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
		s->addEvent720(615, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(620, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(625, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(630, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(635, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(640, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(645, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

		s->addEvent720(648, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
		s->addEvent720(650, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(655, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(660, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(665, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(670, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(675, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(680, TriggerValue::RISE, TriggerWheel::T_SECONDARY);

		s->addEvent720(684, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
		s->addEvent720(685, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(690, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(695, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(700, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(705, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(710, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(715, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
		s->addEvent720(720 - EPS_ANGLE, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(720, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	}
}

void configureDodgeStratusTriggerWaveform(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	s->tdcPosition = 150;

	float w = 7;
	float g = 20;
	s->setTriggerSynchronizationGap2(CHRYSLER_NGC6_GAP - 0.25,
			CHRYSLER_NGC6_GAP + 0.4);

	float base = 0;
	// 2 teeth
	float angle = base + 120.0 - w;
	s->addEvent720(angle, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	angle += g;
	s->addEvent720(angle, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	base += 120;
	// 3 teeth
	angle = base + 120.0 - w;
	s->addEvent720(angle, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	angle += g;
	s->addEvent720(angle, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	angle += g;
	s->addEvent720(angle, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	base += 120;
	// 2 teeth
	angle = base + 120.0 - w;
	s->addEvent720(angle, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	angle += g;
	s->addEvent720(angle, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	base += 120;
	// just one
	angle = base + 120.0 - w;
	s->addEvent720(angle, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	base += 120;
	// 3 teeth
	angle = base + 120.0 - w;
	s->addEvent720(angle, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	angle += g;
	s->addEvent720(angle, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	angle += g;
	s->addEvent720(angle, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	base += 120;
	// just one again
	angle = base + 120.0 - w;
	s->addEvent720(angle, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
}

static void configureNeon1995TriggerWaveformCommon(bool withCam, TriggerWaveform *s) {
	s->useOnlyPrimaryForSync = withCam;
	
	TriggerWheel crank = withCam ? TriggerWheel::T_SECONDARY : TriggerWheel::T_PRIMARY;

	// voodoo magic - we always need 720 at the end
	int base = withCam ? 720 - 560 : 360 - 135;

	float m = withCam ? 1 : 2;

	s->addEvent720(m * (base - 120), TriggerValue::RISE, crank);
	s->addEvent720(m * (base - 116), TriggerValue::FALL, crank);
	s->addEvent720(m * (base - 720 + 616), TriggerValue::RISE, crank);
	s->addEvent720(m * (base - 100), TriggerValue::FALL, crank);
	s->addEvent720(m * (base - 720 + 643), TriggerValue::RISE, crank);
	s->addEvent720(m * (base - 720 + 648), TriggerValue::FALL, crank);
	s->addEvent720(m * (base - 720 + 671), TriggerValue::RISE, crank);
	s->addEvent720(m * (base - 44), TriggerValue::FALL, crank);

	if (withCam) {
		s->addEvent720(base + 0, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	}
	s->addEvent720(m * (base + 20), TriggerValue::RISE, crank);
	s->addEvent720(m * (base + 60), TriggerValue::FALL, crank);
	s->addEvent720(m * (base + 75), TriggerValue::RISE, crank);
	s->addEvent720(m * (base + 79), TriggerValue::FALL, crank);
	s->addEvent720(m * (base + 101), TriggerValue::RISE, crank);
	s->addEvent720(m * (base + 106), TriggerValue::FALL, crank);
	s->addEvent720(m * (base + 130), TriggerValue::RISE, crank);
	s->addEvent720(m * (base + 135), TriggerValue::FALL, crank);

	if (withCam) {
		s->addEvent720(base + 200, TriggerValue::RISE, TriggerWheel::T_PRIMARY); // width = 150

		s->addEvent720(base + 236, TriggerValue::RISE, crank);
		s->addEvent720(base + 239, TriggerValue::FALL, crank);
		s->addEvent720(base + 250, TriggerValue::RISE, crank);
		s->addEvent720(base + 255, TriggerValue::FALL, crank);
		s->addEvent720(base + 277, TriggerValue::RISE, crank);
		s->addEvent720(base + 282, TriggerValue::FALL, crank);
		s->addEvent720(base + 305, TriggerValue::RISE, crank);
		s->addEvent720(base + 310, TriggerValue::FALL, crank);

	s->addEvent720(base + 374, TriggerValue::RISE, crank);

//	if (withCam)
		s->addEvent720(base + 395, TriggerValue::FALL, TriggerWheel::T_PRIMARY); // width =

	s->addEvent720(base + 418, TriggerValue::FALL, crank);
	s->addEvent720(base + 436, TriggerValue::RISE, crank);
	s->addEvent720(base + 441, TriggerValue::FALL, crank);
	s->addEvent720(base + 463, TriggerValue::RISE, crank);
	s->addEvent720(base + 468, TriggerValue::FALL, crank);
	s->addEvent720(base + 492, TriggerValue::RISE, crank);
	s->addEvent720(base + 497, TriggerValue::FALL, crank);

//	if (withCam)
		s->addEvent720(base + 560, TriggerValue::RISE, TriggerWheel::T_PRIMARY); // width =

	}
}

void configureNeon1995TriggerWaveformOnlyCrank(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::Rise);
	s->setTriggerSynchronizationGap(3.79);

	s->tdcPosition = 279;

	configureNeon1995TriggerWaveformCommon(false, s);
}


void configureNeon1995TriggerWaveform(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Fall);

	s->setTriggerSynchronizationGap(0.8227);

	s->initialState[(int)TriggerWheel::T_PRIMARY] = TriggerValue::RISE;

	configureNeon1995TriggerWaveformCommon(true, s);
}

void initJeep18_2_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	s->isSynchronizationNeeded = false;
	s->tdcPosition = 581;

	float off = 212;

	s->addEvent720(534 - 720 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(536 - 720 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(554 - 720 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(556 - 720 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(574 - 720 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(576 - 720 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(594 - 720 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(596 - 720 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

	s->addEvent720(654 - 720 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(656 - 720 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(674 - 720 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(676 - 720 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(694 - 720 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(696 - 720 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(714 - 720 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(716 - 720 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);


	s->addEvent720(54 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(56 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(74 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(76 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(94 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(96 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(114 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(116 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

	s->addEvent720(148 + off, TriggerValue::RISE, TriggerWheel::T_PRIMARY); // 360

	s->addEvent720(174 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(176 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(194 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(196 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(214 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(216 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(234 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(236 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

	s->addEvent720(294 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(296 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(314 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(316 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(334 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(336 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(354 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(356 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

	s->addEvent720(414 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(416 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(434 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(436 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(454 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(456 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(474 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(476 + off, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

	s->addEvent720(508 + off, TriggerValue::FALL, TriggerWheel::T_PRIMARY); // 720

}

static void add4cylblock(int off, TriggerWaveform *s) {
	s->addEvent720(114 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(114 + off + 2, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

	s->addEvent720(134 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(134 + off + 2, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

	s->addEvent720(154 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(154 + off + 2, TriggerValue::FALL, TriggerWheel::T_SECONDARY);

	s->addEvent720(174 + off, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(174 + off + 2, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
}

// TT_JEEP_4_CYL
void initJeep_XJ_4cyl_2500(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	s->isSynchronizationNeeded = false;
	s->tdcPosition = 720 - 236;

	float offset = 124;

	add4cylblock(offset - 180, s);
	add4cylblock(offset, s);

	s->addEvent720(236 + offset, TriggerValue::RISE, TriggerWheel::T_PRIMARY); // 360

	add4cylblock(offset + 180, s);
	add4cylblock(offset + 360, s);

	s->addEvent720(596 + offset, TriggerValue::FALL, TriggerWheel::T_PRIMARY); // 720

}

void configureChryslerNGC_36_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	float wide = 30 * 2;
	float narrow = 10 * 2;

	s->setTriggerSynchronizationGap(3.5);
	for (int i = 1; i < 15; i++) {
		s->setTriggerSynchronizationGap3(/*gapIndex*/i, TRIGGER_GAP_DEVIATION_LOW, TRIGGER_GAP_DEVIATION_HIGH);
	}
	s->setTriggerSynchronizationGap3(/*gapIndex*/15, 0.4 * TRIGGER_GAP_DEVIATION_LOW, 0.4 * TRIGGER_GAP_DEVIATION_HIGH);

	float base = 0;

	for (int i = 0; i < 14; i++) {
		s->addEventAngle(base + narrow / 2, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
		s->addEventAngle(base + narrow, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
		base += narrow;
	}

	s->addEventAngle(base + narrow / 2, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	base += narrow / 2;
	s->addEventAngle(base + wide, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	base += wide;

	for (int i = 0; i < 16; i++) {
		s->addEventAngle(base + narrow / 2, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
		s->addEventAngle(base + narrow, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
		base += narrow;
	}

	// one small tooth at the end of the engine cycle
	s->addEventAngle(s->getCycleDuration() - narrow / 2, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEventAngle(s->getCycleDuration(), TriggerValue::FALL, TriggerWheel::T_PRIMARY);
}
