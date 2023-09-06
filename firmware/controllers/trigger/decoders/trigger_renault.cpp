#include "pch.h"
#include "trigger_renault.h"
#include "trigger_universal.h"

// Renault F3R
void initialize60_2_2_Renault_F(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);
	int totalTeethCount = 60;

	float engineCycle = FOUR_STROKE_ENGINE_CYCLE;
    float toothWidth = 0.5;

    float oneTooth = engineCycle / totalTeethCount;

    int skipped = 3; // yes, from this perspective only three are skipped

    // remaining 56 teeth while filtering four out
    addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, totalTeethCount, skipped, toothWidth, /*offset*/0, engineCycle,
    		NO_LEFT_FILTER, 719);

#if EFI_UNIT_TEST
    criticalAssertVoid(s->wave.phaseCount == (totalTeethCount - skipped) * 2 - 1, "Tooth count 60-3");
#endif // EFI_UNIT_TEST

    float specialPosition = 58 * oneTooth;

    // single twice-the-width tooth
    // we have that weird API where last FALL is added as 720, all while we do not really care about FALL fronts :(
    addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/0, engineCycle,
    		specialPosition - 1, specialPosition - 1 + oneTooth);

#if EFI_UNIT_TEST
    criticalAssertVoid(s->wave.phaseCount == (totalTeethCount - skipped) * 2 + 1, "Tooth count 60-2-2");
#endif // EFI_UNIT_TEST

    s->addEvent(1, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	s->setTriggerSynchronizationGap(0.5);
	s->setSecondTriggerSynchronizationGap(1);
	s->setThirdTriggerSynchronizationGap(2);
}
