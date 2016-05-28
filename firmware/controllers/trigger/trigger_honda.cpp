/*
 * @file	trigger_honda.cpp
 *
 * @date May 27, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "trigger_honda.h"

#define S24 (720.0f / 24 / 2)

static float addAccordPair(TriggerShape *s, float sb, trigger_wheel_e const waveIndex) {
	s->addEvent(sb, waveIndex, TV_RISE);
	sb += S24;
	s->addEvent(sb, waveIndex, TV_FALL);
	sb += S24;

	return sb;
}

#define DIP 7.5f
static float addAccordPair3(TriggerShape *s, float sb) {
	sb += DIP;
	s->addEvent(sb, T_CHANNEL_3, TV_RISE);
	sb += DIP;
	s->addEvent(sb, T_CHANNEL_3, TV_FALL);
	sb += 2 * DIP;
	return sb;
}

/**
 * Thank you Dip!
 * http://forum.pgmfi.org/viewtopic.php?f=2&t=15570start=210#p139007
 */
void configureHondaAccordCDDip(TriggerShape *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	s->initialState[T_SECONDARY] = TV_RISE;
	float sb = 0;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(90, T_SECONDARY, TV_FALL);
	sb = 90;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(180, T_SECONDARY, TV_RISE);
	sb = 180;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(270, T_SECONDARY, TV_FALL);
	sb = 270;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);


	s->addEvent(360.0f - DIP, T_PRIMARY, TV_RISE);
	s->addEvent(360, T_SECONDARY, TV_RISE);
	sb = 360;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(450, T_SECONDARY, TV_FALL);
	sb = 450;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(540, T_SECONDARY, TV_RISE);
	sb = 540;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(630, T_SECONDARY, TV_FALL);
	sb = 630;
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);
	sb = addAccordPair3(s, sb);

	s->addEvent(720.0f - DIP, T_PRIMARY, TV_FALL);

//	s->addEvent(720.0f - 12 * sb, T_SECONDARY, TV_FALL);
//	s->addEvent(720.0f, T_SECONDARY, TV_FALL);

	s->addEvent(720.0f, T_SECONDARY, TV_RISE);

	s->isSynchronizationNeeded = false;
}

void configureHondaAccordCD(TriggerShape *s, bool withOneEventSignal, bool withFourEventSignal,
		trigger_wheel_e const oneEventWave,
		trigger_wheel_e const fourEventWave,
		float prefix) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

//	trigger_wheel_e const oneEventWave = T_CHANNEL_3;
//	bool withFourEventSignal = true;
//	trigger_wheel_e const fourEventWave = T_PRIMARY;

	float sb = 5.0f + prefix;

	float tdcWidth = 0.1854 * 720 / 4;

	s->isSynchronizationNeeded = false;

	sb = addAccordPair(s, sb, T_SECONDARY);

	if (withOneEventSignal)
		s->addEvent(sb - S24 / 2, oneEventWave, TV_RISE);

	sb = addAccordPair(s, sb, T_SECONDARY);
	sb = addAccordPair(s, sb, T_SECONDARY);
	if (withOneEventSignal)
		s->addEvent(sb - S24 / 2, oneEventWave, TV_FALL);
	sb = addAccordPair(s, sb, T_SECONDARY);
	sb = addAccordPair(s, sb, T_SECONDARY);
	if (withFourEventSignal) {
		s->addEvent(1 * 180.0f + prefix - tdcWidth, fourEventWave, TV_RISE);
	}
	sb = addAccordPair(s, sb, T_SECONDARY);
	if (withFourEventSignal) {
		s->addEvent(1 * 180.0f + prefix, fourEventWave, TV_FALL);
	}

	sb = addAccordPair(s, sb, T_SECONDARY);
	sb = addAccordPair(s, sb,T_SECONDARY);
	sb = addAccordPair(s, sb, T_SECONDARY);
	sb = addAccordPair(s, sb, T_SECONDARY);
	sb = addAccordPair(s, sb, T_SECONDARY);

	if (withFourEventSignal) {
		s->addEvent(2 * 180.0f + prefix - tdcWidth, fourEventWave, TV_RISE);
	}
	sb = addAccordPair(s, sb, T_SECONDARY);
	if (withFourEventSignal) {
		s->addEvent(2 * 180.0f + prefix, fourEventWave, TV_FALL);
	}

	for (int i = 3; i <= 4; i++) {
		sb = addAccordPair(s, sb, T_SECONDARY);
		sb = addAccordPair(s, sb, T_SECONDARY);
		sb = addAccordPair(s, sb, T_SECONDARY);
		sb = addAccordPair(s, sb, T_SECONDARY);
		sb = addAccordPair(s, sb, T_SECONDARY);

		if (withFourEventSignal) {
			s->addEvent(i * 180.0f + prefix - tdcWidth, fourEventWave, TV_RISE);
		}
		sb = addAccordPair(s, sb, T_SECONDARY);
		if (withFourEventSignal) {
			s->addEvent(i * 180.0f + prefix, fourEventWave, TV_FALL);
		}
	}
}

void configureHondaCbr600(TriggerShape *s DECLARE_ENGINE_PARAMETER_S) {
	// todo: finish this
	setToothedWheelConfiguration(s, 24, 0, FOUR_STROKE_CRANK_SENSOR);

}


