/**
 * @file	trigger_gm.cpp
 *
 * @date Mar 28, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_gm.h"

static float addTooth(float offset, TriggerWaveform *s) {
	s->addEventAngle(offset, true, TriggerWheel::T_SECONDARY);
	offset += CRANK_MODE_MULTIPLIER * 3;
	s->addEventAngle(offset, false, TriggerWheel::T_SECONDARY);
	offset += CRANK_MODE_MULTIPLIER * 3;
	return offset;
}

/**
 * https://github.com/rusefi/rusefi/issues/2264
 * GM/Daewoo Distributor on the F8CV
 */
void configureGm60_2_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	s->isSynchronizationNeeded = false;
	s->isSecondWheelCam = true;

	float m = CRANK_MODE_MULTIPLIER;
	int offset = 1 * m;

	for (int i = 0; i < 12; i++) {
		offset = addTooth(offset, s);
	}

	offset += m * 2 * 6;

	for (int i = 0; i < 18; i++) {
		offset = addTooth(offset, s);
	}

	offset += m * 2 * 6;

	for (int i = 0; i < 18; i++) {
		offset = addTooth(offset, s);
	}

	offset += m * 2 * 6;

	for (int i = 0; i < 5; i++) {
		offset = addTooth(offset, s);
	}


	s->addEventAngle(m * (360 - 6), true);

	offset = addTooth(offset, s);

	s->addEventAngle(m * (360), false);

}

void configureGmTriggerWaveform(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

	float w = 5;

	s->addEvent360(60 - w, true);
	s->addEvent360(60, false);

	s->addEvent360(120 - w, true);
	s->addEvent360(120.0, false);

	s->addEvent360(180 - w, true);
	s->addEvent360(180, false);

	s->addEvent360(240 - w, true);
	s->addEvent360(240.0, false);

	s->addEvent360(300 - w, true);
	s->addEvent360(300.0, false);

	s->addEvent360(350 - w, true);
	s->addEvent360(350.0, false);

	s->addEvent360(360 - w, true);
	s->addEvent360(360.0, false);

	s->setTriggerSynchronizationGap(6);
}

static int gm_tooth_pair(float startAngle, bool isShortLong, TriggerWaveform* s, int mult, float shortToothWidth)
{
	int window = (isShortLong ? shortToothWidth : (15 - shortToothWidth)) * mult;
	int end = startAngle + mult * 15;

	s->addEvent720(startAngle + window, true);
	s->addEvent720(end, false);

	return end;
}

/**
 * trigger_type_e::TT_GM_24x and trigger_type_e::TT_GM_24x_2
 * https://www.mediafire.com/?40mfgeoe4ctti
 * http://www.ls1gto.com/forums/archive/index.php/t-190549.htm
 * http://www.ls2.com/forums/showthread.php/834483-LS-Timing-Reluctor-Wheels-Explained
 *
 *
 * based on data in https://rusefi.com/forum/viewtopic.php?f=3&t=936&p=30303#p30285
 */
static void initGmLS24(TriggerWaveform *s, float shortToothWidth) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::Rise);

	/* 
	 * Okay, here's how this magic works:
	 * The GM 24x crank wheel has 48 edges.  There is
	 * a falling edge every 15 degrees (1/24 revolution).
	 * After every falling edge, a rising edge occurs either
	 * 5 or 10 (= 15 - 5) degrees later.  The code 0x0A33BE
	 * encodes the pattern of which type of gap occurs in the
	 * pattern.  Starting from the LSB, each bit left is the 
	 * next gap in sequence as the crank turns.  A 0 indicates
	 * long-short (late rising edge), while a 1 indicates
	 * short-long (early rising edge).
	 * 
	 * The first few bits read are 0xE (LSB first!) = 0 - 1 - 1 - 1, so the pattern
	 * looks like this:
	 * ___     _   ___   ___     _
	 *    |___| |_|   |_|   |___| |_ etc
	 * 
	 *    |  0  |  1  |  1  |  0  |
	 * 
	 *     ___ = 10 degrees, _ = 5 deg
	 * 
	 * There is a falling edge at angle=0=720, and this is position
	 * is #1 (and #6) TDC.  If there's a falling edge on the cam
	 * sensor, it's #1 end compression stroke (fire this plug!)
	 * and #6 end exhaust stroke.  If rising, it's exhaust #1,
	 * compression #6.
	 */

	uint32_t code = 0x0A33BE;

	int angle = 0;
	
	for(int i = 0; i < 24; i++)
	{
		bool bit = code & 0x000001;
		code = code >> 1;

		angle = gm_tooth_pair(angle, bit, s, CRANK_MODE_MULTIPLIER, shortToothWidth);
	}
}

// trigger_type_e::TT_GM_24x
void initGmLS24_5deg(TriggerWaveform *s) {
	initGmLS24(s, 5);

	// This is tooth #20, at 310 degrees ATDC #1
	s->setTriggerSynchronizationGap(2.0f);
	s->setSecondTriggerSynchronizationGap(0.5f);
	s->setThirdTriggerSynchronizationGap(2.0f);

	s->tdcPosition = 50;
}

// trigger_type_e::TT_GM_24x_2
void initGmLS24_3deg(TriggerWaveform *s) {
	initGmLS24(s, 3);

	// This is tooth #20, at 312 degrees ATDC #1
	s->setTriggerSynchronizationGap(4.0f);
	s->setSecondTriggerSynchronizationGap(0.25f);
	s->setThirdTriggerSynchronizationGap(4.0f);

	s->tdcPosition = 48;
}
