/**
 * @file	trigger_mitsubishi.cpp
 *
 * @date Aug 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_mitsubishi.h"
#include "trigger_universal.h"

void configureFordAspireTriggerWaveform(TriggerWaveform * s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::RiseOnly);
	s->isSynchronizationNeeded = false;
	s->isSecondWheelCam = true;

	float x = 121.90;
	float y = 110.86;

	s->addEvent720(x + y - 180, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(x, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + y, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + 180, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(360, TriggerValue::RISE, TriggerWheel::T_PRIMARY);

	s->addEvent720(x + 180 + y, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + 360, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + 360 + y, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
	s->addEvent720(x + 540, TriggerValue::FALL, TriggerWheel::T_SECONDARY);
	s->addEvent720(720, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
}

void initializeMitsubishi4gSymmetricalCrank(TriggerWaveform *s) {
		initializeSkippedToothTrigger(s, /* reality */2 * /* symmetrical*/0.5,
				0, FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR, SyncEdge::RiseOnly);
#if EFI_UNIT_TEST
		s->knownOperationMode = true; // this is always placed on crank, overriding 'initializeSkippedToothTrigger' value
#endif
}

// https://github.com/rusefi/rusefi/issues/5593
void initializeVvt6G72(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Both);

    //happy ratio 0.548387 @ 0
    //happy ratio 0.941176 @ 1
    //happy ratio 0.500000 @ 2
    //happy ratio 3.750002 @ 3
    //happy ratio 0.266667 @ 4
    //happy ratio 3.250000 @ 5
    //happy ratio 0.307692 @ 6
    //happy ratio 3.875000 @ 7

    // real working ranges for all of the engine states
    s->setTriggerSynchronizationGap3(0, 1.3, 3.2);
    s->setTriggerSynchronizationGap3(1, 0.3, 0.66);
    s->setTriggerSynchronizationGap3(2, 1.3, 3.2);
    s->setTriggerSynchronizationGap3(3, 0.3, 0.66);
    s->setTriggerSynchronizationGap3(4, 1.3, 3.2);

    s->addEvent360(52.5, TriggerValue::FALL);
    s->addEvent360(82.5, TriggerValue::RISE);
    s->addEvent360(112.5, TriggerValue::FALL);
    s->addEvent360(177.5, TriggerValue::RISE);
    s->addEvent360(207.5, TriggerValue::FALL);
    s->addEvent360(262.5, TriggerValue::RISE);
    s->addEvent360(292.5, TriggerValue::FALL);
    s->addEvent360(360, TriggerValue::RISE);
}

void initializeMitsubishi4g63Cam(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Both);

	// nominal gap 0.5
	s->setSecondTriggerSynchronizationGap2(0.2, 0.95);

	// nominal gap 3.0
	s->setTriggerSynchronizationGap2(2.0f, 5.0f);

	// 131 deg before #1 TDC
	// 49 deg after #1 TDC
	s->addToothRiseFall(225, /*width*/90);

	// 131 deg before #4 TDC
	// 41 deg before #4 TDC
	s->addToothRiseFall(360, /*width*/45);
}

void initialize36_2_1_1(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);
	s->tdcPosition = 90;

  const float WIDE_TOOTH_WIDTH  = 8.5;
  const float WIDE_TOOTH_OFFSET = 6.5;
  const float TOOTH_WIDTH = 5.0;
  const float GAP_WIDTH   = 5.0;

  float offset = WIDE_TOOTH_WIDTH;

  // First tooth after the gap is wide
  // we can't have an event at 0, so we wrap the first rise event around to the end of the cycle
  s->addEvent360(offset, TriggerValue::FALL);
  offset += GAP_WIDTH;

  // Then we have 10 regular teeth
  for (int i = 0; i < 10; i++) {
    s->addEvent360(offset, TriggerValue::RISE);
    s->addEvent360(offset + TOOTH_WIDTH, TriggerValue::FALL);
    offset += TOOTH_WIDTH + GAP_WIDTH;
  }

  // Then a gap followed by a wide tooth
  offset += WIDE_TOOTH_OFFSET;
  s->addEvent360(offset, TriggerValue::RISE);
  offset += WIDE_TOOTH_WIDTH;
  s->addEvent360(offset, TriggerValue::FALL);
  offset += GAP_WIDTH;

  for (int i = 0; i < 10; i++) {
    s->addEvent360(offset, TriggerValue::RISE);
    s->addEvent360(offset + TOOTH_WIDTH, TriggerValue::FALL);
    offset += TOOTH_WIDTH + GAP_WIDTH;
  }

  // Then another gap followed by a wide tooth
  offset += WIDE_TOOTH_OFFSET;
  s->addEvent360(offset, TriggerValue::RISE);
  offset += WIDE_TOOTH_WIDTH;
  s->addEvent360(offset, TriggerValue::FALL);
  offset += GAP_WIDTH;

  for (int i = 0; i < 9; i++) {
    s->addEvent360(offset, TriggerValue::RISE);
    s->addEvent360(offset + TOOTH_WIDTH, TriggerValue::FALL);
    offset += TOOTH_WIDTH + GAP_WIDTH;
  }

  offset += TOOTH_WIDTH + GAP_WIDTH + WIDE_TOOTH_OFFSET; // gap before the last tooth
  // Finally, the last tooth is wide and we wrap it around to the beginning of the cycle
  // offset should be exactly 360 at this point
  s->addEvent360(offset, TriggerValue::RISE);

  // Looking at real captures:
  // wide gap normal   1.0 - 2.6 - 0.5
  // wide gap crank    1.0 - 1.7 - 0.4
  // narrow gap normal 1.0 - 1.65- 0.8
  // narrow gap crank  0.8 - 1.0 - 0.7

  s->setTriggerSynchronizationGap3(0, 0.3, 0.6);
  s->setTriggerSynchronizationGap3(1, 1.7, 3.6);
  s->setTriggerSynchronizationGap3(2, 0.8, 1.2);
}

// Mitsubishi 4B11
// https://wiki.rusefi.com/All-Supported-Triggers#36-2-1
void initialize36_2_1(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CRANK_SENSOR, SyncEdge::RiseOnly);
	s->tdcPosition = 90;
	int totalTeethCount = 36;

	float engineCycle = FOUR_STROKE_ENGINE_CYCLE;
	float toothWidth = 0.5;

	float oneTooth = 720 / totalTeethCount;

	float offset = (36 - 16 - 2 - 17) * oneTooth;

	addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/offset, engineCycle,
			NO_LEFT_FILTER, offset + 16 * oneTooth + 1);

	offset += (16 + 2) * oneTooth;

	addSkippedToothTriggerEvents(TriggerWheel::T_PRIMARY, s, totalTeethCount, 0, toothWidth, /*offset*/offset, engineCycle,
			NO_LEFT_FILTER, offset + 17 * oneTooth + 1);


	s->setTriggerSynchronizationGap(3);
	s->setSecondTriggerSynchronizationGap(1); // redundancy
}

void initializeVvt3A92(TriggerWaveform *s) {
	static const angle_t angles[] = { 120, 132, 240, 360 };
	initializeRiseOnlyTrigger(s, 5, angles, efi::size(angles), FOUR_STROKE_CRANK_SENSOR);

	s->setTriggerSynchronizationGap(9);
	s->setSecondTriggerSynchronizationGap(0.11); // redundancy
}

void initializeMitsubishi4G69Cam(TriggerWaveform *s) {
	static const angle_t angles[] = { 90, 160, 180, 250, 270, 360 };
	initializeRiseOnlyTrigger(s, 5, angles, efi::size(angles));

    s->setTriggerSynchronizationGap(1);
   	s->setSecondTriggerSynchronizationGap(4.5);
}

void initializeMitsubishi6G75Cam(TriggerWaveform *s) {
	// todo: implement me!
}
