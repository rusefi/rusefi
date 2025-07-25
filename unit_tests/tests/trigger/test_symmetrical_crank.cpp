/*
 * @file test_symmetrical_crank.cpp
 *
 * @date Feb 14, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

static void postFourEvents(EngineTestHelper *eth, float mult) {
	eth->fireFall(mult * 394);
	eth->fireRise(mult * 16);
	eth->fireFall(mult * 294);
	eth->fireRise(mult * 16);
}

TEST(engine, testAngleLogicInSymmetricalCrankIssue2980) {
	EngineTestHelper eth(engine_type_e::MAZDA_MIATA_NB2);

	TriggerFormDetails *triggerForm = &engine->triggerCentral.triggerFormDetails;

	TriggerWaveform * form = &engine->triggerCentral.triggerShape;

	#define EXPECT_FINDANGLE(angle, idx) EXPECT_EQ(form->findAngleIndex(triggerForm, angle) & 0xFFFF'FFFE, idx);

	// Check one angle just after every trigger tooth, for two full revolutions (720 degrees, one engine cycle, 4 loops of the trigger)

	// First quarter
	EXPECT_FINDANGLE(0 * 180 + 5, 0u);		// 5
	EXPECT_FINDANGLE(0 * 180 + 115, 2u);		// 115

	// Second quarter
	EXPECT_FINDANGLE(1 * 180 + 5, 4u);		// 180+5 = 185
	EXPECT_FINDANGLE(1 * 180 + 115, 6u);		// 180+115 = 295

	// Third quarter
	EXPECT_FINDANGLE(2 * 180 + 5, 8u);		// 360+5 = 365
	EXPECT_FINDANGLE(2 * 180 + 115, 10u);	// 360+115 = 475

	// Fourth quarter
	EXPECT_FINDANGLE(3 * 180 + 5, 12u);		// 540+5 = 545
	EXPECT_FINDANGLE(3 * 180 + 115, 14u);	// 540+115 = 655
}

TEST(engine, testSymmetricalCrank) {

	EngineTestHelper eth(engine_type_e::MAZDA_MIATA_NB2);

	// this test is not about isFasterEngineSpinUpEnabled so let's disable it to simplify things
	engineConfiguration->isFasterEngineSpinUpEnabled = false;
	engineConfiguration->alwaysInstantRpm = true;

	ASSERT_EQ(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR, getEngineRotationState()->getOperationMode());

	float mult = 0.02;

	ASSERT_EQ( 0,  round(Sensor::getOrZero(SensorType::Rpm))) << "RPM#0";

	postFourEvents(&eth, mult);
	ASSERT_EQ( 0,  round(Sensor::getOrZero(SensorType::Rpm))) << "RPM#0";

	eth.fireFall(mult * 394);
	eth.fireRise(mult * 16);
	eth.fireFall(mult * 294);
	ASSERT_FALSE(engine->triggerCentral.triggerState.getShaftSynchronized());
	eth.fireRise(mult * 16);
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());

	ASSERT_EQ( 0,  round(Sensor::getOrZero(SensorType::Rpm))) << "RPM#0";

	postFourEvents(&eth, mult);
	ASSERT_EQ(2084,  round(Sensor::getOrZero(SensorType::Rpm))) << "RPM#11";

	postFourEvents(&eth, mult);
	ASSERT_EQ(2084,  round(Sensor::getOrZero(SensorType::Rpm))) << "RPM#11";

	postFourEvents(&eth, mult);
	ASSERT_EQ(2084,  round(Sensor::getOrZero(SensorType::Rpm))) << "RPM#11";
}
