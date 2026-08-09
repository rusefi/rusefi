//Aug 18, 2025 @Creesic

#include "pch.h"
#include "trigger_structure.h"
#include "trigger_bmw.h"

void initializeVvtVanosReluctor(TriggerWaveform *s) {
    s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Fall);

    s->shapeWithoutTdc = true;

	// SyncEdge::Fall tracks fall-to-fall durations 90/120/150, ratios 1.33/1.25/0.6:
	// sync on the unique 90/150 = 0.6 gap, the other two are too close to each other
	s->setTriggerSynchronizationGap2(0.4, 0.8);

    // teeth (10, 95), (125, 185), (280, 305) rotated by +55 so the last edge lands at cycle end
    s->addEvent360(65, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
    s->addEvent360(150, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

    s->addEvent360(180, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
    s->addEvent360(240, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

    s->addEvent360(335, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
    s->addEvent360(360, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
}
