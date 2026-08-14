/*
 * @file trigger_toyota.cpp
 *
 * https://thedeltaecho.wordpress.com/2010/03/14/2jz-ge-cam-crank-signals/
 *
 * @date Dec 14, 2015
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "trigger_toyota.h"

/**
 * https://rusefi.com/forum/viewtopic.php?f=5&t=1720
 */
void initialize2jzGE3_34_simulation_shape(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Rise);

	float camD = 720 / 6; // 120

	float crankDelta = 720 / 36 / 2; // 10

	float camAngle = 1;
	float crankAngle = 2 * crankDelta; // skipping two teeth

	for (int i = 0; i < 10; i++) {
		s->addEvent720(crankAngle + 5, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(crankAngle + crankDelta, TriggerValue::FALL, TriggerWheel::T_SECONDARY); // 120
		crankAngle += 10;
	} // 2 + 10

	camAngle += camD;
	s->addEvent720(camAngle, TriggerValue::RISE, TriggerWheel::T_PRIMARY); // 120
	s->addEvent720(camAngle + 3, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	for (int i = 0; i < 12; i++) {
		s->addEvent720(crankAngle + 5, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(crankAngle + crankDelta, TriggerValue::FALL, TriggerWheel::T_SECONDARY); // 120
		crankAngle += 10;
	} // 2 + 22


	camAngle += camD;

	for (int i = 0; i < 12; i++) {
		s->addEvent720(crankAngle + 5, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(crankAngle + crankDelta, TriggerValue::FALL, TriggerWheel::T_SECONDARY); // 120
		crankAngle += crankDelta;
	} // 2 + 34

	camAngle += camD;
	s->addEvent720(camAngle, TriggerValue::RISE, TriggerWheel::T_PRIMARY); // 360
	s->addEvent720(camAngle + 3, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

	crankAngle += 20; // skipping two teeth one more time
	for (int i = 0; i < 10; i++) {
		s->addEvent720(crankAngle + 5, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(crankAngle + crankDelta, TriggerValue::FALL, TriggerWheel::T_SECONDARY); // 120
		crankAngle += crankDelta;
	} // 2 + 10

	camAngle += camD;

	for (int i = 0; i < 12; i++) {
		s->addEvent720(crankAngle + 5, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(crankAngle + crankDelta, TriggerValue::FALL, TriggerWheel::T_SECONDARY); // 120
		crankAngle += crankDelta;
	} // 2 + 22

	camAngle += camD;
	s->addEvent720(camAngle, TriggerValue::RISE, TriggerWheel::T_PRIMARY); // 600
	s->addEvent720(camAngle + 3, TriggerValue::FALL, TriggerWheel::T_PRIMARY);


	for (int i = 0; i < 12; i++) {
		s->addEvent720(crankAngle + 5, TriggerValue::RISE, TriggerWheel::T_SECONDARY);
		s->addEvent720(crankAngle + crankDelta, TriggerValue::FALL, TriggerWheel::T_SECONDARY); // 120
		crankAngle += 10;
	} // 2 + 32
	camAngle += camD;

	s->shapeWithoutTdc = true;
	s->isSynchronizationNeeded = false;
}

void initializeUzCam(TriggerWaveform *s) {
	static const angle_t angles[] = { 135, 270, 360 };
	initializeRiseOnlyTrigger(s, 5, angles, efi::size(angles));

	s->setTriggerSynchronizationGap4(/*gapIndex*/0, 1.5);
	s->setTriggerSynchronizationGap4(/*gapIndex*/1, 0.67);
}

void initialize_3GRfSE_CAM(TriggerWaveform *s) {
 
  s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Fall);

  s->shapeWithoutTdc = true;

	// SyncEdge::Fall tracks fall-to-fall durations 155/90/115, ratios 0.58/1.28/1.35:
	// sync on the unique short-after-long 0.58 gap, the other two are too close to each other
	s->setTriggerSynchronizationGap2(0.4, 0.7);

    
  s->addEvent360(10,  TriggerValue::RISE, TriggerWheel::T_PRIMARY);
  s->addEvent360(70,  TriggerValue::FALL, TriggerWheel::T_PRIMARY);

  s->addEvent360(130, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
  s->addEvent360(215, TriggerValue::FALL, TriggerWheel::T_PRIMARY);

  s->addEvent360(250, TriggerValue::RISE, TriggerWheel::T_PRIMARY);
  s->addEvent360(280, TriggerValue::FALL, TriggerWheel::T_PRIMARY);
}
