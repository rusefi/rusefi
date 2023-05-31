#include "pch.h"

#include "trigger_mercedes.h"
#include "trigger_structure.h"

void setMercedesTwoSegment(TriggerWaveform* s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::Rise);

	s->addEvent360(180 - 10, true);
	s->addEvent360(180, false);

	s->addEvent360(227 - 10, true);
	s->addEvent360(227, false);

	s->addEvent360(360 - 10, true);
	s->addEvent360(360, false);

	s->setTriggerSynchronizationGap(1.35);
	s->setSecondTriggerSynchronizationGap(2.84);
	s->setThirdTriggerSynchronizationGap(0.26);
}
