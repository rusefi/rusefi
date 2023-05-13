#include "pch.h"

#include "trigger_mercedes.h"
#include "trigger_structure.h"

void setMercedesTwoSegment(TriggerWaveform* s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::Rise);

	s->addToothRiseFall(180);

	s->addToothRiseFall(227);

	s->addToothRiseFall(360);

	s->setTriggerSynchronizationGap(1.35);
	s->setSecondTriggerSynchronizationGap(2.84);
	s->setThirdTriggerSynchronizationGap(0.26);
}
