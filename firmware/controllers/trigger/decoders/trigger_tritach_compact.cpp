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
 * To avoid memory overflow from 135 teeth × 2 edges = 270 events,
 * we use the skipped tooth mechanism (135-1 pattern).
 *
 * Place in: firmware/controllers/trigger/decoders/
 */

#include "pch.h"
#include "trigger_tritach_compact.h"
#include "trigger_universal.h"

// --- configureTriTachCompact (keeps TriggerWaveform metadata compact) ---
void configureTriTachCompact(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);
	
	// Audi Tritach has 135 teeth on the flywheel with one reference position
	// To save memory, we use the skipped tooth approach: 135-1
	// This adds only the representative pattern, not all 135 teeth
	
	int totalTeethCount = 135;
	int skippedCount = 1; // One position has a reference marker/gap
	float toothWidth = 0.5f;
	
	// Initialize as a skipped tooth trigger (135-1)
	// This will add a representative pattern without storing all teeth
	initializeSkippedToothTrigger(s, totalTeethCount, skippedCount, FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);
	
	// Configure synchronization gaps for reliable sync
	// With 135-1, the gap ratio should be around 2:1
	s->setTriggerSynchronizationGap(2);
	s->setSecondTriggerSynchronizationGap(1);
	
	s->shapeWithoutTdc = true;
	s->useOnlyRisingEdges = true;
}
