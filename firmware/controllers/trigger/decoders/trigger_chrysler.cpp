/**
 * @file	trigger_chrysler.cpp
 *
 * @date Mar 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "trigger_chrysler.h"
#include "trigger_universal.h"

void initDodgeRam(TriggerWaveform *s) {

	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->useRiseEdge = true;

	s->isSynchronizationNeeded = false;

	addSkippedToothTriggerEvents(T_SECONDARY, s, 8, 0, 0.06, -25, 360, 0, 720);

	s->addEvent720(360, T_PRIMARY, TV_RISE);

	addSkippedToothTriggerEvents(T_SECONDARY, s, 8, 0, 0.06, 360 - 25, 360, 0, 720);

	s->addEvent720(720, T_PRIMARY, TV_FALL);
}

void configureNeon2003TriggerWaveformCrank(TriggerWaveform *s DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR);
	s->useRiseEdge = true;

	s->setTriggerSynchronizationGap(3);

	s->addEvent360(25, T_PRIMARY, TV_FALL);
	s->addEvent360(30, T_PRIMARY, TV_RISE);
	s->addEvent360(35, T_PRIMARY, TV_FALL);
	s->addEvent360(40, T_PRIMARY, TV_RISE);
	s->addEvent360(45, T_PRIMARY, TV_FALL);
	s->addEvent360(50, T_PRIMARY, TV_RISE);
	s->addEvent360(55, T_PRIMARY, TV_FALL);
	s->addEvent360(60, T_PRIMARY, TV_RISE);
	s->addEvent360(65, T_PRIMARY, TV_FALL);
	s->addEvent360(70, T_PRIMARY, TV_RISE);
	s->addEvent360(75, T_PRIMARY, TV_FALL);
	s->addEvent360(80, T_PRIMARY, TV_RISE);
	s->addEvent360(85, T_PRIMARY, TV_FALL);
	s->addEvent360(90, T_PRIMARY, TV_RISE);
	s->addEvent360(95, T_PRIMARY, TV_FALL);
	s->addEvent360(100, T_PRIMARY, TV_RISE);
	s->addEvent360(105, T_PRIMARY, TV_FALL);
	s->addEvent360(110, T_PRIMARY, TV_RISE);
	s->addEvent360(115, T_PRIMARY, TV_FALL);
	s->addEvent360(120, T_PRIMARY, TV_RISE);
	s->addEvent360(125, T_PRIMARY, TV_FALL);
	s->addEvent360(130, T_PRIMARY, TV_RISE);
	s->addEvent360(135, T_PRIMARY, TV_FALL);
	s->addEvent360(140, T_PRIMARY, TV_RISE);
	s->addEvent360(145,T_PRIMARY, TV_FALL);
	s->addEvent360(150, T_PRIMARY, TV_RISE);
	s->addEvent360(155, T_PRIMARY, TV_FALL);
	s->addEvent360(160, T_PRIMARY, TV_RISE);
	s->addEvent360(165, T_PRIMARY, TV_FALL);
	s->addEvent360(170, T_PRIMARY, TV_RISE);
	s->addEvent360(175, T_PRIMARY, TV_FALL);
	s->addEvent360(180, T_PRIMARY, TV_RISE);
	s->addEvent360(185, T_PRIMARY, TV_FALL);
	// gap 25
	s->addEvent360(210, T_PRIMARY, TV_RISE);
	s->addEvent360(215, T_PRIMARY, TV_FALL);
	s->addEvent360(220, T_PRIMARY, TV_RISE);
	s->addEvent360(225, T_PRIMARY, TV_FALL);
	s->addEvent360(230, T_PRIMARY, TV_RISE);
	s->addEvent360(235, T_PRIMARY, TV_FALL);
	s->addEvent360(240, T_PRIMARY, TV_RISE);
	s->addEvent360(245, T_PRIMARY, TV_FALL);
	s->addEvent360(250, T_PRIMARY, TV_RISE);
	s->addEvent360(255, T_PRIMARY, TV_FALL);
	s->addEvent360(260, T_PRIMARY, TV_RISE);
	s->addEvent360(265, T_PRIMARY, TV_FALL);
	s->addEvent360(270, T_PRIMARY, TV_RISE);
	s->addEvent360(275, T_PRIMARY, TV_FALL);
	s->addEvent360(280, T_PRIMARY, TV_RISE);
	s->addEvent360(285, T_PRIMARY, TV_FALL);
	s->addEvent360(290, T_PRIMARY, TV_RISE);
	s->addEvent360(295, T_PRIMARY, TV_FALL);
	s->addEvent360(300, T_PRIMARY, TV_RISE);
	s->addEvent360(305, T_PRIMARY, TV_FALL);
	s->addEvent360(310, T_PRIMARY, TV_RISE);
	s->addEvent360(315, T_PRIMARY, TV_FALL);
	s->addEvent360(320, T_PRIMARY, TV_RISE);
	s->addEvent360(325, T_PRIMARY, TV_FALL);
	s->addEvent360(330, T_PRIMARY, TV_RISE);
	s->addEvent360(335, T_PRIMARY, TV_FALL);
	s->addEvent360(340, T_PRIMARY, TV_RISE);
	s->addEvent360(345, T_PRIMARY, TV_FALL);
	s->addEvent360(350, T_PRIMARY, TV_RISE);
	s->addEvent360(355, T_PRIMARY, TV_FALL);
	s->addEvent360(360, T_PRIMARY, TV_RISE);
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
		s->addEvent720(144, T_PRIMARY, TV_RISE);
		s->addEvent720(180, T_PRIMARY, TV_FALL);

		s->addEvent720(216, T_PRIMARY, TV_RISE);
		s->addEvent720(252, T_PRIMARY, TV_FALL);

		s->addEvent720(288, T_PRIMARY, TV_RISE);
		s->addEvent720(324, T_PRIMARY, TV_FALL);

		s->addEvent720(360, T_PRIMARY, TV_RISE); // width = 144
		s->addEvent720(504, T_PRIMARY, TV_FALL); // width = 36
		s->addEvent720(540, T_PRIMARY, TV_RISE);
		s->addEvent720(576, T_PRIMARY, TV_FALL);
		s->addEvent720(612, T_PRIMARY, TV_RISE);
		s->addEvent720(648, T_PRIMARY, TV_FALL);
		s->addEvent720(684, T_PRIMARY, TV_RISE);
		s->addEvent720(720, T_PRIMARY, TV_FALL);

	} else {
		/**
		 * 7 primary (four narrow + one wide + two narrow)
		 * 64 secondary (one wide + 15 narrow + 16 narrow + one wide + 15 narrow + 16 narrow)
		 *
		 */

		s->addEvent720(25, T_SECONDARY, TV_FALL);
		s->addEvent720(30, T_SECONDARY, TV_RISE);
		s->addEvent720(35, T_SECONDARY, TV_FALL);
		s->addEvent720(40, T_SECONDARY, TV_RISE);
		s->addEvent720(45, T_SECONDARY, TV_FALL);
		s->addEvent720(50, T_SECONDARY, TV_RISE);
		s->addEvent720(55, T_SECONDARY, TV_FALL);
		s->addEvent720(60, T_SECONDARY, TV_RISE);
		s->addEvent720(65, T_SECONDARY, TV_FALL);
		s->addEvent720(70, T_SECONDARY, TV_RISE);
		s->addEvent720(75, T_SECONDARY, TV_FALL);
		s->addEvent720(80, T_SECONDARY, TV_RISE);
		s->addEvent720(85, T_SECONDARY, TV_FALL);
		s->addEvent720(90, T_SECONDARY, TV_RISE);
		s->addEvent720(95, T_SECONDARY, TV_FALL);
		s->addEvent720(100, T_SECONDARY, TV_RISE);
		s->addEvent720(105, T_SECONDARY, TV_FALL);
		s->addEvent720(110, T_SECONDARY, TV_RISE);
		s->addEvent720(115, T_SECONDARY, TV_FALL);
		s->addEvent720(120, T_SECONDARY, TV_RISE);
		s->addEvent720(125, T_SECONDARY, TV_FALL);
		s->addEvent720(130, T_SECONDARY, TV_RISE);
		s->addEvent720(135, T_SECONDARY, TV_FALL);
		s->addEvent720(140, T_SECONDARY, TV_RISE);

		s->addEvent720(144, T_PRIMARY, TV_RISE);

		s->addEvent720(145,T_SECONDARY, TV_FALL);
		s->addEvent720(150, T_SECONDARY, TV_RISE);
		s->addEvent720(155, T_SECONDARY, TV_FALL);
		s->addEvent720(160, T_SECONDARY, TV_RISE);
		s->addEvent720(165, T_SECONDARY, TV_FALL);
		s->addEvent720(170, T_SECONDARY, TV_RISE);
		s->addEvent720(175, T_SECONDARY, TV_FALL);
		s->addEvent720(180 - EPS_ANGLE, T_SECONDARY, TV_RISE);

		s->addEvent720(180, T_PRIMARY, TV_FALL);

		s->addEvent720(185, T_SECONDARY, TV_FALL);
		s->addEvent720(210, T_SECONDARY, TV_RISE);
		s->addEvent720(215, T_SECONDARY, TV_FALL);

		s->addEvent720(216, T_PRIMARY, TV_RISE);

		s->addEvent720(220, T_SECONDARY, TV_RISE);
		s->addEvent720(225, T_SECONDARY, TV_FALL);
		s->addEvent720(230, T_SECONDARY, TV_RISE);
		s->addEvent720(235, T_SECONDARY, TV_FALL);
		s->addEvent720(240, T_SECONDARY, TV_RISE);
		s->addEvent720(245, T_SECONDARY, TV_FALL);
		s->addEvent720(250, T_SECONDARY, TV_RISE);

		s->addEvent720(252, T_PRIMARY, TV_FALL);

		s->addEvent720(255, T_SECONDARY, TV_FALL);

		s->addEvent720(260, T_SECONDARY, TV_RISE);
		s->addEvent720(265, T_SECONDARY, TV_FALL);
		s->addEvent720(270, T_SECONDARY, TV_RISE);
		s->addEvent720(275, T_SECONDARY, TV_FALL);
		s->addEvent720(280, T_SECONDARY, TV_RISE);
		s->addEvent720(285, T_SECONDARY, TV_FALL);

		s->addEvent720(288, T_PRIMARY, TV_RISE);

		s->addEvent720(290, T_SECONDARY, TV_RISE);
		s->addEvent720(295, T_SECONDARY, TV_FALL);
		s->addEvent720(300, T_SECONDARY, TV_RISE);
		s->addEvent720(305, T_SECONDARY, TV_FALL);
		s->addEvent720(310, T_SECONDARY, TV_RISE);
		s->addEvent720(315, T_SECONDARY, TV_FALL);
		s->addEvent720(320, T_SECONDARY, TV_RISE);

		s->addEvent720(324, T_PRIMARY, TV_FALL);

		s->addEvent720(325, T_SECONDARY, TV_FALL);
		s->addEvent720(330, T_SECONDARY, TV_RISE);
		s->addEvent720(335, T_SECONDARY, TV_FALL);
		s->addEvent720(340, T_SECONDARY, TV_RISE);
		s->addEvent720(345, T_SECONDARY, TV_FALL);
		s->addEvent720(350, T_SECONDARY, TV_RISE);
		s->addEvent720(355, T_SECONDARY, TV_FALL);
		s->addEvent720(360 - EPS_ANGLE, T_SECONDARY, TV_RISE);

		s->addEvent720(360, T_PRIMARY, TV_RISE);

		s->addEvent720(385, T_SECONDARY, TV_FALL);
		s->addEvent720(390, T_SECONDARY, TV_RISE);
		s->addEvent720(395, T_SECONDARY, TV_FALL);
		s->addEvent720(400, T_SECONDARY, TV_RISE);
		s->addEvent720(405, T_SECONDARY, TV_FALL);
		s->addEvent720(410, T_SECONDARY, TV_RISE);
		s->addEvent720(415, T_SECONDARY, TV_FALL);
		s->addEvent720(420, T_SECONDARY, TV_RISE);
		s->addEvent720(425, T_SECONDARY, TV_FALL);
		s->addEvent720(430, T_SECONDARY, TV_RISE);
		s->addEvent720(435, T_SECONDARY, TV_FALL);
		s->addEvent720(440, T_SECONDARY, TV_RISE);
		s->addEvent720(445, T_SECONDARY, TV_FALL);
		s->addEvent720(450, T_SECONDARY, TV_RISE);
		s->addEvent720(455, T_SECONDARY, TV_FALL);
		s->addEvent720(460, T_SECONDARY, TV_RISE);
		s->addEvent720(465, T_SECONDARY, TV_FALL);
		s->addEvent720(470, T_SECONDARY, TV_RISE);
		s->addEvent720(475, T_SECONDARY, TV_FALL);
		s->addEvent720(480, T_SECONDARY, TV_RISE);
		s->addEvent720(485, T_SECONDARY, TV_FALL);
		s->addEvent720(490, T_SECONDARY, TV_RISE);
		s->addEvent720(495, T_SECONDARY, TV_FALL);
		s->addEvent720(500, T_SECONDARY, TV_RISE);

		s->addEvent720(504, T_PRIMARY, TV_FALL);

		s->addEvent720(505, T_SECONDARY, TV_FALL);
		s->addEvent720(510, T_SECONDARY, TV_RISE);
		s->addEvent720(515, T_SECONDARY, TV_FALL);
		s->addEvent720(520, T_SECONDARY, TV_RISE);
		s->addEvent720(525, T_SECONDARY, TV_FALL);
		s->addEvent720(530, T_SECONDARY, TV_RISE);
		s->addEvent720(535, T_SECONDARY, TV_FALL);
		s->addEvent720(540 - EPS_ANGLE, T_SECONDARY, TV_RISE);

		s->addEvent720(540, T_PRIMARY, TV_RISE);

		s->addEvent720(545, T_SECONDARY, TV_FALL);
		s->addEvent720(570, T_SECONDARY, TV_RISE);
		s->addEvent720(575, T_SECONDARY, TV_FALL);

		s->addEvent720(576, T_PRIMARY, TV_FALL);
		s->addEvent720(580, T_SECONDARY, TV_RISE);
		s->addEvent720(585, T_SECONDARY, TV_FALL);
		s->addEvent720(590, T_SECONDARY, TV_RISE);
		s->addEvent720(595, T_SECONDARY, TV_FALL);
		s->addEvent720(600, T_SECONDARY, TV_RISE);
		s->addEvent720(605, T_SECONDARY, TV_FALL);
		s->addEvent720(610, T_SECONDARY, TV_RISE);

		s->addEvent720(612, T_PRIMARY, TV_RISE);
		s->addEvent720(615, T_SECONDARY, TV_FALL);
		s->addEvent720(620, T_SECONDARY, TV_RISE);
		s->addEvent720(625, T_SECONDARY, TV_FALL);
		s->addEvent720(630, T_SECONDARY, TV_RISE);
		s->addEvent720(635, T_SECONDARY, TV_FALL);
		s->addEvent720(640, T_SECONDARY, TV_RISE);
		s->addEvent720(645, T_SECONDARY, TV_FALL);

		s->addEvent720(648, T_PRIMARY, TV_FALL);
		s->addEvent720(650, T_SECONDARY, TV_RISE);
		s->addEvent720(655, T_SECONDARY, TV_FALL);
		s->addEvent720(660, T_SECONDARY, TV_RISE);
		s->addEvent720(665, T_SECONDARY, TV_FALL);
		s->addEvent720(670, T_SECONDARY, TV_RISE);
		s->addEvent720(675, T_SECONDARY, TV_FALL);
		s->addEvent720(680, T_SECONDARY, TV_RISE);

		s->addEvent720(684, T_PRIMARY, TV_RISE);
		s->addEvent720(685, T_SECONDARY, TV_FALL);
		s->addEvent720(690, T_SECONDARY, TV_RISE);
		s->addEvent720(695, T_SECONDARY, TV_FALL);
		s->addEvent720(700, T_SECONDARY, TV_RISE);
		s->addEvent720(705, T_SECONDARY, TV_FALL);
		s->addEvent720(710, T_SECONDARY, TV_RISE);
		s->addEvent720(715, T_SECONDARY, TV_FALL);
		s->addEvent720(720 - EPS_ANGLE, T_SECONDARY, TV_RISE);
		s->addEvent720(720, T_PRIMARY, TV_FALL);
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
	s->addEvent720(angle, T_PRIMARY, TV_RISE);
	s->addEvent720(angle + w, T_PRIMARY, TV_FALL);
	angle += g;
	s->addEvent720(angle, T_PRIMARY, TV_RISE);
	s->addEvent720(angle + w, T_PRIMARY, TV_FALL);

	base += 120;
	// 3 teeth
	angle = base + 120.0 - w;
	s->addEvent720(angle, T_PRIMARY, TV_RISE);
	s->addEvent720(angle + w, T_PRIMARY, TV_FALL);
	angle += g;
	s->addEvent720(angle, T_PRIMARY, TV_RISE);
	s->addEvent720(angle + w, T_PRIMARY, TV_FALL);
	angle += g;
	s->addEvent720(angle, T_PRIMARY, TV_RISE);
	s->addEvent720(angle + w, T_PRIMARY, TV_FALL);

	base += 120;
	// 2 teeth
	angle = base + 120.0 - w;
	s->addEvent720(angle, T_PRIMARY, TV_RISE);
	s->addEvent720(angle + w, T_PRIMARY, TV_FALL);
	angle += g;
	s->addEvent720(angle, T_PRIMARY, TV_RISE);
	s->addEvent720(angle + w, T_PRIMARY, TV_FALL);

	base += 120;
	// just one
	angle = base + 120.0 - w;
	s->addEvent720(angle, T_PRIMARY, TV_RISE);
	s->addEvent720(angle + w, T_PRIMARY, TV_FALL);

	base += 120;
	// 3 teeth
	angle = base + 120.0 - w;
	s->addEvent720(angle, T_PRIMARY, TV_RISE);
	s->addEvent720(angle + w, T_PRIMARY, TV_FALL);
	angle += g;
	s->addEvent720(angle, T_PRIMARY, TV_RISE);
	s->addEvent720(angle + w, T_PRIMARY, TV_FALL);
	angle += g;
	s->addEvent720(angle, T_PRIMARY, TV_RISE);
	s->addEvent720(angle + w, T_PRIMARY, TV_FALL);

	base += 120;
	// just one again
	angle = base + 120.0 - w;
	s->addEvent720(angle, T_PRIMARY, TV_RISE);
	s->addEvent720(angle + w, T_PRIMARY, TV_FALL);
}

static void configureNeon1995TriggerWaveformCommon(bool withCam, TriggerWaveform *s) {
	trigger_wheel_e crank = withCam ? T_SECONDARY : T_PRIMARY;

	// voodoo magic - we always need 720 at the end
	int base = withCam ? 720 - 560 : 360 - 135;

	float m = withCam ? 1 : 2;

	s->addEvent720(m * (base - 120), crank, TV_RISE);
	s->addEvent720(m * (base - 116), crank, TV_FALL);
	s->addEvent720(m * (base - 720 + 616), crank, TV_RISE);
	s->addEvent720(m * (base - 100), crank, TV_FALL);
	s->addEvent720(m * (base - 720 + 643), crank, TV_RISE);
	s->addEvent720(m * (base - 720 + 648), crank, TV_FALL);
	s->addEvent720(m * (base - 720 + 671), crank, TV_RISE);
	s->addEvent720(m * (base - 44), crank, TV_FALL);

	if (withCam) {
		s->addEvent720(base + 0, T_PRIMARY, TV_FALL);
	}
	s->addEvent720(m * (base + 20), crank, TV_RISE);
	s->addEvent720(m * (base + 60), crank, TV_FALL);
	s->addEvent720(m * (base + 75), crank, TV_RISE);
	s->addEvent720(m * (base + 79), crank, TV_FALL);
	s->addEvent720(m * (base + 101), crank, TV_RISE);
	s->addEvent720(m * (base + 106), crank, TV_FALL);
	s->addEvent720(m * (base + 130), crank, TV_RISE);
	s->addEvent720(m * (base + 135), crank, TV_FALL);

	if (withCam) {
		s->addEvent720(base + 200, T_PRIMARY, TV_RISE); // width = 150

		s->addEvent720(base + 236, crank, TV_RISE);
		s->addEvent720(base + 239, crank, TV_FALL);
		s->addEvent720(base + 250, crank, TV_RISE);
		s->addEvent720(base + 255, crank, TV_FALL);
		s->addEvent720(base + 277, crank, TV_RISE);
		s->addEvent720(base + 282, crank, TV_FALL);
		s->addEvent720(base + 305, crank, TV_RISE);
		s->addEvent720(base + 310, crank, TV_FALL);

	s->addEvent720(base + 374, crank, TV_RISE);

//	if (withCam)
		s->addEvent720(base + 395, T_PRIMARY, TV_FALL); // width =

	s->addEvent720(base + 418, crank, TV_FALL);
	s->addEvent720(base + 436, crank, TV_RISE);
	s->addEvent720(base + 441, crank, TV_FALL);
	s->addEvent720(base + 463, crank, TV_RISE);
	s->addEvent720(base + 468, crank, TV_FALL);
	s->addEvent720(base + 492, crank, TV_RISE);
	s->addEvent720(base + 497, crank, TV_FALL);

//	if (withCam)
		s->addEvent720(base + 560, T_PRIMARY, TV_RISE); // width =

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


	s->initialState[T_PRIMARY] = TV_RISE;

	configureNeon1995TriggerWaveformCommon(true, s);

	s->useOnlyPrimaryForSync = true;
}

void initJeep18_2_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->isSynchronizationNeeded = false;
	s->tdcPosition = 581;

	float off = 212;

	s->addEvent720(534 - 720 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(536 - 720 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(554 - 720 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(556 - 720 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(574 - 720 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(576 - 720 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(594 - 720 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(596 - 720 + off, T_SECONDARY, TV_FALL);

	s->addEvent720(654 - 720 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(656 - 720 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(674 - 720 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(676 - 720 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(694 - 720 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(696 - 720 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(714 - 720 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(716 - 720 + off, T_SECONDARY, TV_FALL);


	s->addEvent720(54 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(56 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(74 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(76 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(94 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(96 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(114 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(116 + off, T_SECONDARY, TV_FALL);

	s->addEvent720(148 + off, T_PRIMARY, TV_RISE); // 360

	s->addEvent720(174 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(176 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(194 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(196 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(214 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(216 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(234 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(236 + off, T_SECONDARY, TV_FALL);

	s->addEvent720(294 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(296 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(314 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(316 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(334 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(336 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(354 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(356 + off, T_SECONDARY, TV_FALL);

	s->addEvent720(414 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(416 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(434 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(436 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(454 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(456 + off, T_SECONDARY, TV_FALL);
	s->addEvent720(474 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(476 + off, T_SECONDARY, TV_FALL);

	s->addEvent720(508 + off, T_PRIMARY, TV_FALL); // 720

}

static void add4cylblock(int off, TriggerWaveform *s) {
	s->addEvent720(114 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(114 + off + 2, T_SECONDARY, TV_FALL);

	s->addEvent720(134 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(134 + off + 2, T_SECONDARY, TV_FALL);

	s->addEvent720(154 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(154 + off + 2, T_SECONDARY, TV_FALL);

	s->addEvent720(174 + off, T_SECONDARY, TV_RISE);
	s->addEvent720(174 + off + 2, T_SECONDARY, TV_FALL);
}

// TT_JEEP_4_CYL
void initJeep_XJ_4cyl_2500(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);
	s->isSynchronizationNeeded = false;
	s->tdcPosition = 720 - 236;

	float offset = 124;

	add4cylblock(offset - 180, s);
	add4cylblock(offset, s);

	s->addEvent720(236 + offset, T_PRIMARY, TV_RISE); // 360

	add4cylblock(offset + 180, s);
	add4cylblock(offset + 360, s);

	s->addEvent720(596 + offset, T_PRIMARY, TV_FALL); // 720

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
		s->addEventAngle(base + narrow / 2, T_PRIMARY, TV_RISE);
		s->addEventAngle(base + narrow, T_PRIMARY, TV_FALL);
		base += narrow;
	}

	s->addEventAngle(base + narrow / 2, T_PRIMARY, TV_RISE);
	base += narrow / 2;
	s->addEventAngle(base + wide, T_PRIMARY, TV_FALL);
	base += wide;

	for (int i = 0; i < 16; i++) {
		s->addEventAngle(base + narrow / 2, T_PRIMARY, TV_RISE);
		s->addEventAngle(base + narrow, T_PRIMARY, TV_FALL);
		base += narrow;
	}

	// one small tooth at the end of the engine cycle
	s->addEventAngle(s->getCycleDuration() - narrow / 2, T_PRIMARY, TV_RISE);
	s->addEventAngle(s->getCycleDuration(), T_PRIMARY, TV_FALL);
	s->useOnlyPrimaryForSync = true;
}
