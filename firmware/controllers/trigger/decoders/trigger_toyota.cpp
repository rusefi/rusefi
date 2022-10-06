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
		s->addEvent720(crankAngle + 5, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(crankAngle + crankDelta, TriggerWheel::T_SECONDARY, TriggerValue::FALL); // 120
		crankAngle += 10;
	} // 2 + 10

	camAngle += camD;
	s->addEvent720(camAngle, TriggerWheel::T_PRIMARY, TriggerValue::RISE); // 120
	s->addEvent720(camAngle + 3, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	for (int i = 0; i < 12; i++) {
		s->addEvent720(crankAngle + 5, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(crankAngle + crankDelta, TriggerWheel::T_SECONDARY, TriggerValue::FALL); // 120
		crankAngle += 10;
	} // 2 + 22


	camAngle += camD;

	for (int i = 0; i < 12; i++) {
		s->addEvent720(crankAngle + 5, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(crankAngle + crankDelta, TriggerWheel::T_SECONDARY, TriggerValue::FALL); // 120
		crankAngle += crankDelta;
	} // 2 + 34

	camAngle += camD;
	s->addEvent720(camAngle, TriggerWheel::T_PRIMARY, TriggerValue::RISE); // 360
	s->addEvent720(camAngle + 3, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	crankAngle += 20; // skipping two teeth one more time
	for (int i = 0; i < 10; i++) {
		s->addEvent720(crankAngle + 5, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(crankAngle + crankDelta, TriggerWheel::T_SECONDARY, TriggerValue::FALL); // 120
		crankAngle += crankDelta;
	} // 2 + 10

	camAngle += camD;

	for (int i = 0; i < 12; i++) {
		s->addEvent720(crankAngle + 5, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(crankAngle + crankDelta, TriggerWheel::T_SECONDARY, TriggerValue::FALL); // 120
		crankAngle += crankDelta;
	} // 2 + 22

	camAngle += camD;
	s->addEvent720(camAngle, TriggerWheel::T_PRIMARY, TriggerValue::RISE); // 600
	s->addEvent720(camAngle + 3, TriggerWheel::T_PRIMARY, TriggerValue::FALL);


	for (int i = 0; i < 12; i++) {
		s->addEvent720(crankAngle + 5, TriggerWheel::T_SECONDARY, TriggerValue::RISE);
		s->addEvent720(crankAngle + crankDelta, TriggerWheel::T_SECONDARY, TriggerValue::FALL); // 120
		crankAngle += 10;
	} // 2 + 32
	camAngle += camD;

	s->shapeWithoutTdc = true;
	s->isSynchronizationNeeded = false;
}

void initializeToyota4_1(TriggerWaveform *s) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Fall);

	int w = 10;

	s->addEvent360(180 - w, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(180, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	s->addEvent360(270 - w, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(270, TriggerWheel::T_PRIMARY, TriggerValue::FALL);

	s->addEvent360(360 - w, TriggerWheel::T_PRIMARY, TriggerValue::RISE);
	s->addEvent360(360, TriggerWheel::T_PRIMARY, TriggerValue::FALL);
}
