#include "trigger_mercedes.h"
#include "trigger_structure.h"

void setMercedesTwoSegment(TriggerWaveform* s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR);

	s->addEvent360(223 - 10, T_PRIMARY, TV_RISE);
	s->addEvent360(223, T_PRIMARY, TV_FALL);

	s->addEvent360(316 - 10, T_PRIMARY, TV_RISE);
	s->addEvent360(316, T_PRIMARY, TV_FALL);

	s->addEvent360(360 - 10, T_PRIMARY, TV_RISE);
	s->addEvent360(360, T_PRIMARY, TV_FALL);

	s->setTriggerSynchronizationGap(0.47);
	s->setSecondTriggerSynchronizationGap(0.42);
	s->setThirdTriggerSynchronizationGap(5.07);
}
