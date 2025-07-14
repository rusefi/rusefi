/**
 * @file	trigger_renix.cpp
 *
 * https://en.wikipedia.org/wiki/Renix
 * Has something to do with AMC/Jeep
 *
 * @date Apr 18, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_renix.h"

static void commonRenix(TriggerWaveform *s) {
	// 44-2-2 is symmetrical so we only need to define one half
	int count = 22;
	float tooth = s->getCycleDuration() / count; // hint: tooth = 8.181818 degrees

	float currentAngle = 0;
	for (int i = 0;i < 20;i++) {
		s->addEventAngle(currentAngle + tooth / 2, TriggerValue::RISE);
		s->addEventAngle(currentAngle + tooth, TriggerValue::FALL);
		currentAngle += tooth;
	}

	s->addEventAngle(currentAngle + tooth, TriggerValue::RISE);

	// float math error accumulates at this point so we have to spell out 180
	s->addEventAngle(s->getCycleDuration(), TriggerValue::FALL);
}

// TT_RENIX_44_2_2
void initializeRenix44_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR, SyncEdge::RiseOnly);
	commonRenix(s);
}

// TT_RENIX_66_2_2_2
void initializeRenix66_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_THREE_TIMES_CRANK_SENSOR, SyncEdge::RiseOnly);
	commonRenix(s);
}

// basically like the "normal" renix 22-2 pattern -> 66-2-2-2, except without the extra random tooth there in the middle.
// this is copied from vq35 36-2-2-2 pattern.
void JeepRenix66_2_2(TriggerWaveform* s, size_t halfCylinderCount, size_t totalWheel, size_t missing) {

	auto toothAngle = 360.0f / totalWheel;

	auto patternTeeth = totalWheel / halfCylinderCount;
	auto toothCount = patternTeeth - missing;

	float currentAngle = missing * toothAngle;
	for (size_t i = 0; i < toothCount; i++) {
		currentAngle += toothAngle;
		s->addEventAngle(currentAngle - (toothAngle/2), TriggerValue::RISE);
		if (i==toothCount-1) { // last event at 120
			s->addEventAngle( s->getCycleDuration(), TriggerValue::FALL);
		} else {
			s->addEventAngle(currentAngle, TriggerValue::FALL);
		}
	}
}

// TT_JEEPRENIX_66_2_2_2
void initializeJeepRenix66_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_THREE_TIMES_CRANK_SENSOR, SyncEdge::RiseOnly);

	// 6 cylinder = 66 tooth wheel, missing 2 teeth in 3 spots
	JeepRenix66_2_2(s, 3, 66, 2);
	s->setTriggerSynchronizationGap3(/*gapIndex*/0, 0.2, 0.5);
	s->setTriggerSynchronizationGap3(/*gapIndex*/1, 2, 4);
	s->setTriggerSynchronizationGap3(/*gapIndex*/2, 0.6, 1.4);
}