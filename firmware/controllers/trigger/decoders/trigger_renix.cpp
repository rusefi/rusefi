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
	// 44-2-2 is symmetrical so we only need to define one half (180 crank degrees).
	// The half consists of 22 evenly spaced tooth slots out of which two are
	// missing. Real-world recordings (see unit_tests .../renix_44_2_2.csv) show
	// that the two missing teeth are NOT adjacent: they sit at slot 0 and slot 2,
	// i.e. the wheel produces two consecutive ~2x gaps separated by a single
	// regular tooth followed by 18 regular teeth.
	const int slotCount = 22;
	const float slot = s->getCycleDuration() / slotCount; // hint: slot = 8.181818 degrees

	for (int i = 0; i < slotCount; i++) {
		// the two missing teeth, one slot apart
		if (i == 0 || i == 2) {
			continue;
		}

		float slotStart = i * slot;
		s->addEventAngle(slotStart + slot / 2, TriggerValue::RISE);
		if (i == slotCount - 1) {
			// float math error accumulates at this point so we have to spell out 180
			s->addEventAngle(s->getCycleDuration(), TriggerValue::FALL);
		} else {
			s->addEventAngle(slotStart + slot, TriggerValue::FALL);
		}
	}

	// Sync on the tooth at slot 1: it follows the missing slot 0 (a 2x gap),
	// while the preceding tooth (slot 21 of the previous cycle) was a regular
	// tooth - so this/last = 2.0 and last/prev = 1.0.
	s->setTriggerSynchronizationGap3(/*gapIndex*/0, 1.5, 2.5);
	s->setTriggerSynchronizationGap3(/*gapIndex*/1, 0.75, 1.25);
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
