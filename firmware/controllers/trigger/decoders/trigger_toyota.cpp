/*
 * @file trigger_toyota.cpp
 *
 * https://thedeltaecho.wordpress.com/2010/03/14/2jz-ge-cam-crank-signals/
 *
 * @date Dec 14, 2015
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "trigger_toyota.h"

EXTERN_ENGINE;

void initialize2jzGE1_12(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	float crankD = 360 / 12 / 2; // 15

	float crankAngle = 10;
	s->addEvent2(crankAngle, T_SECONDARY, TV_FALL, -1, 721 PASS_ENGINE_PARAMETER_SUFFIX); // 120

	for (int i = 0; i < 2; i++) {
		s->addEvent2(crankAngle + crankD, T_SECONDARY, TV_RISE, -1, 721 PASS_ENGINE_PARAMETER_SUFFIX);
		crankAngle += crankD;
		s->addEvent2(crankAngle + crankD, T_SECONDARY, TV_FALL, -1, 721 PASS_ENGINE_PARAMETER_SUFFIX); // 120
		crankAngle += crankD;
	}


	s->addEvent2(75, T_PRIMARY, TV_FALL, -1, 721 PASS_ENGINE_PARAMETER_SUFFIX);

	for (int i = 0; i < 21; i++) {
		s->addEvent2(crankAngle + crankD, T_SECONDARY, TV_RISE, -1, 721 PASS_ENGINE_PARAMETER_SUFFIX);
		crankAngle += crankD;
		s->addEvent2(crankAngle + crankD, T_SECONDARY, TV_FALL, -1, 721 PASS_ENGINE_PARAMETER_SUFFIX); // 120
		crankAngle += crankD;
	}

	s->addEvent2(crankAngle + crankD, T_SECONDARY, TV_RISE, -1, 721 PASS_ENGINE_PARAMETER_SUFFIX);
	crankAngle += crankD;


	s->addEvent2(720, T_PRIMARY, TV_RISE, -1, 721 PASS_ENGINE_PARAMETER_SUFFIX);

	s->isSynchronizationNeeded = false;
}

void initialize2jzGE3_34(TriggerShape *s DECLARE_ENGINE_PARAMETER_SUFFIX) {
	setToothedWheelConfiguration(s, 36, 2, FOUR_STROKE_CRANK_SENSOR PASS_ENGINE_PARAMETER_SUFFIX);

//	s->initialize(FOUR_STROKE_CAM_SENSOR, true);
//
//	float camD = 720 / 6; // 120
//
//	float crankAngle = 20; // skipping two teeth
//
//	for (int i = 0; i < 10; i++) {
//		s->addEvent2(crankAngle + 5, T_SECONDARY, TV_RISE, -1, 721);
//		s->addEvent2(crankAngle + 9.9, T_SECONDARY, TV_FALL, -1, 721); // 120
//		crankAngle += 10;
//	} // 2 + 10
//
//	float camAngle = 0;
//	camAngle += camD;
//	s->addEvent2(camAngle, T_PRIMARY, TV_RISE, -1, 721); // 120
//
//	for (int i = 0; i < 12; i++) {
//		s->addEvent2(crankAngle + 5, T_SECONDARY, TV_RISE, -1, 721);
//		s->addEvent2(crankAngle + 9.9, T_SECONDARY, TV_FALL, -1, 721); // 120
//		crankAngle += 10;
//	} // 2 + 22
//
//
//	camAngle += camD;
//	s->addEvent2(camAngle, T_PRIMARY, TV_FALL, -1, 721); // 240
//
//	for (int i = 0; i < 12; i++) {
//		s->addEvent2(crankAngle + 5, T_SECONDARY, TV_RISE, -1, 721);
//		s->addEvent2(crankAngle + 9.9, T_SECONDARY, TV_FALL, -1, 721); // 120
//		crankAngle += 10;
//	} // 2 + 34
//
//	camAngle += camD;
//	s->addEvent2(camAngle, T_PRIMARY, TV_RISE, -1, 721); // 360
//
//	crankAngle += 20; // skipping two teeth one more time
//	for (int i = 0; i < 10; i++) {
//		s->addEvent2(crankAngle + 5, T_SECONDARY, TV_RISE, -1, 721);
//		s->addEvent2(crankAngle + 9.9, T_SECONDARY, TV_FALL, -1, 721); // 120
//		crankAngle += 10;
//	} // 2 + 10
//
//	camAngle += camD;
//	s->addEvent2(camAngle, T_PRIMARY, TV_FALL, -1, 721); // 480
//
//	for (int i = 0; i < 12; i++) {
//		s->addEvent2(crankAngle + 5, T_SECONDARY, TV_RISE, -1, 721);
//		s->addEvent2(crankAngle + 9.9, T_SECONDARY, TV_FALL, -1, 721); // 120
//		crankAngle += 10;
//	} // 2 + 22
//
//	camAngle += camD;
//	s->addEvent2(camAngle, T_PRIMARY, TV_RISE, -1, 721); // 600
//
//
//	for (int i = 0; i < 12; i++) {
//		s->addEvent2(crankAngle + 5, T_SECONDARY, TV_RISE, -1, 721);
//		s->addEvent2(crankAngle + 9.9, T_SECONDARY, TV_FALL, -1, 721); // 120
//		crankAngle += 10;
//	} // 2 + 32
//	camAngle += camD;
//	s->addEvent2(camAngle, T_PRIMARY, TV_FALL, -1, 721); // 720
//
//	s->isSynchronizationNeeded = false;

}
