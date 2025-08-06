/**
 * @file	trigger_subaru.cpp
 *
 * @date Sep 10, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_subaru.h"

void initialize_one_of_36_2_2_2(TriggerWaveform *s, int firstCount, int secondCount) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

	float narrow = 360 / 36;
	float wide = narrow * 3;

	float base = 0;

	for (int i = 0; i < firstCount; i++) {
		s->addToothFallRise(base + narrow, narrow / 2);
		base += narrow;
	}

	s->addToothFallRise(base + wide, wide / 2);
	base += wide;

	for (int i = 0; i < secondCount; i++) {
		s->addToothFallRise(base + narrow, narrow / 2);
		base += narrow;
	}

  // depending on firstCount + secondCount we might have a gap here

  s->addToothFallRise(360 - wide, narrow / 2);
  s->addToothFallRise(360, narrow / 2);
}

/**
 * This trigger is also used by Nissan and Mazda
 * https://rusefi.com/forum/viewtopic.php?f=2&t=1932
 */
void initialize36_2_2_2(TriggerWaveform *s) {
	initialize_one_of_36_2_2_2(s, 12, 15);

#if EFI_UNIT_TEST
	// usually used on crank but placed on 'cam' on '2-stroke' rotary
	// this 'knownOperationMode' does not matter for trigger decoding only matters for .ini code generation and trigger images
	s->knownOperationMode = false;
#endif // EFI_UNIT_TEST

    // 36/2/2/2 data from https://rusefi.com/online/view.php?log=1287
    // todo: probably should be unified with EZ30 below?
	s->setTriggerSynchronizationGap3(/*gapIndex*/0, 0.25, 0.5);
	s->setTriggerSynchronizationGap3(/*gapIndex*/1, 0.7, 1.7);
	s->setTriggerSynchronizationGap3(/*gapIndex*/2, 2.25, 4.2);
}

void initializeSubaruEZ30(TriggerWaveform *s) {
	initialize_one_of_36_2_2_2(s, 18, 9);

    s->tdcPosition = 240;

	s->setTriggerSynchronizationGap3(/*gapIndex*/0, 0.25, 0.5);
	s->setTriggerSynchronizationGap3(/*gapIndex*/1, 0.7, 1.5);
	s->setTriggerSynchronizationGap3(/*gapIndex*/2, 2, 4);
}

static void initializeSubaru7_6(TriggerWaveform *s, bool withCrankWheel) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

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
		s->addEvent720((180 * (cycle)) + 63 - width, TriggerValue::RISE, TriggerWheel::T_SECONDARY);	\
		s->addEvent720((180 * (cycle)) + 63, TriggerValue::FALL, TriggerWheel::T_SECONDARY)
	/* 65 degrees BTDC, but we have 20 degrees shift:
	 * 180 - 65 - 20 = 95 */
	#define SUBARU76_CRANK_PULSE1(cycle) \
		s->addEvent720((180 * (cycle)) + 95 - width, TriggerValue::RISE, TriggerWheel::T_SECONDARY);	\
		s->addEvent720((180 * (cycle)) + 95, TriggerValue::FALL, TriggerWheel::T_SECONDARY)
	/* 10 degrees BTDC, but we have 20 degrees shift:
	 * 180 - 10 - 20 = 150 */
	#define SUBARU76_CRANK_PULSE2(cycle) \
		s->addEvent720((180 * (cycle)) + 150 - width, TriggerValue::RISE, TriggerWheel::T_SECONDARY);	\
		s->addEvent720((180 * (cycle)) + 150, TriggerValue::FALL, TriggerWheel::T_SECONDARY)

	#define SUBARU76_CAM_PULSE(cycle, offset) \
		s->addEvent720((180 * (cycle)) + (offset) - width, TriggerValue::RISE, TriggerWheel::T_PRIMARY);	\
		s->addEvent720((180 * (cycle)) + (offset), TriggerValue::FALL, TriggerWheel::T_PRIMARY)

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
	s->setTriggerSynchronizationGap3(/*index*/0, 6.53 * TRIGGER_GAP_DEVIATION_LOW, 15);
	s->setTriggerSynchronizationGap3(/*index*/1, 0.3, 1 * TRIGGER_GAP_DEVIATION_HIGH);
	s->setTriggerSynchronizationGap3(/*index*/2, 0.08, 0.3);

	s->useOnlyPrimaryForSync = withCrankWheel;
}

void initializeSubaru7_6(TriggerWaveform *s) {
	initializeSubaru7_6(s, true);
}

void initializeSubaru7_6_camOnly(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	const float width = 1;
	const float offset = 720 - (3 * 180 + 20);

	s->tdcPosition = 180 + offset;

	#define SUBARU76_CAMONLY_PULSE(cyl, subtooth) \
		s->addEventAngle(offset + (180 * (cyl)) + 20 + (15 * (subtooth)) - width, TriggerValue::RISE, TriggerWheel::T_PRIMARY);	\
		s->addEventAngle(offset + (180 * (cyl)) + 20 + (15 * (subtooth)), TriggerValue::FALL, TriggerWheel::T_PRIMARY)

	// CYL1
	// 5, 20, 35
	SUBARU76_CAMONLY_PULSE(0, -1);
	SUBARU76_CAMONLY_PULSE(0,  0);
	SUBARU76_CAMONLY_PULSE(0, +1);

	// CYL3
	// 200
	SUBARU76_CAMONLY_PULSE(1,  0);

	// CYL2
	// 380, 395
	SUBARU76_CAMONLY_PULSE(2,  0);
	SUBARU76_CAMONLY_PULSE(2, +1);

	// CYL4
	// 560
	SUBARU76_CAMONLY_PULSE(3,  0);

	// Tooths positions, deltas and gap ratios
	//  (5,  20,  35, 200, 380, 395, 560) 725...
	//      (15,  15, 165, 180,  15, 165, 165)  15...
	// K:  0.09,   1,  11,1.09,0.08,  11,   1)0.09

	// Gaps: 1, 0.09, 1, 11 at 560
	//s->setTriggerSynchronizationGap3(0,  0.50,  1.50);
	//s->setTriggerSynchronizationGap3(0,  0.04,  0.15);
	//s->setTriggerSynchronizationGap3(1,  0.50,  1.50);
	//s->setTriggerSynchronizationGap3(2,  7.00, 15.00);

	s->setTriggerSynchronizationGap3(/*index*/0, 6.53 * TRIGGER_GAP_DEVIATION_LOW, 15);
	s->setTriggerSynchronizationGap3(/*index*/1, 0.3, 1 * TRIGGER_GAP_DEVIATION_HIGH);
	s->setTriggerSynchronizationGap3(/*index*/2, 0.08, 0.3);
}

void initializeSubaruOnly7(TriggerWaveform *s) {
	//initializeSubaru7_6(s, false);
	initializeSubaru7_6_camOnly(s);
}

void initializeSubaru7_6_crankOnly(TriggerWaveform *s) {
	/**
	 * Note how we use 0..180 range while defining FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR trigger
	 * Note that only half of the physical wheel is defined here!
	 */
	s->initialize(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR, SyncEdge::RiseOnly);

	const float width = 1;
	const float offset = 10;

	s->tdcPosition = 65;

	// BTDC: 97, 65, 10
	// Distances: 93, 32, 55
	s->addEventAngle(180 + offset - 97 - width, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEventAngle(180 + offset - 97, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	s->addEventAngle(180 + offset - 65 - width, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEventAngle(180 + offset - 65, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	s->addEventAngle(180 + offset - 10 - width, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
	s->addEventAngle(180 + offset - 10, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	// Nominal gaps:
	// 55 / 32 = 1.7187
	//s->setTriggerSynchronizationGap2(0.85f, 3.43f);
	// 93 / 55 = 1.6909
	//s->setTriggerSynchronizationGap2(0.84f, 3.38);
	// 32 / 93 = 0.344
	s->setTriggerSynchronizationGap2(0.172f, 0.688f);
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
	/* we should use only falling edges */
	float width = 5.0;

	float offset = 10.0; /* to make last event @ 720 */

	/* T_CHANNEL_3 currently not supported, to keep trigger decode happy
	 * set cam second as primary, so logic will be able to sync
	 * Crank angle sensor #1 = TriggerWheel:: T_SECONDARY
	 * Crank andle sensor #2 = T_CHANNEL_3 - not supported yet
	 * Cam angle sensor = T_PRIMARY */
#define SVX_CRANK_1			TriggerWheel::T_SECONDARY
//#define SVX_CRANK_2			T_CHANNEL_3
#define SVX_CAM				TriggerWheel::T_PRIMARY

#define CRANK_1_FALL(n)		(20.0 + offset + 30.0 * (n))
#define CRANK_1_RISE(n)		(CRANK_1_FALL(n) - width)

#define SUBARU_SVX_CRANK1_PULSE(n) \
	s->addEventAngle(CRANK_1_RISE(n), TriggerValue::RISE, SVX_CRANK_1);	\
	s->addEventAngle(CRANK_1_FALL(n), TriggerValue::FALL, SVX_CRANK_1)

	/* cam falling edge offset from preceding Cr #1 falling edge */
	float cam_offset = (10.0 + 30.0 + 30.0 + 30.0) - 90.0;
#define SUBARU_SVX_CAM_PULSE(n) \
	s->addEvent720(CRANK_1_RISE(n) + cam_offset, TriggerValue::RISE, SVX_CAM);	\
	s->addEvent720(CRANK_1_FALL(n) + cam_offset, TriggerValue::FALL, SVX_CAM)

#ifdef SVX_CRANK_2
	/* Cr #2 signle tooth falling edge is (55 + 1) BTDC
	 * preceding Cr #1 falling edge is (10 + 30 + 30) BTDC */
	float crank_2_offset = (10.0 + 30.0 + 30.0) - (55.0 + 1.0);

	#define SUBARU_SVX_CRANK2_PULSE(n) \
		s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, TriggerValue::RISE, SVX_CRANK_2); \
		s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, TriggerValue::FALL, SVX_CRANK_2)
#else
	#define SUBARU_SVX_CRANK2_PULSE(n)	(void)(n)
#endif

		/* we should use only falling edges */
	// TODO: this trigger needs to be converted to SyncEdge::RiseOnly, so invert all rise/fall events!
	// see https://github.com/rusefi/rusefi/issues/4624
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Fall);
	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;

	/* counting Crank #1 tooths, should reach 23 at the end */
	n = 0;
	/******  0  *****/
	SUBARU_SVX_CRANK1_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
		/* crank #2 - one 1/1 */
		SUBARU_SVX_CRANK2_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
	/* +10 - TDC #1 */
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
			/* cam - one */
			SUBARU_SVX_CAM_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
		/* crank #2 - three - 1/3 */
		SUBARU_SVX_CRANK2_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
		/* crank #2 - three - 2/3 */
		SUBARU_SVX_CRANK2_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
	/* +10 - TDC #6 */
		/* crank #2 - three - 3/3 */
		SUBARU_SVX_CRANK2_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
		/* crank #2 - two - 1/2 */
		SUBARU_SVX_CRANK2_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
		/* crank #2 - two - 2/2 */
		SUBARU_SVX_CRANK2_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
	/* +10 - TDC #3 */

	/****** 360 *****/
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
		/* crank #2 - one - 1/1 */
		SUBARU_SVX_CRANK2_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
	/* +10 - TDC #2 */
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
		/* crank #2 - three - 1/3 */
		SUBARU_SVX_CRANK2_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
		/* crank #2 - three - 2/3 */
		SUBARU_SVX_CRANK2_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
	/* +10 - TDC #5 */
		/* crank #2 - three - 3/3 */
		SUBARU_SVX_CRANK2_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
		/* crank #2 - two - 1/2 */
		SUBARU_SVX_CRANK2_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
		/* crank #2 - two - 2/2 */
		SUBARU_SVX_CRANK2_PULSE(n);
	n++;
	SUBARU_SVX_CRANK1_PULSE(n);
	/* +10 - TDC #4 */
	/****** 720 *****/

	/* from sichronization point, which is Cam falling */
	s->tdcPosition = 720 - 30;

#undef SUBARU_SVX_CRANK1_PULSE
#undef SUBARU_SVX_CRANK2_PULSE
#undef SUBARU_SVX_CAM_PULSE
}
