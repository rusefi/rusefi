/*
 * @file trigger_vw.cpp
 *
 * @date Aug 25, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "trigger_vw.h"
#include "trigger_universal.h"

void setSkodaFavorit(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::Fall);

	s->addEvent360(0, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(46, TriggerValue::RISE, TriggerWheel::T_PRIMARY);

	s->addEvent360(180, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(223, TriggerValue::RISE, TriggerWheel::T_PRIMARY);

	s->addEvent360(226, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
	s->addEvent360(229, TriggerValue::RISE, TriggerWheel::T_PRIMARY);

	s->tdcPosition = 180 + 46;
	s->setTriggerSynchronizationGap3(/*gapIndex*/0, 0.80, 2);
}

void setVwConfiguration(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

	int totalTeethCount = 60;
	int skippedCount = 2;

	float engineCycle = FOUR_STROKE_ENGINE_CYCLE;
	float toothWidth = 0.5;

	addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, 60, 2, toothWidth, 0, engineCycle,
			NO_LEFT_FILTER, 690);

	float angleDown = engineCycle / totalTeethCount * (totalTeethCount - skippedCount - 1 + (1 - toothWidth) );
	s->addEventClamped(0 + angleDown + 12, TriggerValue::RISE, TriggerWheel::T_PRIMARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);
	s->addEventClamped(0 + engineCycle, TriggerValue::FALL, TriggerWheel::T_PRIMARY, NO_LEFT_FILTER, NO_RIGHT_FILTER);

	s->setTriggerSynchronizationGap2(1.6, 4);
	s->setSecondTriggerSynchronizationGap(1); // this gap is not required to synch on perfect signal but is needed to handle to reject cranking transition noise
	s->setThirdTriggerSynchronizationGap(1);
}
