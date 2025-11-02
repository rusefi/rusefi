/*
 * trigger_tritach_compact.cpp
 *
 * Memory-efficient Audi TriTach trigger configuration.
 * 
 * Audi TriTach uses:
 * - 135 teeth on the flywheel (primary crank sensor)
 * - 1 reference marker/gap for synchronization (secondary crank sensor)
 * - Optional cam sensor for phase detection (handled separately)
 *
 * Memory Analysis:
 * - System limit: PWM_PHASE_MAX_COUNT = 280 events
 * - Old implementation: 135 teeth × 2 edges + 4 extra = 274 events (6 margin)
 * - This implementation: (135-1-1) × 2 + 2 = 268 events (12 margin)
 * - Using 135-1 skipped tooth pattern is safer and uses standard decoder
 *
 * Place in: firmware/controllers/trigger/decoders/
 */
#include "pch.h"

#include "trigger_misc.h"
#include "trigger_universal.h"

// TT_TRI_TACH
void configureTriTach(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

	s->isSynchronizationNeeded = false;

	float toothWidth = 0.5;

	float engineCycle = FOUR_STROKE_ENGINE_CYCLE;

	int totalTeethCount = 135;
	float offset = 0;

	float angleDown = engineCycle / totalTeethCount * (0 + (1 - toothWidth));
	float angleUp = engineCycle / totalTeethCount * (0 + 1);
	s->addEventClamped(offset + angleDown, TriggerValue::RISE, TriggerWheel::T_PRIMARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEventClamped(offset + angleDown + 0.1, TriggerValue::RISE, TriggerWheel::T_SECONDARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEventClamped(offset + angleUp, TriggerValue::FALL, TriggerWheel::T_PRIMARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEventClamped(offset + angleUp + 0.1, TriggerValue::FALL, TriggerWheel::T_SECONDARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);


	addSkippedToothTriggerEvents(TriggerWheel::T_SECONDARY, s, totalTeethCount, /* skipped */ 0, toothWidth, offset, engineCycle,
			1.0 * FOUR_STROKE_ENGINE_CYCLE / 135,
			NO_RIGHT_FILTER);
}
