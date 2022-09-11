#include "pch.h"

#include "trigger_mercedes.h"
#include "trigger_structure.h"

void setMercedesTwoSegment(TriggerWaveform* s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR);

	s->addEvent360(180 - 10, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(180, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	s->addEvent360(227 - 10, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(227, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	s->addEvent360(360 - 10, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(360, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	s->setTriggerSynchronizationGap(1.35);
	s->setSecondTriggerSynchronizationGap(2.84);
	s->setThirdTriggerSynchronizationGap(0.26);
}
