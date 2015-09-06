/**
 * @file	trigger_chrysler.cpp
 *
 * @date Mar 24, 2014
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "trigger_chrysler.h"
#include "trigger_decoder.h"

void initDodgeRam(TriggerShape *s) {

	s->reset(FOUR_STROKE_CAM_SENSOR, true);
	s->useRiseEdge = true;

	s->isSynchronizationNeeded = false;

	addSkippedToothTriggerEvents(T_SECONDARY, s, 8, 0, 0.06, -25, 360, 0, 720);

	s->addEvent(360, T_PRIMARY, TV_HIGH);

	addSkippedToothTriggerEvents(T_SECONDARY, s, 8, 0, 0.06, 360 - 25, 360, 0, 720);

	s->addEvent(720, T_PRIMARY, TV_LOW);
}

void configureNeon2003TriggerShape(TriggerShape *s) {
	bool_t useOnlyPrimary = false;

	s->reset(FOUR_STROKE_CAM_SENSOR, !useOnlyPrimary);
	s->useRiseEdge = true;
	s->gapBothDirections = true;

	// are these non-default values really needed here now that the gap is finally precise?
	s->setTriggerSynchronizationGap2(0.5 * CHRYSLER_NGC4_GAP, 1.5 * CHRYSLER_NGC4_GAP);

	s->useOnlyPrimaryForSync = true;

	if (useOnlyPrimary) {


	s->addEvent(143.0712499, T_PRIMARY, TV_HIGH);
	s->addEvent(182.684791, T_PRIMARY, TV_LOW);

	s->addEvent(215.424166, T_PRIMARY, TV_HIGH);
	s->addEvent(256.247395, T_PRIMARY, TV_LOW);

	s->addEvent(288.659999, T_PRIMARY, TV_HIGH);
	s->addEvent(326.778229, T_PRIMARY, TV_LOW);

	s->addEvent(361.522083, T_PRIMARY, TV_HIGH);
	s->addEvent(508.960416, T_PRIMARY, TV_LOW);

	s->addEvent(545.635, T_PRIMARY, TV_HIGH);
	s->addEvent(582.98864, T_PRIMARY, TV_LOW);

	s->addEvent(617.22989, T_PRIMARY, TV_HIGH);
	s->addEvent(653.38625, T_PRIMARY, TV_LOW);

	s->addEvent(684.1080, T_PRIMARY, TV_HIGH);
	s->addEvent(720, T_PRIMARY, TV_LOW);

	} else {

	s->addEvent(25.557796298609748, T_SECONDARY, TV_LOW);
	s->addEvent(29.669658950975318, T_SECONDARY, TV_HIGH);
	s->addEvent(34.47539842592758, T_SECONDARY, TV_LOW);
	s->addEvent(38.47161494119537, T_SECONDARY, TV_HIGH);
	s->addEvent(43.11030999589528, T_SECONDARY, TV_LOW);
	s->addEvent(46.990880374065284, T_SECONDARY, TV_HIGH);
	s->addEvent(51.68097371191976, T_SECONDARY, TV_LOW);
	s->addEvent(55.6000928024557, T_SECONDARY, TV_HIGH);
	s->addEvent(60.3287348526761, T_SECONDARY, TV_LOW);
	s->addEvent(64.15790694769154, T_SECONDARY, TV_HIGH);
	s->addEvent(69.18208912605073, T_SECONDARY, TV_LOW);
	s->addEvent(73.80793460996199, T_SECONDARY, TV_HIGH);
	s->addEvent(78.36953223993004, T_SECONDARY, TV_LOW);
	s->addEvent(82.55849231702747, T_SECONDARY, TV_HIGH);
	s->addEvent(88.00671033141185, T_SECONDARY, TV_LOW);
	s->addEvent(92.47836096585941, T_SECONDARY, TV_HIGH);
	s->addEvent(98.36346438705763, T_SECONDARY, TV_LOW);
	s->addEvent(103.5675405564578, T_SECONDARY, TV_HIGH);
	s->addEvent(109.31129869898096, T_SECONDARY, TV_LOW);
	s->addEvent(113.8985954705263, T_SECONDARY, TV_HIGH);
	s->addEvent(119.42391090964253, T_SECONDARY, TV_LOW);
	s->addEvent(124.10115467670836, T_SECONDARY, TV_HIGH);
	s->addEvent(129.92201024396337, T_SECONDARY, TV_LOW);
	s->addEvent(134.66350186497243, T_SECONDARY, TV_HIGH);
	s->addEvent(137.13061945639177, T_PRIMARY, TV_HIGH);
	s->addEvent(140.67710099405707, T_SECONDARY, TV_LOW);
	s->addEvent(145.35434476112292, T_SECONDARY, TV_HIGH);
	s->addEvent(151.40649260257348, T_SECONDARY, TV_LOW);
	s->addEvent(156.2636303606803, T_SECONDARY, TV_HIGH);
	s->addEvent(162.59846875948102, T_SECONDARY, TV_LOW);
	s->addEvent(167.49415522995378, T_SECONDARY, TV_HIGH);
	s->addEvent(173.59770135455892, T_SECONDARY, TV_LOW);
	s->addEvent(178.5319365373976, T_SECONDARY, TV_HIGH);
	s->addEvent(182.83654275159282, T_PRIMARY, TV_LOW);
	s->addEvent(183.69746399443184, T_SECONDARY, TV_LOW);
	s->addEvent(208.11164849285242, T_SECONDARY, TV_HIGH);
	s->addEvent(213.90680491853016, T_SECONDARY, TV_LOW);
	s->addEvent(214.4978851748077, T_PRIMARY, TV_HIGH);
	s->addEvent(217.71027787196832, T_SECONDARY, TV_HIGH);
	s->addEvent(222.63166348401833, T_SECONDARY, TV_LOW);
	s->addEvent(227.0133671229454, T_SECONDARY, TV_HIGH);
	s->addEvent(231.26657505398603, T_SECONDARY, TV_LOW);
	s->addEvent(235.2499419984652, T_SECONDARY, TV_HIGH);
	s->addEvent(239.99143361947424, T_SECONDARY, TV_LOW);
	s->addEvent(244.11614584262844, T_SECONDARY, TV_HIGH);
	s->addEvent(248.92188531758072, T_SECONDARY, TV_LOW);
	s->addEvent(250.2196919672336, T_PRIMARY, TV_LOW);
	s->addEvent(252.89240269127123, T_SECONDARY, TV_HIGH);
	s->addEvent(258.2121249977692, T_SECONDARY, TV_LOW);
	s->addEvent(262.6066782074849, T_SECONDARY, TV_HIGH);
	s->addEvent(267.9649492263488, T_SECONDARY, TV_LOW);
	s->addEvent(272.3081041529099, T_SECONDARY, TV_HIGH);
	s->addEvent(278.36025199436045, T_SECONDARY, TV_LOW);
	s->addEvent(282.0352292399122, T_PRIMARY, TV_HIGH);
	s->addEvent(283.53862902218333, T_SECONDARY, TV_HIGH);
	s->addEvent(289.20528973997466, T_SECONDARY, TV_LOW);
	s->addEvent(293.9082326486178, T_SECONDARY, TV_HIGH);
	s->addEvent(299.20225581353844, T_SECONDARY, TV_LOW);
	s->addEvent(304.033694430068, T_SECONDARY, TV_HIGH);
	s->addEvent(309.7903021433798, T_SECONDARY, TV_LOW);
	s->addEvent(314.77593560937305, T_SECONDARY, TV_HIGH);
	s->addEvent(320.37834847322114, T_SECONDARY, TV_LOW);
	s->addEvent(322.9996609141042, T_PRIMARY, TV_LOW);
	s->addEvent(325.04274266949835, T_SECONDARY, TV_HIGH);
	s->addEvent(331.1848375064694, T_SECONDARY, TV_LOW);
	s->addEvent(336.04197526457625, T_SECONDARY, TV_HIGH);
	s->addEvent(342.1969196723359, T_SECONDARY, TV_LOW);
	s->addEvent(346.8484642978245, T_SECONDARY, TV_HIGH);
	s->addEvent(353.2732496921457, T_SECONDARY, TV_LOW);
	s->addEvent(358.34883015365943, T_SECONDARY, TV_HIGH);
	s->addEvent(360.7902486035015, T_PRIMARY, TV_HIGH);
	s->addEvent(384.20216658040795, T_SECONDARY, TV_LOW);
	s->addEvent(388.53247193618046, T_SECONDARY, TV_HIGH);
	s->addEvent(393.22256527403493, T_SECONDARY, TV_LOW);
	s->addEvent(397.06458693983905, T_SECONDARY, TV_HIGH);
	s->addEvent(401.98597255188906, T_SECONDARY, TV_LOW);
	s->addEvent(406.2391804829297, T_SECONDARY, TV_HIGH);
	s->addEvent(410.71083111737727, T_SECONDARY, TV_LOW);
	s->addEvent(414.86124248210876, T_SECONDARY, TV_HIGH);
	s->addEvent(419.5770349615405, T_SECONDARY, TV_LOW);
	s->addEvent(423.4833044812878, T_SECONDARY, TV_HIGH);
	s->addEvent(428.6616815091107, T_SECONDARY, TV_LOW);
	s->addEvent(433.1204825727696, T_SECONDARY, TV_HIGH);
	s->addEvent(438.1446647511288, T_SECONDARY, TV_LOW);
	s->addEvent(442.32077525743756, T_SECONDARY, TV_HIGH);
	s->addEvent(448.1287812539039, T_SECONDARY, TV_LOW);
	s->addEvent(453.84684025484984, T_SECONDARY, TV_HIGH);
	s->addEvent(458.871022433209, T_SECONDARY, TV_LOW);
	s->addEvent(463.92090375314547, T_SECONDARY, TV_HIGH);
	s->addEvent(469.48476790462763, T_SECONDARY, TV_LOW);
	s->addEvent(473.7765245480342, T_SECONDARY, TV_HIGH);
	s->addEvent(479.5845305445006, T_SECONDARY, TV_LOW);
	s->addEvent(485.3282886870237, T_SECONDARY, TV_HIGH);
	s->addEvent(490.3396212945943, T_SECONDARY, TV_LOW);
	s->addEvent(495.27385647743296, T_SECONDARY, TV_HIGH);
	s->addEvent(500.82487105812646, T_SECONDARY, TV_LOW);
	s->addEvent(505.3864686880945, T_SECONDARY, TV_HIGH);
	s->addEvent(505.7719558117538, T_PRIMARY, TV_LOW);
	s->addEvent(511.8112540824157, T_SECONDARY, TV_LOW);
	s->addEvent(516.6940909820999, T_SECONDARY, TV_HIGH);
	s->addEvent(522.8233362482823, T_SECONDARY, TV_LOW);
	s->addEvent(527.7061731479664, T_SECONDARY, TV_HIGH);
	s->addEvent(533.9510645512465, T_SECONDARY, TV_LOW);
	s->addEvent(539.6562739814038, T_SECONDARY, TV_HIGH);
	s->addEvent(543.9480306248104, T_SECONDARY, TV_LOW);
	s->addEvent(546.5179447825388, T_PRIMARY, TV_HIGH);
	s->addEvent(568.722003105313, T_SECONDARY, TV_HIGH);
	s->addEvent(573.7590348544608, T_SECONDARY, TV_LOW);
	s->addEvent(577.6524548034195, T_SECONDARY, TV_HIGH);
	s->addEvent(580.4536612353435, T_PRIMARY, TV_LOW);
	s->addEvent(582.4710438491603, T_SECONDARY, TV_LOW);
	s->addEvent(586.7114022094123, T_SECONDARY, TV_HIGH);
	s->addEvent(591.1316545607053, T_SECONDARY, TV_LOW);
	s->addEvent(594.9479770849321, T_SECONDARY, TV_HIGH);
	s->addEvent(599.9336105509253, T_SECONDARY, TV_LOW);
	s->addEvent(603.8912783538273, T_SECONDARY, TV_HIGH);
	s->addEvent(608.9540092445524, T_SECONDARY, TV_LOW);
	s->addEvent(610.8942944336374, T_PRIMARY, TV_HIGH);
	s->addEvent(613.8111470026591, T_SECONDARY, TV_HIGH);
	s->addEvent(618.3727446326272, T_SECONDARY, TV_LOW);
	s->addEvent(622.5874038513019, T_SECONDARY, TV_HIGH);
	s->addEvent(628.2926132814591, T_SECONDARY, TV_LOW);
	s->addEvent(632.7514143451181, T_SECONDARY, TV_HIGH);
	s->addEvent(638.9063587528777, T_SECONDARY, TV_LOW);
	s->addEvent(643.7891956525618, T_SECONDARY, TV_HIGH);
	s->addEvent(648.1580497207003, T_PRIMARY, TV_LOW);
	s->addEvent(649.5972016490283, T_SECONDARY, TV_LOW);
	s->addEvent(654.0174540003212, T_SECONDARY, TV_HIGH);
	s->addEvent(659.6584155765353, T_SECONDARY, TV_LOW);
	s->addEvent(664.4384559099102, T_SECONDARY, TV_HIGH);
	s->addEvent(670.3364089018971, T_SECONDARY, TV_LOW);
	s->addEvent(674.975103956597, T_SECONDARY, TV_HIGH);
	s->addEvent(680.1277818428426, T_PRIMARY, TV_HIGH);
	s->addEvent(680.8730569485839, T_SECONDARY, TV_LOW);
	s->addEvent(685.4346545785519, T_SECONDARY, TV_HIGH);
	s->addEvent(691.8594399728731, T_SECONDARY, TV_LOW);
	s->addEvent(696.7037281601913, T_SECONDARY, TV_HIGH);
	s->addEvent(702.8329734263738, T_SECONDARY, TV_LOW);
	s->addEvent(707.8957043170989, T_SECONDARY, TV_HIGH);
	s->addEvent(714.0634982956473, T_SECONDARY, TV_LOW);
	s->addEvent(718.9463351953314, T_SECONDARY, TV_HIGH);
	s->addEvent(720.0, T_PRIMARY, TV_LOW);

	}
}

void configureDodgeStratusTriggerShape(TriggerShape *s) {
	s->reset(FOUR_STROKE_CAM_SENSOR, false);
	s->isSynchronizationNeeded = true;

	s->tdcPosition = 510;

	float w = 7;
	float g = 20;
	s->setTriggerSynchronizationGap2(CHRYSLER_NGC6_GAP - 0.5, CHRYSLER_NGC6_GAP + 0.5);

	float base = 0;
	float angle = base + 120.0 - w;
	s->addEvent(angle, T_PRIMARY, TV_HIGH);
	s->addEvent(angle + w, T_PRIMARY, TV_LOW);

	base += 120;
	// 3 teeth
	angle = base + 120.0 - w;
	s->addEvent(angle, T_PRIMARY, TV_HIGH);
	s->addEvent(angle + w, T_PRIMARY, TV_LOW);
	angle += g;
	s->addEvent(angle, T_PRIMARY, TV_HIGH);
	s->addEvent(angle + w, T_PRIMARY, TV_LOW);
	angle += g;
	s->addEvent(angle, T_PRIMARY, TV_HIGH);
	s->addEvent(angle + w, T_PRIMARY, TV_LOW);

	base += 120;
	// 2 teeth
	angle = base + 120.0 - w;
	s->addEvent(angle, T_PRIMARY, TV_HIGH);
	s->addEvent(angle + w, T_PRIMARY, TV_LOW);
	angle += g;
	s->addEvent(angle, T_PRIMARY, TV_HIGH);
	s->addEvent(angle + w, T_PRIMARY, TV_LOW);

	base += 120;
	// just one
	angle = base + 120.0 - w;
	s->addEvent(angle, T_PRIMARY, TV_HIGH);
	s->addEvent(angle + w, T_PRIMARY, TV_LOW);

	base += 120;
	angle = base + 120.0 - w;
	s->addEvent(angle, T_PRIMARY, TV_HIGH);
	s->addEvent(angle + w, T_PRIMARY, TV_LOW);
	angle += g;
	s->addEvent(angle, T_PRIMARY, TV_HIGH);
	s->addEvent(angle + w, T_PRIMARY, TV_LOW);
	angle += g;
	s->addEvent(angle, T_PRIMARY, TV_HIGH);
	s->addEvent(angle + w, T_PRIMARY, TV_LOW);


	base += 120;
	// just one again
	angle = base + 120.0 - w;
	s->addEvent(angle, T_PRIMARY, TV_HIGH);
	s->addEvent(angle + w, T_PRIMARY, TV_LOW);
}

void configureNeon1995TriggerShape(TriggerShape *s) {
	s->reset(FOUR_STROKE_CAM_SENSOR, true);

	s->setTriggerSynchronizationGap(0.72);

	s->useRiseEdge = false;

	// voodoo magic - we always need 720 at the end
	int base = 720 - 560;

	s->initialState[T_PRIMARY] = 1;

	s->addEvent(base - 720 + 600, T_SECONDARY, TV_HIGH);
	s->addEvent(base - 720 + 604, T_SECONDARY, TV_LOW);
	s->addEvent(base - 720 + 616, T_SECONDARY, TV_HIGH);
	s->addEvent(base - 720 + 620, T_SECONDARY, TV_LOW);
	s->addEvent(base - 720 + 643, T_SECONDARY, TV_HIGH);
	s->addEvent(base - 720 + 648, T_SECONDARY, TV_LOW);
	s->addEvent(base - 720 + 671, T_SECONDARY, TV_HIGH);
	s->addEvent(base - 720 + 676, T_SECONDARY, TV_LOW);

	s->addEvent(base + 0, T_PRIMARY, TV_LOW);

	s->addEvent(base + 20, T_SECONDARY, TV_HIGH);
	s->addEvent(base + 60, T_SECONDARY, TV_LOW);
	s->addEvent(base + 75, T_SECONDARY, TV_HIGH);
	s->addEvent(base + 79, T_SECONDARY, TV_LOW);
	s->addEvent(base + 101, T_SECONDARY, TV_HIGH);
	s->addEvent(base + 106, T_SECONDARY, TV_LOW);
	s->addEvent(base + 130, T_SECONDARY, TV_HIGH);
	s->addEvent(base + 135, T_SECONDARY, TV_LOW);

	s->addEvent(base + 200, T_PRIMARY, TV_HIGH); // width = 150

	s->addEvent(base + 236, T_SECONDARY, TV_HIGH);
	s->addEvent(base + 239, T_SECONDARY, TV_LOW);
	s->addEvent(base + 250, T_SECONDARY, TV_HIGH);
	s->addEvent(base + 255, T_SECONDARY, TV_LOW);
	s->addEvent(base + 277, T_SECONDARY, TV_HIGH);
	s->addEvent(base + 282, T_SECONDARY, TV_LOW);
	s->addEvent(base + 305, T_SECONDARY, TV_HIGH);
	s->addEvent(base + 310, T_SECONDARY, TV_LOW);

	s->addEvent(base + 374, T_SECONDARY, TV_HIGH);

	s->addEvent(base + 395, T_PRIMARY, TV_LOW); // width =

	s->addEvent(base + 418, T_SECONDARY, TV_LOW);
	s->addEvent(base + 436, T_SECONDARY, TV_HIGH);
	s->addEvent(base + 441, T_SECONDARY, TV_LOW);
	s->addEvent(base + 463, T_SECONDARY, TV_HIGH);
	s->addEvent(base + 468, T_SECONDARY, TV_LOW);
	s->addEvent(base + 492, T_SECONDARY, TV_HIGH);
	s->addEvent(base + 497, T_SECONDARY, TV_LOW);

	s->addEvent(base + 560, T_PRIMARY, TV_HIGH); // width =
}

