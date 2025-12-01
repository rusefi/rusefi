/*
 * trigger_misc.cpp
 *
 *  Created on: Oct 30, 2018
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_misc.h"
#include "trigger_universal.h"

// TT_FIAT_IAW_P8
void configureFiatIAQ_P8(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Rise);

	int width = 60;
	s->tdcPosition = width;

	s->addEvent720(width, TriggerValue::RISE);
	s->addEvent720(180, TriggerValue::FALL);

	s->addEvent720(180 + width, TriggerValue::RISE);
	s->addEvent720(720, TriggerValue::FALL);
	s->setTriggerSynchronizationGap(3);
}

/**
 * based on https://fordsix.com/threads/understanding-standard-and-signature-pip-thick-film-ignition.81515/
 * based on https://www.w8ji.com/distributor_stabbing.htm
 */
void configureFordPip(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Rise);

	s->tdcPosition = 662.5;

	s->setTriggerSynchronizationGap(0.66);
	s->setSecondTriggerSynchronizationGap(1.25);
	/**
	 * sensor is mounted on distributor but trigger shape is defined in engine cycle angles
	 */
	int oneCylinder = s->getCycleDuration() / 8;

	s->addEventAngle(oneCylinder * 0.75, TriggerValue::RISE);
	s->addEventAngle(oneCylinder, TriggerValue::FALL);


	for (int i = 2;i<=8;i++) {
		s->addEventAngle(oneCylinder * (i - 0.5), TriggerValue::RISE);
		s->addEventAngle(oneCylinder * i, TriggerValue::FALL);
	}

}

void configureFordST170(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	int width = 10;

	int total = s->getCycleDuration() / 8;

	s->addEventAngle(1 * total - width, TriggerValue::RISE);
	s->addEventAngle(1 * total, TriggerValue::FALL);

	s->addEventAngle(2 * total - width, TriggerValue::RISE);
	s->addEventAngle(2 * total, TriggerValue::FALL);

	s->addEventAngle(4 * total - width, TriggerValue::RISE);
	s->addEventAngle(4 * total, TriggerValue::FALL);

	s->addEventAngle(6 * total - width, TriggerValue::RISE);
	s->addEventAngle(6 * total, TriggerValue::FALL);

	s->addEventAngle(8 * total - width, TriggerValue::RISE);
	s->addEventAngle(8 * total, TriggerValue::FALL);
}

static void daihatsu(TriggerWaveform * s, int count) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	s->tdcPosition = 720 - 30;

	int width = 10;

	s->addEventAngle(30 - width, TriggerValue::RISE);
	s->addEventAngle(30, TriggerValue::FALL);

	for (int i = 1;i<=count;i++) {
		s->addEventAngle(s->getCycleDuration() / count * i - width, TriggerValue::RISE);
		s->addEventAngle(s->getCycleDuration() / count * i, TriggerValue::FALL);
	}
}

void configureDaihatsu3cyl(TriggerWaveform * s) {
	daihatsu(s, 3);
	s->setTriggerSynchronizationGap(0.125);
}


void configureDaihatsu4cyl(TriggerWaveform * s) {
	daihatsu(s, 4);
	s->setTriggerSynchronizationGap(0.17);
}

void configureBarra3plus1cam(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);

	// This wheel has four teeth
	// two short gaps, and two long gaps
	// short = 60 deg
	// long = 120 deg

	{
		int offset = 60;
		int w = 5;

		s->addToothRiseFall(offset, w);

		// short gap 60 deg
		s->addToothRiseFall(offset + 60, w);

		// long gap 120 deg
		s->addToothRiseFall(offset + 180, w);
		// long gap 120 deg
		s->addToothRiseFall(offset + 300, w);

		// short gap, 60 deg back to zero/720
	}

	// sync tooth is the zero tooth, the first short gap after two long gaps
	s->setTriggerSynchronizationGap3(0, 1.6f, 2.4f);
	// previous gap should be 1.0
	s->setTriggerSynchronizationGap3(1, 0.8f, 1.2f);
}

void configureBenelli(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

    constexpr float magic = 15;

	float angle = 3 * magic;

	s->addEvent360(angle / 2 /* 22.5 */, TriggerValue::RISE);
	s->addEvent360(angle /*     45   */, TriggerValue::FALL);

	for (int i = 0;i< 24 - 3;i++) {
		angle += magic / 2;
		s->addEvent360(angle, TriggerValue::RISE);

		angle += magic / 2;
		s->addEvent360(angle, TriggerValue::FALL);
	}
}

void configure60degSingleTooth(TriggerWaveform *s) {
	/** @todo
	 * My approach was to utilize ::Both especially for single
	 * tooth and manual kikstarter, to be ready on both sides of blind.
	 * But unfortuneally, my experiments show me the Trigger can't
	 * become syncronized by 'last' and folowed 'first' events only.
	 * Also I observe phase-aligment mehanism is trying to consume a
	 * longer side of trigger as latest before TDC.
	 * I wish to setup SyncEdge::Both for my TT_60DEG_TOOTH after
	 * this case of scenario become work well. For now, ::Rise work
	 * well for my 60 degree trigger and both edges phase sync work
	 * as mush as expected for my engine startup. */

	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::Rise);

	s->addEvent360(300, TriggerValue::RISE);
	s->addEvent360(360, TriggerValue::FALL);

	s->tdcPosition = 60;

	s->isSynchronizationNeeded = false;
	s->useOnlyPrimaryForSync = true;
}

void configureArcticCat(TriggerWaveform *s) {
  s->initialize(TWO_STROKE, SyncEdge::Rise);

  int totalTeethCount = 24;
  float engineCycle = TWO_STROKE_CYCLE_DURATION;
  float toothWidth = 0.5;

    addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/0, engineCycle,
    		/*from*/ 30 + 1, /* to */ 195 + 1);


    addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/0, engineCycle,
    		/*from*/ 210 + 1, /* to */ NO_RIGHT_FILTER);

  s->setTriggerSynchronizationGap(2);
  int c = 9;
  for (int gapIndex = 1; gapIndex <= c; gapIndex++) {
    s->setTriggerSynchronizationGap3(gapIndex, 0.75, 1.25);
  }


}

/**
 * Audi 5 cylinder trigger wheel implementation
 *
 * Physical trigger system (Motronic sensor designations):
 * - G28: VR 135 crankteeth - Engine Speed sensor on flywheel/starter ring
 * - G4:  VR crank home - Ignition Timing Reference, single tooth at 62° BTDC #1
 * - G40: CAM HALL sensor - Single window per cam rotation, used for 720° sync
 *
 * Wiring:
 * - G28 (T55/47) → Primary Trigger Input (135 teeth for RPM)
 * - G4  (T55/49) → Secondary Trigger Input (1 tooth timing reference)
 * - G40          → CAM Input 0 (HALL sensor, single window)
 * - T55/48       → VR Ground (shared by G28 and G4)
 *
 * The G40 CAM HALL signal acts as a software gate for the G4 timing reference:
 * - When G40 CAM HALL is HIGH: G4 crank home pulse is PROCESSED
 * - When G40 CAM HALL is LOW: G4 crank home pulse is IGNORED
 *
 * Software gating: G40 CAM HALL gates the G4 timing reference signal
 * to produce 1 crank-precise sync pulse per cam rotation (720°)
 *
 * Based on VEMS documentation for Audi 5 cylinder engines.
 * 135 teeth per 360° crank rotation = 2.667° per tooth
 * G4 crank home at 62° BTDC cylinder #1
 *
 * Note: This trigger uses the primary channel for the G28 135-tooth wheel only.
 * The G4 secondary signal and G40 CAM HALL gating are handled by software gating
 * logic in trigger_central.cpp when useAudiTriggerGating is enabled.
 */
void configureAudi5Cyl135_1_1(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);

	// The G28 135-tooth wheel has no missing teeth (no gaps), so there's no gap-based
	// sync pattern to detect. isSynchronizationNeeded refers to gap detection in
	// the primary trigger signal, which is not applicable here.
	s->isSynchronizationNeeded = false;
	// shapeWithoutTdc indicates the primary wheel pattern doesn't provide
	// a unique TDC reference point - TDC sync comes from G4 via software gating
	s->shapeWithoutTdc = true;
	// needSecondTriggerInput enables processing of the G4 secondary signal,
	// which is used with G40 CAM HALL software gating for cycle synchronization
	s->needSecondTriggerInput = true;
	s->tdcPosition = 62;                 // G4 crank home is 62° BTDC cylinder #1

	float engineCycle = FOUR_STROKE_ENGINE_CYCLE; // 720°
	float toothWidth = 0.5;
	int totalTeethCount = 135;

	// PRIMARY: 135 evenly-spaced teeth from G28 sensor
	// Over 720° engine cycle, this means 270 tooth events
	addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, totalTeethCount, 0,
		toothWidth, 0, engineCycle, NO_LEFT_FILTER, NO_RIGHT_FILTER);

	// Note: Secondary (G4) and G40 CAM HALL gating are implemented in software
	// via useAudiTriggerGating configuration option in trigger_central.cpp
}
