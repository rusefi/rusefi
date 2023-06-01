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

	s->addEvent720(360, true, TriggerWheel::T_PRIMARY);

	addSkippedToothTriggerEvents(TriggerWheel::T_SECONDARY, s, 8, 0, 0.06, 360 - 25, 360, 0, 720);

	s->addEvent720(720, false, TriggerWheel::T_PRIMARY);
}

void configureNeon2003TriggerWaveformCrank(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

	s->setTriggerSynchronizationGap(3);

	s->addEvent360(25, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(30, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(35, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(40, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(45, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(50, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(55, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(60, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(65, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(70, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(75, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(80, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(85, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(90, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(95, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(100, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(105, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(110, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(115, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(120, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(125, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(130, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(135, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(140, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(145,false, TriggerWheel::T_PRIMARY);
	s->addEvent360(150, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(155, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(160, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(165, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(170, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(175, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(180, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(185, false, TriggerWheel::T_PRIMARY);
	// gap 25
	s->addEvent360(210, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(215, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(220, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(225, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(230, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(235, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(240, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(245, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(250, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(255, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(260, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(265, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(270, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(275, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(280, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(285, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(290, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(295, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(300, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(305, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(310, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(315, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(320, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(325, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(330, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(335, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(340, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(345, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(350, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(355, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(360, true, TriggerWheel::T_PRIMARY);
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
		s->addEvent720(144, true, TriggerWheel::T_PRIMARY);
		s->addEvent720(180, false, TriggerWheel::T_PRIMARY);

		s->addEvent720(216, true, TriggerWheel::T_PRIMARY);
		s->addEvent720(252, false, TriggerWheel::T_PRIMARY);

		s->addEvent720(288, true, TriggerWheel::T_PRIMARY);
		s->addEvent720(324, false, TriggerWheel::T_PRIMARY);

		s->addEvent720(360, true, TriggerWheel::T_PRIMARY); // width = 144
		s->addEvent720(504, false, TriggerWheel::T_PRIMARY); // width = 36
		s->addEvent720(540, true, TriggerWheel::T_PRIMARY);
		s->addEvent720(576, false, TriggerWheel::T_PRIMARY);
		s->addEvent720(612, true, TriggerWheel::T_PRIMARY);
		s->addEvent720(648, false, TriggerWheel::T_PRIMARY);
		s->addEvent720(684, true, TriggerWheel::T_PRIMARY);
		s->addEvent720(720, false, TriggerWheel::T_PRIMARY);

	} else {
		/**
		 * 7 primary (four narrow + one wide + two narrow)
		 * 64 secondary (one wide + 15 narrow + 16 narrow + one wide + 15 narrow + 16 narrow)
		 *
		 */

		s->useOnlyPrimaryForSync = true;

		s->addEvent720(25, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(30, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(35, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(40, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(45, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(50, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(55, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(60, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(65, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(70, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(75, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(80, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(85, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(90, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(95, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(100, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(105, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(110, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(115, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(120, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(125, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(130, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(135, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(140, true, TriggerWheel::T_SECONDARY);

		s->addEvent720(144, true, TriggerWheel::T_PRIMARY);

		s->addEvent720(145,false, TriggerWheel::T_SECONDARY);
		s->addEvent720(150, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(155, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(160, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(165, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(170, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(175, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(180 - EPS_ANGLE, true, TriggerWheel::T_SECONDARY);

		s->addEvent720(180, false, TriggerWheel::T_PRIMARY);

		s->addEvent720(185, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(210, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(215, false, TriggerWheel::T_SECONDARY);

		s->addEvent720(216, true, TriggerWheel::T_PRIMARY);

		s->addEvent720(220, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(225, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(230, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(235, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(240, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(245, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(250, true, TriggerWheel::T_SECONDARY);

		s->addEvent720(252, false, TriggerWheel::T_PRIMARY);

		s->addEvent720(255, false, TriggerWheel::T_SECONDARY);

		s->addEvent720(260, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(265, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(270, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(275, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(280, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(285, false, TriggerWheel::T_SECONDARY);

		s->addEvent720(288, true, TriggerWheel::T_PRIMARY);

		s->addEvent720(290, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(295, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(300, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(305, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(310, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(315, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(320, true, TriggerWheel::T_SECONDARY);

		s->addEvent720(324, false, TriggerWheel::T_PRIMARY);

		s->addEvent720(325, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(330, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(335, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(340, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(345, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(350, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(355, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(360 - EPS_ANGLE, true, TriggerWheel::T_SECONDARY);

		s->addEvent720(360, true, TriggerWheel::T_PRIMARY);

		s->addEvent720(385, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(390, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(395, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(400, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(405, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(410, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(415, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(420, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(425, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(430, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(435, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(440, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(445, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(450, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(455, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(460, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(465, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(470, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(475, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(480, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(485, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(490, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(495, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(500, true, TriggerWheel::T_SECONDARY);

		s->addEvent720(504, false, TriggerWheel::T_PRIMARY);

		s->addEvent720(505, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(510, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(515, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(520, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(525, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(530, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(535, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(540 - EPS_ANGLE, true, TriggerWheel::T_SECONDARY);

		s->addEvent720(540, true, TriggerWheel::T_PRIMARY);

		s->addEvent720(545, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(570, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(575, false, TriggerWheel::T_SECONDARY);

		s->addEvent720(576, false, TriggerWheel::T_PRIMARY);
		s->addEvent720(580, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(585, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(590, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(595, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(600, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(605, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(610, true, TriggerWheel::T_SECONDARY);

		s->addEvent720(612, true, TriggerWheel::T_PRIMARY);
		s->addEvent720(615, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(620, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(625, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(630, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(635, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(640, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(645, false, TriggerWheel::T_SECONDARY);

		s->addEvent720(648, false, TriggerWheel::T_PRIMARY);
		s->addEvent720(650, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(655, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(660, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(665, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(670, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(675, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(680, true, TriggerWheel::T_SECONDARY);

		s->addEvent720(684, true, TriggerWheel::T_PRIMARY);
		s->addEvent720(685, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(690, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(695, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(700, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(705, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(710, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(715, false, TriggerWheel::T_SECONDARY);
		s->addEvent720(720 - EPS_ANGLE, true, TriggerWheel::T_SECONDARY);
		s->addEvent720(720, false, TriggerWheel::T_PRIMARY);
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
	s->addEvent720(angle, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, false, TriggerWheel::T_PRIMARY);
	angle += g;
	s->addEvent720(angle, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, false, TriggerWheel::T_PRIMARY);

	base += 120;
	// 3 teeth
	angle = base + 120.0 - w;
	s->addEvent720(angle, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, false, TriggerWheel::T_PRIMARY);
	angle += g;
	s->addEvent720(angle, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, false, TriggerWheel::T_PRIMARY);
	angle += g;
	s->addEvent720(angle, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, false, TriggerWheel::T_PRIMARY);

	base += 120;
	// 2 teeth
	angle = base + 120.0 - w;
	s->addEvent720(angle, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, false, TriggerWheel::T_PRIMARY);
	angle += g;
	s->addEvent720(angle, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, false, TriggerWheel::T_PRIMARY);

	base += 120;
	// just one
	angle = base + 120.0 - w;
	s->addEvent720(angle, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, false, TriggerWheel::T_PRIMARY);

	base += 120;
	// 3 teeth
	angle = base + 120.0 - w;
	s->addEvent720(angle, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, false, TriggerWheel::T_PRIMARY);
	angle += g;
	s->addEvent720(angle, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, false, TriggerWheel::T_PRIMARY);
	angle += g;
	s->addEvent720(angle, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, false, TriggerWheel::T_PRIMARY);

	base += 120;
	// just one again
	angle = base + 120.0 - w;
	s->addEvent720(angle, true, TriggerWheel::T_PRIMARY);
	s->addEvent720(angle + w, false, TriggerWheel::T_PRIMARY);
}

void configureNeon1995TriggerWaveformOnlyCrank(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::Rise);

	// Nominal gap 3.25
	s->setTriggerSynchronizationGap3(0, 1.6, 4.5);

	// Nominal gap 1.0
	s->setTriggerSynchronizationGap3(1, 0.5, 1.5);
	s->setTriggerSynchronizationGap3(2, 0.5, 1.5);

	// Nominal gap 0.168
	s->setTriggerSynchronizationGap3(3, 0.1, 0.3);

	s->tdcPosition = 279;

	// voodoo magic - we always need 720 at the end
	int base = 59;

	s->addEvent360(base + 0, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 60, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 75, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 82, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 96, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 102, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 116, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 122, false, TriggerWheel::T_PRIMARY);

	s->addEvent360(base + 235, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 242, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 255, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 261, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 275, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 281, false, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 295, true, TriggerWheel::T_PRIMARY);
	s->addEvent360(base + 301, false, TriggerWheel::T_PRIMARY);
}

void initJeep18_2_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	s->isSynchronizationNeeded = false;
	s->tdcPosition = 581;

	float off = 212;

	s->addEvent720(534 - 720 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(536 - 720 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(554 - 720 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(556 - 720 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(574 - 720 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(576 - 720 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(594 - 720 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(596 - 720 + off, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(654 - 720 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(656 - 720 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(674 - 720 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(676 - 720 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(694 - 720 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(696 - 720 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(714 - 720 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(716 - 720 + off, false, TriggerWheel::T_SECONDARY);


	s->addEvent720(54 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(56 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(74 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(76 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(94 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(96 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(114 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(116 + off, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(148 + off, true, TriggerWheel::T_PRIMARY); // 360

	s->addEvent720(174 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(176 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(194 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(196 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(214 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(216 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(234 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(236 + off, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(294 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(296 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(314 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(316 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(334 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(336 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(354 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(356 + off, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(414 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(416 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(434 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(436 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(454 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(456 + off, false, TriggerWheel::T_SECONDARY);
	s->addEvent720(474 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(476 + off, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(508 + off, false, TriggerWheel::T_PRIMARY); // 720

}

static void add4cylblock(int off, TriggerWaveform *s) {
	s->addEvent720(114 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(114 + off + 2, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(134 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(134 + off + 2, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(154 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(154 + off + 2, false, TriggerWheel::T_SECONDARY);

	s->addEvent720(174 + off, true, TriggerWheel::T_SECONDARY);
	s->addEvent720(174 + off + 2, false, TriggerWheel::T_SECONDARY);
}

// trigger_type_e::TT_JEEP_4_CYL
void initJeep_XJ_4cyl_2500(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	s->isSynchronizationNeeded = false;
	s->tdcPosition = 720 - 236;

	float offset = 124;

	add4cylblock(offset - 180, s);
	add4cylblock(offset, s);

	s->addEvent720(236 + offset, true, TriggerWheel::T_PRIMARY); // 360

	add4cylblock(offset + 180, s);
	add4cylblock(offset + 360, s);

	s->addEvent720(596 + offset, false, TriggerWheel::T_PRIMARY); // 720

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
		s->addEventAngle(base + narrow / 2, true, TriggerWheel::T_PRIMARY);
		s->addEventAngle(base + narrow, false, TriggerWheel::T_PRIMARY);
		base += narrow;
	}

	s->addEventAngle(base + narrow / 2, true, TriggerWheel::T_PRIMARY);
	base += narrow / 2;
	s->addEventAngle(base + wide, false, TriggerWheel::T_PRIMARY);
	base += wide;

	for (int i = 0; i < 16; i++) {
		s->addEventAngle(base + narrow / 2, true, TriggerWheel::T_PRIMARY);
		s->addEventAngle(base + narrow, false, TriggerWheel::T_PRIMARY);
		base += narrow;
	}

	// one small tooth at the end of the engine cycle
	s->addEventAngle(s->getCycleDuration() - narrow / 2, true, TriggerWheel::T_PRIMARY);
	s->addEventAngle(s->getCycleDuration(), false, TriggerWheel::T_PRIMARY);
}
