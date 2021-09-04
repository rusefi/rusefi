/**
 * @file	trigger_subaru.cpp
 *
 * @date Sep 10, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "trigger_subaru.h"

/**
 * This trigger is also used by Nissan and Mazda
 * https://rusefi.com/forum/viewtopic.php?f=2&t=1932
 */
void initialize36_2_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	float wide = 30 * 2;
	float narrow = 10 * 2;

	s->setTriggerSynchronizationGap(0.6);
	s->setSecondTriggerSynchronizationGap(0.9);

	float base = 0;

	for (int i = 0; i < 12; i++) {
		s->addEvent720(base + narrow / 2, T_PRIMARY, TV_RISE);
		s->addEvent720(base + narrow, T_PRIMARY, TV_FALL);
		base += narrow;
	}

	s->addEvent720(base + wide / 2, T_PRIMARY, TV_RISE);
	s->addEvent720(base + wide, T_PRIMARY, TV_FALL);
	base += wide;

	for (int i = 0; i < 15; i++) {
		s->addEvent720(base + narrow / 2, T_PRIMARY, TV_RISE);
		s->addEvent720(base + narrow, T_PRIMARY, TV_FALL);
		base += narrow;
	}

	s->addEvent720(720 - wide - wide / 2, T_PRIMARY, TV_RISE);
	s->addEvent720(720 - wide, T_PRIMARY, TV_FALL);

	s->addEvent720(720 - wide / 2, T_PRIMARY, TV_RISE);
	s->addEvent720(720, T_PRIMARY, TV_FALL);
	s->useOnlyPrimaryForSync = true;
}

static void initializeSubaru7_6(TriggerWaveform *s, bool withCrankWheel) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	/* To make trigger decoder happy last event should be exactly at 720
	 * This code generates two trigger patterns: crank+cam (7+6) and
	 * cam only (7-6).
	 * So last event should be CAM event
	 * Crank: --------||-|---||-|-----||-|---||-|
	 * Cam:   -|-|-|------|------!-|------|------
	 * '!' pulse is selected as event at 720
	 * So next event is the first one on following description
	 * '!' pulse happens 20 degrees ATDC #2 (third in order)
	 * or 180 + 180 + 20. So we have */
	s->tdcPosition = 160 + 360;

	float width = 5;

	/* 97 degrees BTDC, but we have 20 degrees shift:
	 * 180 - 97 - 20 = 63 */
	#define SUBARU76_CRANK_PULSE0(cycle) \
		s->addEvent720((180 * (cycle)) + 63 - width, T_SECONDARY, TV_RISE);	\
		s->addEvent720((180 * (cycle)) + 63, T_SECONDARY, TV_FALL)
	/* 65 degrees BTDC, but we have 20 degrees shift:
	 * 180 - 65 - 20 = 95 */
	#define SUBARU76_CRANK_PULSE1(cycle) \
		s->addEvent720((180 * (cycle)) + 95 - width, T_SECONDARY, TV_RISE);	\
		s->addEvent720((180 * (cycle)) + 95, T_SECONDARY, TV_FALL)
	/* 10 degrees BTDC, but we have 20 degrees shift:
	 * 180 - 10 - 20 = 150 */
	#define SUBARU76_CRANK_PULSE2(cycle) \
		s->addEvent720((180 * (cycle)) + 150 - width, T_SECONDARY, TV_RISE);	\
		s->addEvent720((180 * (cycle)) + 150, T_SECONDARY, TV_FALL)

	#define SUBARU76_CAM_PULSE(cycle, offset) \
		s->addEvent720((180 * (cycle)) + (offset) - width, T_PRIMARY, TV_RISE);	\
		s->addEvent720((180 * (cycle)) + (offset), T_PRIMARY, TV_FALL)

	/* (TDC#2 + 20) + 15 */
	SUBARU76_CAM_PULSE(0, +15);

	if (withCrankWheel) {
		SUBARU76_CRANK_PULSE0(0);
		SUBARU76_CRANK_PULSE1(0);
		SUBARU76_CRANK_PULSE2(0);
	}

	/* (TDC#4 + 20) */
	SUBARU76_CAM_PULSE(1, 0);

	if (withCrankWheel) {
		SUBARU76_CRANK_PULSE0(1);
		SUBARU76_CRANK_PULSE1(1);
		SUBARU76_CRANK_PULSE2(1);
	}

	/* (TDC#1 + 20) - 15 */
	SUBARU76_CAM_PULSE(2, -15);

	/* (TDC#1 + 20) */
	SUBARU76_CAM_PULSE(2, 0);

	/* (TDC#1 + 20) + 15 */
	SUBARU76_CAM_PULSE(2, +15);

	if (withCrankWheel) {
		SUBARU76_CRANK_PULSE0(2);
		SUBARU76_CRANK_PULSE1(2);
		SUBARU76_CRANK_PULSE2(2);
	}

	/* (TDC#3 + 20) */
	SUBARU76_CAM_PULSE(3, 0);

	if (withCrankWheel) {
		SUBARU76_CRANK_PULSE0(3);
		SUBARU76_CRANK_PULSE1(3);
		SUBARU76_CRANK_PULSE2(3);
	}

	/* (TDC#2 + 20) */
	SUBARU76_CAM_PULSE(4, 0);

	// why is this trigger gap wider than average?
	s->setTriggerSynchronizationGap2(6.53 * TRIGGER_GAP_DEVIATION_LOW, 10.4 * TRIGGER_GAP_DEVIATION_HIGH);
	s->setTriggerSynchronizationGap3(1, 0.8 * TRIGGER_GAP_DEVIATION_LOW, 1 * TRIGGER_GAP_DEVIATION_HIGH);

	s->useOnlyPrimaryForSync = true;
}

void initializeSubaruOnly7(TriggerWaveform *s) {
	initializeSubaru7_6(s, false);
}

void initializeSubaru7_6(TriggerWaveform *s) {
	initializeSubaru7_6(s, true);
}

/*
 * Falling edges showed only:
 *              6       3       2       5       4       1
 * Cr #1 |-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
 * Cr #2 ---|-|-|---|-|-----|-------|-|-|---|-|-----|-------
 * Cam   -|-------------------------------------------------
 *
 * Cr #1 last falling edge BTDC: 10
 * Cr #2 single tooth's falling edge BTDC #1, #2: (55 + 1)
 * There is no details about gap betweent Cr #2 tooths in 2 and 3 groups.
 * Looking at timing diagram it is same as for Cr #1 = 30 degrees.
 * So:
 * Cr #2 two tooth group BTDC #3, #4: (55 + 1), (55 + 1 - 30)
 * Cr #2 three tooth group BTDC #5, #6: (55 + 1), (55 + 1 - 30), (55 + 1 - 60) - last event actually after DTC
 * Again there is no details about Cam tooth position, looking at
 * diagrams it is about 30 degrees after #1 TDC
 * Cam single tooth falling edge BTDC #6: (120 - 30) = 90
 */

void initializeSubaru_SVX(TriggerWaveform *s) {
	int n;
	float cam_offset = 15.0;
	/* we should use only falling edges */
	float width = 5.0;

	/* additional 10 degrees should be removed!!! */
#define CRANK_1_FALL(n)		(20.0 + 10.0 + 30.0 * (n))
#define CRANK_1_RISE(n)		(CRANK_1_FALL(n) - width)

	/* T_CHANNEL_3 currently not supported, to keep trigger decode happy
	 * set cam second as primary, so logic will be able to sync
	 * Crank angle sensor #1 = T_SECONDARY
	 * Crank andle sensor #2 = T_CHANNEL_3 - not supported yet
	 * Cam angle sensor = T_PRIMARY */
#define SVX_CRANK_1			T_SECONDARY
//#define SVX_CRANK_2			T_CHANNEL_3
#define SVX_CAM				T_PRIMARY

#ifdef SVX_CRANK_2
	/* crank 2 falling happens between crank #1 fallings */
	float crank_2_offset = 15.0;
#endif

	s->initialize(FOUR_STROKE_CAM_SENSOR);

	/******  0  *****/
	n = 0;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	n = 1;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	#ifdef SVX_CRANK_2
		/* crank #2 - one 1/1 */
		s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
		s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	#endif
	n = 2;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	n = 3;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* +10 - TDC #1 */
	n = 4;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
			/* cam - one */
			s->addEvent720(CRANK_1_RISE(n) + cam_offset, SVX_CAM, TV_RISE);
			s->addEvent720(CRANK_1_FALL(n) + cam_offset, SVX_CAM, TV_FALL);
	n = 5;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	#ifdef SVX_CRANK_2
		/* crank #2 - three - 1/3 */
		s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
		s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	#endif
	n = 6;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	#ifdef SVX_CRANK_2
		/* crank #2 - three - 2/3 */
		s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
		s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	#endif
	n = 7;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* +10 - TDC #6 */
	#ifdef SVX_CRANK_2
		/* crank #2 - three - 3/3 */
		s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
		s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	#endif
	n = 8;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	n = 9;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	#ifdef SVX_CRANK_2
		/* crank #2 - two - 1/2 */
		s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
		s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	#endif
	n = 10;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	#ifdef SVX_CRANK_2
		/* crank #2 - two - 2/2 */
		s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
		s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	#endif
	n = 11;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* +10 - TDC #3 */

	/****** 360 *****/
	n = 12;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	n = 13;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	#ifdef SVX_CRANK_2
		/* crank #2 - one - 1/1 */
		s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
		s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	#endif
	n = 14;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	n = 15;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* +10 - TDC #2 */
	n = 16;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	n = 17;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	#ifdef SVX_CRANK_2
		/* crank #2 - three - 1/3 */
		s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
		s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	#endif
	n = 18;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	#ifdef SVX_CRANK_2
		/* crank #2 - three - 2/3 */
		s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
		s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	#endif
	n = 19;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* +10 - TDC #5 */
	#ifdef SVX_CRANK_2
		/* crank #2 - three - 3/3 */
		s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
		s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	#endif
	n = 20;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	n = 21;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	#ifdef SVX_CRANK_2
		/* crank #2 - two - 1/2 */
		s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
		s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	#endif
	n = 22;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	#ifdef SVX_CRANK_2
		/* crank #2 - two - 2/2 */
		s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
		s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	#endif
	n = 23;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* +10 - TDC #4 */
	/****** 720 *****/

	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;

	s->tdcPosition = -60;
}
