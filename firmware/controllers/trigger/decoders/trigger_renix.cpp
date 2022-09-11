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
		s->addEventAngle(currentAngle + tooth / 2, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
		s->addEventAngle(currentAngle + tooth, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
		currentAngle += tooth;
	}

	s->addEventAngle(currentAngle + tooth, TriggerWheel::T_PRIMARY, TriggerValue::RISE);

	// float math error accumulates at this point so we have to spell out 180
	s->addEventAngle(s->getCycleDuration(), TriggerWheel::T_PRIMARY, TriggerValue::FALL);
}

// TT_RENIX_44_2_2
void initializeRenix44_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR);
	commonRenix(s);
}

// TT_RENIX_66_2_2_2
void initializeRenix66_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_THREE_TIMES_CRANK_SENSOR);
	commonRenix(s);
}
