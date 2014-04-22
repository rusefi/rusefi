/**
 * @file	test_trigger_decoder.c
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#include "main.h"
#include "test_trigger_decoder.h"
#include "trigger_decoder.h"
#include "engine_math.h"

#include "ford_aspire.h"
#include "dodge_neon.h"
#include "ford_1995_inline_6.h"
#include "mazda_323.h"

void sendOutConfirmation(char *value, int i) {
	// test implementation
}

static void testDodgeNeonDecoder(void) {
	printf("*************************************************** testDodgeNeonDecoder\r\n");
	initTriggerDecoder();

	engine_configuration_s ec;
	engine_configuration2_s ec2;

	resetConfigurationExt(DODGE_NEON_1995, &ec, &ec2);

	trigger_shape_s * shape = &ec2.triggerShape;
	trigger_state_s state;
	clearTriggerState(&state);




	assertFalseM("1 shaft_is_synchronized", state.shaft_is_synchronized);


	int r = 0;
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, r + 60);
	assertFalseM("2 shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization

	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, r + 210);
	assertFalseM("3 shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization

	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, r + 420);
	assertFalseM("4 shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization

	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, r + 630);
	assertFalse(state.shaft_is_synchronized); // still no synchronization

	printf("2nd camshaft revolution\r\n");
	r = 720;
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, r + 60);
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, r + 210);
	assertTrue(state.shaft_is_synchronized);
	assertEquals(0, state.current_index);
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, r + 420);
	assertEquals(1, state.current_index);
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, r + 630);
	assertEquals(2, state.current_index);

	printf("3rd camshaft revolution\r\n");
	r = 2 * 720;
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, r + 60);
	assertEqualsM("current index", 3, state.current_index);
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, r + 210);
	assertTrue(state.shaft_is_synchronized);
	assertEqualsM("current index", 0, state.current_index);
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, r + 420);
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, r + 630);
}

static void test1995FordInline6TriggerDecoder(void) {
	printf("*************************************************** test1995FordInline6TriggerDecoder\r\n");
	initTriggerDecoder();
	resetOutputSignals();

	engine_configuration_s ec;
	engine_configuration2_s ec2;

	resetConfigurationExt(FORD_INLINE_6_1995, &ec, &ec2);

	ActuatorEventList *ecl = &ec2.engineEventConfiguration.ignitionEvents;
	assertEqualsM("ignition events size", 6, ecl->size);
	assertEqualsM("event index", 0, ecl->events[0].eventIndex);
	assertEquals(13, ecl->events[0].angleOffset);

	assertEqualsM("event index", 10, ecl->events[5].eventIndex);
	assertEquals(13, ecl->events[5].angleOffset);

	trigger_state_s state;
	clearTriggerState(&state);

	trigger_shape_s * shape = &ec2.triggerShape;
	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized);
	int r = 0;
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, r + 10);
	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, r + 11);
	assertTrue(state.shaft_is_synchronized); // first signal rise synchronize
	assertEquals(0, state.current_index);
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, r++);
	assertEquals(1, state.current_index);

	for (int i = 2; i < 10;) {
		processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, r++);
		assertEqualsM("even", i++, state.current_index);
		processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, r++);
		assertEqualsM("odd", i++, state.current_index);
	}

	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, r++);
	assertEquals(10, state.current_index);

	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, r++);
	assertEquals(11, state.current_index);

	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, r++);
	assertEquals(0, state.current_index); // new revolution

	assertEqualsM("running dwell", 0.5, getSparkDwellMsT(&ec, 2000));
}

void testFordAspire(void) {
	printf("*************************************************** testTriggerDecoder\r\n");

	engine_configuration_s ec;
	engine_configuration2_s ec2;
	resetConfigurationExt(FORD_ASPIRE_1996, &ec, &ec2);

	assertEqualsM("cranking dwell", 54.166670, getSparkDwellMsT(&ec, 200));
	assertEqualsM("running dwell", 4, getSparkDwellMsT(&ec, 2000));

	assertEqualsM("higher rpm dwell", 3.25, getSparkDwellMsT(&ec, 6000));
}

void testMazda323(void) {
	printf("*************************************************** testMazda323\r\n");

	engine_configuration_s ec;
	engine_configuration2_s ec2;
	resetConfigurationExt(MAZDA_323, &ec, &ec2);

}

void testMazdaMianaNbDecoder(void) {
	printf("*************************************************** testMazdaMianaNbDecoder\r\n");

	engine_configuration_s ec;
	engine_configuration2_s ec2;
	resetConfigurationExt(MAZDA_MIATA_NB, &ec, &ec2);

	trigger_state_s state;
	clearTriggerState(&state);
	trigger_shape_s * shape = &ec2.triggerShape;

	int a = 0;
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, a + 20);
	assertFalseM("0a shaft_is_synchronized", state.shaft_is_synchronized);
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, a + 340);
	assertFalseM("0b shaft_is_synchronized", state.shaft_is_synchronized);

	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, a + 360);
	assertFalseM("0c shaft_is_synchronized", state.shaft_is_synchronized);
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, a + 380);
	assertFalseM("0d shaft_is_synchronized", state.shaft_is_synchronized);
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, a + 400);
	assertTrueM("0e shaft_is_synchronized", state.shaft_is_synchronized);

	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, a + 720);
	assertTrueM("0f shaft_is_synchronized", state.shaft_is_synchronized);

	a = 720;
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, a + 20);
	assertTrueM("1a shaft_is_synchronized", state.shaft_is_synchronized);
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, a + 340);
	assertTrueM("1b shaft_is_synchronized", state.shaft_is_synchronized);

	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, a + 360);
	assertTrueM("1c shaft_is_synchronized", state.shaft_is_synchronized);
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, a + 380);
	assertTrueM("1d shaft_is_synchronized", state.shaft_is_synchronized);
	assertEquals(5, state.current_index);

	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, a + 400);
	assertTrueM("1e shaft_is_synchronized", state.shaft_is_synchronized);
	assertEquals(0, state.current_index);


	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, a + 720);
	assertTrueM("1f shaft_is_synchronized", state.shaft_is_synchronized);


}

void testGY6_139QMB(void) {
	printf("*************************************************** testGY6_139QMB\r\n");

	engine_configuration_s ec;
	engine_configuration2_s ec2;
	resetConfigurationExt(GY6_139QMB, &ec, &ec2);

	trigger_state_s state;
	clearTriggerState(&state);
	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized);

	trigger_shape_s * shape = &ec2.triggerShape;

	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized);
	assertEquals(0, state.current_index);

	int now = 0;
	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_UP, now++);
	assertTrueM("shaft_is_synchronized", state.shaft_is_synchronized);
	assertEquals(0, state.current_index);

	processTriggerEvent(&state, shape, &ec.triggerConfig, SHAFT_PRIMARY_DOWN, now++);
	assertTrueM("shaft_is_synchronized", state.shaft_is_synchronized);
	assertEquals(1, state.current_index);
}

void testTriggerDecoder(void) {
	printf("*************************************************** testTriggerDecoder\r\n");

	engine_configuration2_s ec2;

	initializeSkippedToothTriggerShapeExt(&ec2, 2, 0);
	assertEquals(ec2.triggerShape.size, 4);
	assertEquals(ec2.triggerShape.wave.switchTimes[0], 0.25);
	assertEquals(ec2.triggerShape.wave.switchTimes[1], 0.5);
	assertEquals(ec2.triggerShape.wave.switchTimes[2], 0.75);
	assertEquals(ec2.triggerShape.wave.switchTimes[3], 1);

	testDodgeNeonDecoder();
	testFordAspire();
	test1995FordInline6TriggerDecoder();
	testMazdaMianaNbDecoder();
	testGY6_139QMB();

	testMazda323();
}

