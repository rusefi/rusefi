/**
 * @file	trigger_gm.cpp
 *
 * @date Mar 28, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_gm.h"

#define GM_60_W 6

static float addTooth(float offset, TriggerWaveform *s) {
	s->addToothRiseFall(offset + GM_60_W / 2, GM_60_W / 2, TriggerWheel::T_SECONDARY);
	return offset + GM_60_W;
}

/**
 * https://github.com/rusefi/rusefi/issues/2264
 * GM/Daewoo Distributor on the F8CV
 */
void configureGm60_2_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	s->isSynchronizationNeeded = false;
	s->isSecondWheelCam = true;

	int offset = 1;

	for (int i=0;i<12;i++) {
		offset = addTooth(offset, s);
	}

	offset += 2 * GM_60_W;

	for (int i=0;i<18;i++) {
		offset = addTooth(offset, s);
	}

	offset += 2 * GM_60_W;

	for (int i=0;i<18;i++) {
		offset = addTooth(offset, s);
	}

	offset += 2 * GM_60_W;

	for (int i=0;i<5;i++) {
		offset = addTooth(offset, s);
	}

	s->addEvent360(360 - GM_60_W, TriggerValue::RISE);

	offset = addTooth(offset, s);

	s->addEvent360(360, TriggerValue::FALL);

}

void configureGmTriggerWaveform(TriggerWaveform *s) {
	static const angle_t angles[] = { 60, 120, 180, 240, 300, 350, 360 };
	initializeRiseOnlyTrigger(s, 5, angles, efi::size(angles), FOUR_STROKE_CRANK_SENSOR);

	s->setTriggerSynchronizationGap(6);
}

static int gm_tooth_pair(float startAngle, bool isShortLong, TriggerWaveform* s, float shortToothWidth)
{
	int window = (isShortLong ? shortToothWidth : (15 - shortToothWidth));
	int end = startAngle + 15;

	s->addEvent360(startAngle + window, TriggerValue::RISE);
	s->addEvent360(end, TriggerValue::FALL);

	return end;
}

/**
 * TT_GM_24x_5 and TT_GM_24x_3
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

		angle = gm_tooth_pair(angle, bit, s, shortToothWidth);
	}
}

// TT_GM_24x_5
void initGmLS24_5deg(TriggerWaveform *s) {
	initGmLS24(s, 5);

	// This is tooth #20, at 310 degrees ATDC #1
	s->setTriggerSynchronizationGap(2.0f);
	s->setSecondTriggerSynchronizationGap(0.5f);
	s->setThirdTriggerSynchronizationGap(2.0f);

	s->tdcPosition = 50;
}

// TT_GM_24x_3
void initGmLS24_3deg(TriggerWaveform *s) {
	initGmLS24(s, 3);

	// This is tooth #20, at 312 degrees ATDC #1
	s->setTriggerSynchronizationGap(4.0f);
	s->setSecondTriggerSynchronizationGap(0.25f);
	s->setThirdTriggerSynchronizationGap(4.0f);

	s->tdcPosition = 48;
}

/**
 * GM Gen II LT1 Optispark low-resolution track only. The 360-slot track is unused.
 * Geometry: speeduino/Ardu-Stim, wheel_defs.h, optispark_lt1 (720 samples/cycle).
 * https://github.com/speeduino/Ardu-Stim/blob/51f624a9ae369035211952862befe099c2b93a58/ardustim/ardustim/wheel_defs.h
 * The eight rising edges are 90 crank degrees apart. Pulse widths alternate
 * 14, 4, 24, 4, 34, 4, 44, 4 crank degrees, identifying a complete 720-degree cycle.
 * This reference waveform does not establish cylinder #1 TDC: calibrate the
 * trigger offset on the engine. See docs/triggers/gm-lt1-optispark-8.md.
 */
void initializeGmLt1Optispark8(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Both);

	static constexpr angle_t widths[] = {14, 4, 24, 4, 34, 4, 44, 4};
	for (size_t i = 0; i < efi::size(widths); i++) {
		const angle_t rise = 86 + 90 * i;
		s->addEvent720(rise, TriggerValue::RISE);
		s->addEvent720(rise + widths[i], TriggerValue::FALL);
	}

	// Sync at the end of the 14-degree pulse (100 degrees in this waveform).
	// Also check 86/4 and 4/46. The third ratio rejects reverse rotation,
	// whose 14/76 and 76/4 ratios would otherwise fit the first two windows.
	s->setTriggerSynchronizationGap2(0.12f, 0.21f);
	s->setSecondTriggerSynchronizationGap2(16.0f, 27.0f);
	s->setTriggerSynchronizationGap3(2, 0.065f, 0.11f);
}
