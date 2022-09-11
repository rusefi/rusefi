/*
 * trigger_suzuki.cpp
 *
 * @date Oct 4, 2021
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"

#include "trigger_suzuki.h"

void initializeSuzukiG13B(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	float w = 5;
	float specialTooth = 20;

	s->addEvent720(180 - w, T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(180, T_PRIMARY, TriggerValue::FALL);

	s->addEvent720(2 * specialTooth + 180 - w, T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(2 * specialTooth + 180, T_PRIMARY, TriggerValue::FALL);

	s->addEvent720(360 - w, T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(360, T_PRIMARY, TriggerValue::FALL);

	s->addEvent720(540 - w, T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(540, T_PRIMARY, TriggerValue::FALL);

	s->addEvent720(720 - w, T_PRIMARY, TriggerValue::RISE);
	s->addEvent720(720, T_PRIMARY, TriggerValue::FALL);

	s->setTriggerSynchronizationGap(0.22);
	s->setSecondTriggerSynchronizationGap(1);
}
