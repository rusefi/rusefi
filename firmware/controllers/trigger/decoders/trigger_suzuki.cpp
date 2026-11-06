/*
 * trigger_suzuki.cpp
 *
 * @date Oct 4, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

#include "trigger_suzuki.h"

// Contributor-supplied waveform, reported working on an engine
// Two groups of 10 and 22 teeth, with two missing teeth between them.
// The requested 75-degree TDC baseline is unverified: see docs/triggers/suzuki-36-2-2.md.
void configureSuzuki36_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);
	s->tdcPosition = 75;

	// Sync on the second rising edge after the gap following the 22-tooth group.
	// The longer history rejects the otherwise identical gap after 10 teeth.
	s->setTriggerSynchronizationGap3(/*gapIndex*/0, 0.2f, 0.6f);
	s->setTriggerSynchronizationGap3(/*gapIndex*/1, 2.2f, 3.8f);
	for (int i = 2; i < 13; i++) {
		s->setTriggerSynchronizationGap3(/*gapIndex*/i, 0.7f, 1.6f);
	}

	angle_t base = 25;
	for (int i = 0; i < 10; i++) {
		s->addEventAngle(base, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
		s->addEventAngle(base + 5, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
		base += 10;
	}

	base += 20;
	for (int i = 0; i < 22; i++) {
		s->addEventAngle(base, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
		s->addEventAngle(base + 5, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
		base += 10;
	}
}

void initializeSuzukiG13B(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	float w = 5;
	float specialTooth = 20;

	s->addEvent720(180 - w, TriggerValue::RISE);
	s->addEvent720(180, TriggerValue::FALL);

	s->addEvent720(2 * specialTooth + 180 - w, TriggerValue::RISE);
	s->addEvent720(2 * specialTooth + 180, TriggerValue::FALL);

	s->addEvent720(360 - w, TriggerValue::RISE);
	s->addEvent720(360, TriggerValue::FALL);

	s->addEvent720(540 - w, TriggerValue::RISE);
	s->addEvent720(540, TriggerValue::FALL);

	s->addEvent720(720 - w, TriggerValue::RISE);
	s->addEvent720(720, TriggerValue::FALL);

	s->setTriggerSynchronizationGap(0.22f);
	s->setSecondTriggerSynchronizationGap(1);
}

void initializeSuzukiG16B(TriggerWaveform *s) {
	static const angle_t angles[] = { 35, 50, 90, 125, 165, 180, 215, 270, 305, 345, 360 };
	initializeRiseOnlyTrigger(s, 5, angles, efi::size(angles));

	// Set sync gap based on largest gap between teeth
	// Calculate gaps: 15,40,35,40,15,35,55,35,40,15,35 (degrees)
	// Largest is 55° between 180-235, one before is 35°
	// Ratio largest/previous 1.57
	s->setTriggerSynchronizationGap(1.57f);
}

// K6A non-VVT distributor, also used on EA21R Cappuccino: a single VR pickup with
// 10 teeth per cam revolution in a 4-3-3 arrangement. Groups start 120 cam degrees apart.
// Only the rising edges carry the pattern. Rising edge positions within a group,
// measured from EA21R cranking captures (see #7080 and test_real_suzuki_k6a.cpp):
//   three-tooth groups: 0, 15, 43
//   four-tooth group: 0, 15, 43.5, 57.5
void initializeSuzukiK6A(TriggerWaveform *s) {
	// Falling edge angles. Each rising edge is 5 degrees earlier.
	static const angle_t angles[] = { 15, 43, 120, 135, 163.5f, 177.5f, 240, 255, 283, 360 };
	initializeRiseOnlyTrigger(s, 5, angles, efi::size(angles));

	// Sync on the first tooth after the four-tooth group: a 62.5 degree gap after a 14 degree gap.
	// Nominal ratio 4.46. Measured 3.95 to 4.57 while cranking. The largest other ratio is 77/28 = 2.75.
	s->setTriggerSynchronizationGap3(/*gapIndex*/0, 3.3f, 6.0f);
	// Previous gap: 14 degrees after 28.5. Nominal ratio 0.49. Measured 0.48 to 0.55.
	s->setTriggerSynchronizationGap3(/*gapIndex*/1, 0.3f, 0.8f);
}
