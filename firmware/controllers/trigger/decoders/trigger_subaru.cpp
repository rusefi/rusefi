/**
 * @file	trigger_subaru.cpp
 *
 * @date Sep 10, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "trigger_subaru.h"

/**
 * This trigger is also used by Nissan and Mazda
 */
void initialize36_2_2_2(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR);

	float wide = 30 * 2;
	float narrow = 10 * 2;

	s->isSynchronizationNeeded = true;
	s->setTriggerSynchronizationGap(0.5);
	s->setSecondTriggerSynchronizationGap(1);

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

	float magic = 333;

	s->tdcPosition = 192;

	float width = 5;

	s->addEvent720(25 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(25, T_PRIMARY, TV_FALL);

	if (withCrankWheel) {
		s->addEvent720(magic - 180 - 87 - width, T_SECONDARY, TV_RISE);
		s->addEvent720(magic - 180 - 87, T_SECONDARY, TV_FALL);

		s->addEvent720(magic - 180 - 55 - width, T_SECONDARY, TV_RISE);
		s->addEvent720(magic - 180 - 55, T_SECONDARY, TV_FALL);

		s->addEvent720(magic - 180 - width, T_SECONDARY, TV_RISE);
		s->addEvent720(magic - 180, T_SECONDARY, TV_FALL);

	}
	s->addEvent720(182 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(182, T_PRIMARY, TV_FALL);

	if (withCrankWheel) {
		s->addEvent720(magic - 87 - width, T_SECONDARY, TV_RISE);
		s->addEvent720(magic - 87, T_SECONDARY, TV_FALL);
		s->addEvent720(magic - 55 - width, T_SECONDARY, TV_RISE);
		s->addEvent720(magic - 55, T_SECONDARY, TV_FALL);
		s->addEvent720(magic - width, T_SECONDARY, TV_RISE);
		s->addEvent720(magic, T_SECONDARY, TV_FALL);
	}

	s->addEvent720(343 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(343, T_PRIMARY, TV_FALL);

	s->addEvent720(366 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(366, T_PRIMARY, TV_FALL);

	s->addEvent720(384 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(384, T_PRIMARY, TV_FALL);

	if (withCrankWheel) {
		s->addEvent720(magic + 180 - 87 - width, T_SECONDARY, TV_RISE);
		s->addEvent720(magic + 180 - 87, T_SECONDARY, TV_FALL);
		s->addEvent720(magic + 180 - 55 - width, T_SECONDARY, TV_RISE);
		s->addEvent720(magic + 180 - 55, T_SECONDARY, TV_FALL);
		s->addEvent720(magic + 180 - width, T_SECONDARY, TV_RISE);
		s->addEvent720(magic + 180, T_SECONDARY, TV_FALL);
	}

	s->addEvent720(538 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(538, T_PRIMARY, TV_FALL);

	if (withCrankWheel) {
		s->addEvent720(magic + 360 - 87 - width, T_SECONDARY, TV_RISE);
		s->addEvent720(magic + 360 - 87, T_SECONDARY, TV_FALL);
		s->addEvent720(magic + 360 - 55 - width, T_SECONDARY, TV_RISE);
		s->addEvent720(magic + 360 - 55, T_SECONDARY, TV_FALL);
		s->addEvent720(magic + 360 - width, T_SECONDARY, TV_RISE);
		s->addEvent720(magic + 360, T_SECONDARY, TV_FALL);
	}

	s->addEvent720(720 - width, T_PRIMARY, TV_RISE);
	s->addEvent720(720, T_PRIMARY, TV_FALL);

	s->setTriggerSynchronizationGap2(4.9, 9);
	s->setTriggerSynchronizationGap3(1, 0.6, 1.25);

	s->isSynchronizationNeeded = true;

	s->useOnlyPrimaryForSync = true;

}

void initializeSubaruOnly7(TriggerWaveform *s) {
	initializeSubaru7_6(s, false);
}

void initializeSubaru7_6(TriggerWaveform *s) {
	initializeSubaru7_6(s, true);
}

void initializeSubaru_SVX(TriggerWaveform *s) {
	int n;
	/* crank 2 falling happens between crank #1 fallings */
	float crank_2_offset = 15.0;
	float cam_offset = 15.0;
	/* we should use only falling edges */
	float width = 5.0;

	/* additional 10 degrees should be removed!!! */
#define CRANK_1_FALL(n)		(20.0 + 10.0 + 30.0 * (n))
#define CRANK_1_RISE(n)		(CRANK_1_FALL(n) - width)

	/* T_CHANNEL_3 currently not supported, to keep trigger decode happy
	 * set cam second as secondary, so logic will be able to sync
	 * Crank angle sensor #1 = T_SECONDARY
	 * Crank andle sensor #2 = T_CHANNEL_3 - not supported yet
	 * Cam angle sensor = T_PRIMARY */
#define SVX_CRANK_1			T_SECONDARY
#define SVX_CRANK_2			T_CHANNEL_3
#define SVX_CAM				T_PRIMARY

	s->initialize(FOUR_STROKE_CAM_SENSOR);

	/******  0  *****/
	n = 0;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	n = 1;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* crank #2 - one 1/1 */
	s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
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
	/* crank #2 - three - 1/3 */
	s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	n = 6;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* crank #2 - three - 2/3 */
	s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	n = 7;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* +10 - TDC #6 */
	/* crank #2 - three - 3/3 */
	s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	n = 8;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	n = 9;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* crank #2 - two - 1/2 */
	s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	n = 10;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* crank #2 - two - 2/2 */
	s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
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
	/* crank #2 - one - 1/1 */
	s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
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
	/* crank #2 - three - 1/3 */
	s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	n = 18;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* crank #2 - three - 2/3 */
	s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	n = 19;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* +10 - TDC #5 */
	/* crank #2 - three - 3/3 */
	s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	n = 20;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	n = 21;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* crank #2 - two - 1/2 */
	s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	n = 22;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* crank #2 - two - 2/2 */
	s->addEvent720(CRANK_1_RISE(n) + crank_2_offset, SVX_CRANK_2, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n) + crank_2_offset, SVX_CRANK_2, TV_FALL);
	n = 23;
	s->addEvent720(CRANK_1_RISE(n), SVX_CRANK_1, TV_RISE);
	s->addEvent720(CRANK_1_FALL(n), SVX_CRANK_1, TV_FALL);
	/* +10 - TDC #4 */
	/****** 720 *****/

	s->setTriggerSynchronizationGap2(4.9, 9);
	s->setTriggerSynchronizationGap3(1, 0.6, 1.25);

	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;
}
