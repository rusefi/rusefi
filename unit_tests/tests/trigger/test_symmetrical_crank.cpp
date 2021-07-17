/*
 * @file test_symmetrical_crank.cpp
 *
 * @date Feb 14, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_test_helper.h"

static void postFourEvents(EngineTestHelper *eth, float mult) {
	eth->fireFall(mult * 384);
	eth->fireRise(mult * 16);
	eth->fireFall(mult * 304);
	eth->fireRise(mult * 16);
}

TEST(engine, testAngleLogicInSymmetricalCrankIssue2980) {
	WITH_ENGINE_TEST_HELPER(MAZDA_MIATA_2003);

	TriggerFormDetails *triggerForm = &ENGINE(triggerCentral.triggerFormDetails);

	TriggerWaveform * form = &ENGINE(triggerCentral.triggerShape);

	ASSERT_EQ(form->findAngleIndex(triggerForm, 10), 1);
	ASSERT_EQ(form->findAngleIndex(triggerForm, 180), 5);

	ASSERT_EQ(form->findAngleIndex(triggerForm, 310), 5);
	ASSERT_EQ(form->findAngleIndex(triggerForm, 540), 5);

	ASSERT_EQ(form->findAngleIndex(triggerForm, 640), 5);
	ASSERT_EQ(form->findAngleIndex(triggerForm, 650), 7);
	ASSERT_EQ(form->findAngleIndex(triggerForm, 660), 15);
	ASSERT_EQ(form->findAngleIndex(triggerForm, 670), 15);
	ASSERT_EQ(form->findAngleIndex(triggerForm, 680), 15);

	ASSERT_EQ(form->findAngleIndex(triggerForm, 700), 15);
	ASSERT_EQ(form->findAngleIndex(triggerForm, 710), 15);
}

TEST(engine, testSymmetricalCrank) {

	WITH_ENGINE_TEST_HELPER(MAZDA_MIATA_2003);

	// this test is not about isFasterEngineSpinUpEnabled so let's disable it to simplify things
	CONFIG(isFasterEngineSpinUpEnabled) = false;


	ASSERT_EQ(FOUR_STROKE_SYMMETRICAL_CRANK_SENSOR, engine->getOperationMode(PASS_ENGINE_PARAMETER_SIGNATURE));

	float mult = 0.02;

	ASSERT_EQ( 0,  GET_RPM()) << "RPM#0";

	postFourEvents(&eth, mult);
	ASSERT_EQ( 0,  GET_RPM()) << "RPM#0";

	eth.fireFall(mult * 384);
	eth.fireRise(mult * 16);
	eth.fireFall(mult * 304);
	ASSERT_FALSE(engine->triggerCentral.triggerState.getShaftSynchronized());
	eth.fireRise(mult * 16);
	ASSERT_TRUE(engine->triggerCentral.triggerState.getShaftSynchronized());

	ASSERT_EQ( 0,  GET_RPM()) << "RPM#0";



	for (int i = 0; i < 6 ; i++) {
		postFourEvents(&eth, mult);
		ASSERT_EQ( 0,  GET_RPM()) << "RPM#0";
	}

	mult = 0.1;
	postFourEvents(&eth, mult);
	ASSERT_EQ( 1042,  GET_RPM()) << "RPM#11";

	postFourEvents(&eth, mult);
	ASSERT_EQ( 1042,  GET_RPM()) << "RPM#11";

	postFourEvents(&eth, mult);
	ASSERT_EQ( 1042,  GET_RPM()) << "RPM#11";
}
