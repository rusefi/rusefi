/**
 * @file	trigger_gm.cpp
 *
 * @date Mar 28, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "trigger_gm.h"

void configureGmTriggerShape(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, false);

	// all angles are x2 here - so, 5 degree width is 10
	float w = 10;

	float m = CRANK_MODE_MULTIPLIER;

	s->addEvent2(m * 60 - w, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(m * 60, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(m * 120 - w, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(m * 120.0, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(m * 180 - w, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(m * 180, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(m * 240 - w, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(m * 240.0, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(m * 300 - w, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(m * 300.0, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(m * 350 - w, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(m * 350.0, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(m * 360 - w, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(m * 360.0, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	s->setTriggerSynchronizationGap(6);
}

static int gm_tooth_pair(float startAngle, bool isLongShort, TriggerShape* s, int mult DECLARE_ENGINE_PARAMETER_SUFFIX)
{
	int window = (isLongShort ? 12 : 3) * mult;
	int end = startAngle + mult * 15;

	s->addEvent2(startAngle + window, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(end, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	return end;
}

/**
 * TT_GM_LS_24
 * https://www.mediafire.com/?40mfgeoe4ctti
 * http://www.ls1gto.com/forums/archive/index.php/t-190549.htm
 * http://www.ls2.com/forums/showthread.php/834483-LS-Timing-Reluctor-Wheels-Explained
 *
 *
 * based on data in https://rusefi.com/forum/viewtopic.php?f=3&t=936&p=30303#p30285
 */
void initGmLS24(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, false);
	trigger_wheel_e ch = T_PRIMARY;

	/* 
	 * Okay, here's how this magic works:
	 * The GM 24x crank wheel has 48 edges.  There is
	 * a falling edge every 15 degrees (1/24 revolution).
	 * After every falling edge, a rising edge occurs either
	 * 3 or 12 (= 15 - 3) degrees later.  The code 0x0A33BE
	 * encodes the pattern of which type of gap occurs in the
	 * pattern.  Starting from the LSB, each bit left is the 
	 * next gap in sequence as the crank turns.  A 0 indicates
	 * long-short, while a 1 indicates short-long.
	 * 
	 * The first few bits read are 0xE (LSB first!) = 0 - 1 - 1 - 1, so the pattern
	 * looks like this:
	 * ___     _   ___   ___   ___
	 *    |___| |_|   |_|   |_|   |_ etc
	 * 
	 *    |  0  |  1  |  1  |  1  |
	 * 
	 *     ___ = 12 degrees, _ = 3 deg
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

		angle = gm_tooth_pair(angle, bit, s, CRANK_MODE_MULTIPLIER PASS_ENGINE_PARAMETER_SUFFIX);
	}

	s->useOnlyPrimaryForSync = true;

	s->setTriggerSynchronizationGap(1.6);
	s->setSecondTriggerSynchronizationGap(1);
	s->setThirdTriggerSynchronizationGap(2.5);
}

