//Aug 18, 2025 @Creesic

#include "pch.h"
#include "trigger_structure.h"
#include "trigger_bmw.h"

void initializeVvtVanosReluctor(TriggerWaveform *s) {
    s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Fall);

    s->shapeWithoutTdc = true;

	// SyncEdge::Fall tracks fall-to-fall durations 155/90/115, ratios 0.58/1.28/1.35:
	// sync on the unique short-after-long 0.58 gap, the other two are too close to each other
	s->setTriggerSynchronizationGap2(0.4, 0.8);

    // cam-degree teeth (19.5, 44), (109.5, 199), (219.5, 289) measured from a real N52
    // capture (unit_tests/tests/trigger/resources/real-bmw-e90.teeth, TEST(realBmwE90, realCams)),
    // rotated by +71 so the last edge lands at cycle end
    s->addEvent360(90.5f, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
    s->addEvent360(115, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

    s->addEvent360(180.5f, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
    s->addEvent360(270, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

    s->addEvent360(290.5f, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
    s->addEvent360(360, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
}
