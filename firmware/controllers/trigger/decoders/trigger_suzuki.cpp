/*
 * trigger_suzuki.cpp
 *
 * @date Oct 4, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

#include "trigger_suzuki.h"

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

	s->setTriggerSynchronizationGap(0.22);
	s->setSecondTriggerSynchronizationGap(1);
}

void initializeSuzukiG16B(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	
	float w = 5; // width of tooth in degrees

	angle_t teethAngles[] = { 0, 15, 55, 90, 130, 145, 180, 235, 270, 310, 325 }; // angle center of tooth

	int size = sizeof(teethAngles) / sizeof(teethAngles[0]);
	angle_t offset = 360 - teethAngles[size - 1];

	for(int i = 0; i < size; i++) {
		angle_t pos = teethAngles[i];
		s->addToothRiseFall(pos + offset, w);
	}
	
	// Set sync gap based on largest gap between teeth
	// Calculate gaps: 15,40,35,40,15,35,55,35,40,15,35 (degrees)
	// Largest is 55° between 180-235, one before is 35°
	// Ratio largest/previous 1.57
	s->setTriggerSynchronizationGap(1.57);
	
	// Optional second gap if needed for reliable sync
	// s->setSecondTriggerSynchronizationGap(2.3);
}

void initializeSuzukiK6A(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	float w = 5;

	int secondTooth = 15;

	// a bit lame: we start with falling front of first tooth
	s->addEvent360(5, TriggerValue::FALL);

	s->addToothRiseFall(secondTooth, w);
	s->addToothRiseFall(43, w);

	s->addToothRiseFall(120, w);
	s->addToothRiseFall(120 + secondTooth, w);
	s->addToothRiseFall(158, w);
	s->addToothRiseFall(158 + secondTooth, w);

	s->addToothRiseFall(240, w);
	s->addToothRiseFall(240 + secondTooth, w);
	s->addToothRiseFall(283, w);

	// a bit lame: we end with rising front of first tooth
	s->addEvent360(360, TriggerValue::RISE);

	s->setTriggerSynchronizationGap(4.47);
	s->setSecondTriggerSynchronizationGap(0.65);
}
