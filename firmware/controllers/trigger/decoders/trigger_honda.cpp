/*
 * @file	trigger_honda.cpp
 *
 * @date May 27, 2016
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "trigger_honda.h"
#include "trigger_universal.h"

#define S24 (720.0f / 24 / 2)

static float addAccordPair(TriggerShape *s, float sb, trigger_wheel_e const channelIndex DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->addEvent720(sb, channelIndex, TV_RISE);
	sb += S24;
	s->addEvent720(sb, channelIndex, TV_FALL);
	sb += S24;

	return sb;
}

#define DIP 7.5f
static float addAccordPair3(TriggerShape *s, float sb DECLARE_ENGINE_PARAMETER_SUFFIX) {
	sb += DIP;
	s->addEvent720(sb, T_CHANNEL_3, TV_RISE);
	sb += DIP;
	s->addEvent720(sb, T_CHANNEL_3, TV_FALL);
	sb += 2 * DIP;
	return sb;
}

/**
 * Thank you Dip!
 * http://forum.pgmfi.org/viewtopic.php?f=2&t=15570start=210#p139007
 */
void configureHondaAccordCDDip(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	s->initialState[T_SECONDARY] = TV_RISE;
	float sb = 0;
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent720(90, T_SECONDARY, TV_FALL);
	sb = 90;
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent720(180, T_SECONDARY, TV_RISE);
	sb = 180;
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent720(270, T_SECONDARY, TV_FALL);
	sb = 270;
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);


	s->addEvent720(360.0f - DIP, T_PRIMARY, TV_RISE);
	s->addEvent720(360, T_SECONDARY, TV_RISE);
	sb = 360;
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent720(450, T_SECONDARY, TV_FALL);
	sb = 450;
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent720(540, T_SECONDARY, TV_RISE);
	sb = 540;
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent720(630, T_SECONDARY, TV_FALL);
	sb = 630;
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair3(s, sb PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent720(720.0f - DIP, T_PRIMARY, TV_FALL);

//	s->addEvent720(720.0f - 12 * sb, T_SECONDARY, TV_FALL);
//	s->addEvent720(720.0f, T_SECONDARY, TV_FALL);

	s->addEvent720(720.0f, T_SECONDARY, TV_RISE);

	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;
}

/**
 * '1' is conditional
 * '4' is conditional
 * '24' is always secondary channel
 */
void configureHonda_1_4_24(TriggerShape *s, bool withOneEventSignal, bool withFourEventSignal,
		trigger_wheel_e const oneEventWave,
		trigger_wheel_e const fourEventWave,
		float prefix DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);


	float sb = 5.0f + prefix;

	float tdcWidth = 0.1854 * 720 / 4;

	s->isSynchronizationNeeded = false;

	sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);

	if (withOneEventSignal)
		s->addEvent720(sb - S24 / 2, oneEventWave, TV_RISE);

	sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
	if (withOneEventSignal)
		s->addEvent720(sb - S24 / 2, oneEventWave, TV_FALL);
	sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
	if (withFourEventSignal) {
		s->addEvent720(1 * 180.0f + prefix - tdcWidth, fourEventWave, TV_RISE);
	}
	sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
	if (withFourEventSignal) {
		s->addEvent720(1 * 180.0f + prefix, fourEventWave, TV_FALL);
	}

	sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair(s, sb,T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
	sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);

	if (withFourEventSignal) {
		s->addEvent720(2 * 180.0f + prefix - tdcWidth, fourEventWave, TV_RISE);
	}
	sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
	if (withFourEventSignal) {
		s->addEvent720(2 * 180.0f + prefix, fourEventWave, TV_FALL);
	}

	for (int i = 3; i <= 4; i++) {
		sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
		sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
		sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
		sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
		sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);

		if (withFourEventSignal) {
			s->addEvent720(i * 180.0f + prefix - tdcWidth, fourEventWave, TV_RISE);
		}
		sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
		if (withFourEventSignal) {
			s->addEvent720(i * 180.0f + prefix, fourEventWave, TV_FALL);
		}
	}
	s->useOnlyPrimaryForSync = true;
}

void configureHondaCbr600(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX) {
	// todo: finish this
	setToothedWheelConfiguration(s, 24, 0, FOUR_STROKE_CRANK_SENSOR PASS_ENGINE_PARAMETER_SUFFIX);

}

void configureHondaCbr600custom(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX) {

	float w = 720 / 2 / 24;
//	s->initialize(FOUR_STROKE_CAM_SENSOR, false);
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	s->useOnlyPrimaryForSync = true;
	s->isSynchronizationNeeded = true;
	s->setTriggerSynchronizationGap2(0.7, 1.1);


	float a = 0;

	a += w;
	s->addEvent720(a, T_SECONDARY, TV_RISE);
	a += w;
	s->addEvent720(a - 1, T_SECONDARY, TV_FALL); // 30

	a += w;
	s->addEvent720(a, T_SECONDARY, TV_RISE);
	s->addEvent720(52.4, T_PRIMARY, TV_FALL);
	a += w;
	s->addEvent720(a - 1, T_SECONDARY, TV_FALL); // 60

	for (int i = 0;i<10;i++) {
		a += w;
		s->addEvent720(a, T_SECONDARY, TV_RISE);
		a += w;
		s->addEvent720(a, T_SECONDARY, TV_FALL);
	}

	a += w;
	s->addEvent720(a, T_SECONDARY, TV_RISE);

	s->addEvent720(381.34f, T_PRIMARY, TV_RISE);

	a += w;
	s->addEvent720(a - 1, T_SECONDARY, TV_FALL);

	for (int i = 0;i<1;i++) {
		a += w;
		s->addEvent720(a, T_SECONDARY, TV_RISE);
		a += w;
		s->addEvent720(a, T_SECONDARY, TV_FALL);
	}

	a += w;
	s->addEvent720(a, T_SECONDARY, TV_RISE);


	s->addEvent720(449.1f, T_PRIMARY, TV_FALL);

	a += w;
	s->addEvent720(a, T_SECONDARY, TV_FALL);


	for (int i = 0;i<8;i++) {
		a += w;
		s->addEvent720(a, T_SECONDARY, TV_RISE);
		a += w;
		s->addEvent720(a, T_SECONDARY, TV_FALL);
	}

	a += w;
	s->addEvent720(a, T_SECONDARY, TV_RISE);
	a += w;
	s->addEvent720(a - 1, T_SECONDARY, TV_FALL);


	s->addEvent720(720.0f, T_PRIMARY, TV_RISE);

}

void configureHondaAccordShifted(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX) {
	float w = 720 / 2 / 24;
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	float sb = S24;

	// like this there is no issue
//	s->addEvent720(S24 + 0.001, T_PRIMARY, TV_RISE);
//	s->addEvent720(S24 + 0.1, T_SECONDARY, TV_RISE);

	s->addEvent720(S24 + 0.001, T_SECONDARY, TV_RISE);
	s->addEvent720(S24 + 0.1, T_PRIMARY, TV_RISE);



	sb += S24;
	s->addEvent720(sb, T_SECONDARY, TV_FALL);
	sb += S24;

	s->addEvent720(S24 + 22, T_PRIMARY, TV_FALL);


	for (int i = 0;i<23;i++) {
		sb = addAccordPair(s, sb, T_SECONDARY PASS_ENGINE_PARAMETER_SUFFIX);
	}



	s->useOnlyPrimaryForSync = true;
	s->isSynchronizationNeeded = false;
}

void configureOnePlus16(TriggerShape *s, operation_mode_e operationMode DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	int totalTeethCount = 16;
	int skippedCount = 0;

	s->addEvent720(2, T_PRIMARY, TV_RISE);
	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 0, 360, 2, 20 PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent720(20, T_PRIMARY, TV_FALL);
	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 0, 360, 20, NO_RIGHT_FILTER PASS_ENGINE_PARAMETER_SUFFIX);

	addSkippedToothTriggerEvents(T_SECONDARY, s, totalTeethCount, skippedCount, 0.5, 360, 360, NO_LEFT_FILTER,
	NO_RIGHT_FILTER PASS_ENGINE_PARAMETER_SUFFIX);

	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;
}


