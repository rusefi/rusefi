#include "trigger_mercedes.h"
#include "trigger_structure.h"

void setMercedesTwoSegment(TriggerWaveform* s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR);

	s->addEvent360(180 - 10, T_PRIMARY, TV_RISE);
	s->addEvent360(180, T_PRIMARY, TV_FALL);

	s->addEvent360(227 - 10, T_PRIMARY, TV_RISE);
	s->addEvent360(227, T_PRIMARY, TV_FALL);

	s->addEvent360(360 - 10, T_PRIMARY, TV_RISE);
	s->addEvent360(360, T_PRIMARY, TV_FALL);

	s->setTriggerSynchronizationGap(1.35);
	s->setSecondTriggerSynchronizationGap(2.84);
	s->setThirdTriggerSynchronizationGap(0.26);
}
