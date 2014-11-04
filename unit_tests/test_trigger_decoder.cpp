/**
 * @file	test_trigger_decoder.cpp
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#include "main.h"
#include "test_trigger_decoder.h"
#include "trigger_decoder.h"
#include "engine_math.h"
#include "thermistors.h"

#include "ford_aspire.h"
#include "dodge_neon.h"
#include "ford_1995_inline_6.h"
#include "mazda_323.h"
#include "rpm_calculator.h"
#include "event_queue.h"
#include "algo.h"

#include "trigger_central.h"
#include "main_trigger_callback.h"
#include "engine.h"
#include "advance_map.h"
#include "engine_test_helper.h"
#include "speed_density.h"

Engine engine;

extern int timeNow;

extern bool printGapRatio;

extern "C" {
void sendOutConfirmation(char *value, int i);
}

void sendOutConfirmation(char *value, int i) {
	// test implementation
}

int getTheAngle(engine_type_e engineType) {
	EngineTestHelper eth(engineType);

	engine_configuration_s *ec = eth.ec;


	initDataStructures(ec);
	resetConfigurationExt(NULL, engineType, &eth.engine);

	trigger_shape_s * shape = &eth.ec2.triggerShape;
	return findTriggerZeroEventIndex(shape, &ec->triggerConfig);
}

static void testDodgeNeonDecoder(void) {
	printf("*************************************************** testDodgeNeonDecoder\r\n");
	initTriggerDecoder();

	assertEqualsM("trigger zero index", 8, getTheAngle(DODGE_NEON_1995));

	EngineTestHelper eth(DODGE_NEON_1995);

	engine_configuration_s *ec = eth.ec;

	resetConfigurationExt(NULL, DODGE_NEON_1995, &eth.engine);
	assertEquals(8, eth.ec2.triggerShape.getTriggerShapeSynchPointIndex());

	trigger_shape_s * shape = &eth.ec2.triggerShape;
	TriggerState state;

	assertFalseM("1 shaft_is_synchronized", state.shaft_is_synchronized);

	int r = 0;
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, r + 60);
//	assertFalseM("2 shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization

//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, r + 210);
//	assertFalseM("3 shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization
//
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, r + 420);
//	assertFalseM("4 shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization
//
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, r + 630);
//	assertFalse(state.shaft_is_synchronized); // still no synchronization
//
//	printf("2nd camshaft revolution\r\n");
//	r = 720;
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, r + 60);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, r + 210);
//	assertTrue(state.shaft_is_synchronized);
//	assertEquals(0, state.current_index);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, r + 420);
//	assertEquals(1, state.current_index);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, r + 630);
//	assertEquals(2, state.current_index);
//
//	printf("3rd camshaft revolution\r\n");
//	r = 2 * 720;
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, r + 60);
//	assertEqualsM("current index", 3, state.current_index);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, r + 210);
//	assertTrue(state.shaft_is_synchronized);
//	assertEqualsM("current index", 0, state.current_index);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, r + 420);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, r + 630);
}

static void assertTriggerPosition(event_trigger_position_s *position, int eventIndex, float angleOffset) {
	assertEqualsM("eventIndex", eventIndex, position->eventIndex);
	assertEqualsM("angleOffset", angleOffset, position->angleOffset);
}

static void test1995FordInline6TriggerDecoder(void) {
	printf("*************************************************** test1995FordInline6TriggerDecoder\r\n");

	assertEqualsM("triggerIndex ", 0, getTheAngle(FORD_INLINE_6_1995));

	initTriggerDecoder();

	EngineTestHelper eth(FORD_INLINE_6_1995);

	engine_configuration_s *ec = eth.ec;

	resetConfigurationExt(NULL, FORD_INLINE_6_1995, &eth.engine);
	assertEqualsM("triggerShapeSynchPointIndex", 0, eth.ec2.triggerShape.getTriggerShapeSynchPointIndex());

	trigger_shape_s * shape = &eth.ec2.triggerShape;
	event_trigger_position_s position;
	assertEqualsM("globalTriggerAngleOffset", 0, ec->globalTriggerAngleOffset);
	findTriggerPosition(ec, shape, &position, 0);
	assertTriggerPosition(&position, 0, 0);

	findTriggerPosition(ec, shape, &position, 200);
	assertTriggerPosition(&position, 3, 20);

	findTriggerPosition(ec, shape, &position, 360);
	assertTriggerPosition(&position, 6, 0);


	IgnitionEventList *ecl = &eth.ec2.engineEventConfiguration.ignitionEvents[0];
	assertEqualsM("ignition events size", 6, ecl->size);
	assertEqualsM("event index", 0, ecl->events[0].dwellPosition.eventIndex);
	assertEquals(0, ecl->events[0].dwellPosition.angleOffset);

	assertEqualsM("event index", 10, ecl->events[5].dwellPosition.eventIndex);
	assertEquals(0, ecl->events[5].dwellPosition.angleOffset);

	TriggerState state;

	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized);
	int r = 10;
	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, r);
	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization
	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, ++r);
	assertTrue(state.shaft_is_synchronized); // first signal rise synchronize
	assertEquals(0, state.getCurrentIndex());
	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, r++);
	assertEquals(1, state.getCurrentIndex());

	for (int i = 2; i < 10;) {
		state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, r++);
		assertEqualsM("even", i++, state.getCurrentIndex());
		state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, r++);
		assertEqualsM("odd", i++, state.getCurrentIndex());
	}

	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, r++);
	assertEquals(10, state.getCurrentIndex());

	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, r++);
	assertEquals(11, state.getCurrentIndex());

	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, r++);
	assertEquals(0, state.getCurrentIndex()); // new revolution

	assertEqualsM("running dwell", 0.5, getSparkDwellMsT(ec, 2000));
}

void testFordAspire(void) {
	printf("*************************************************** testFordAspire\r\n");

	assertEquals(4, getTheAngle(FORD_ASPIRE_1996));

	EngineTestHelper eth(FORD_ASPIRE_1996);

	engine_configuration_s *ec = eth.ec;
	resetConfigurationExt(NULL, FORD_ASPIRE_1996, &eth.engine);
	assertEquals(4, eth.ec2.triggerShape.getTriggerShapeSynchPointIndex());

	assertEquals(800, ec->fuelRpmBins[0]);
	assertEquals(7000, ec->fuelRpmBins[15]);

	ec->crankingChargeAngle = 65;
	ec->crankingTimingAngle = 31;

	assertEqualsM("cranking dwell", 54.166670, getSparkDwellMsT(ec, 200));
	assertEqualsM("running dwell", 4, getSparkDwellMsT(ec, 2000));

	assertEqualsM("higher rpm dwell", 3.25, getSparkDwellMsT(ec, 6000));
}

void testMazda323(void) {
	printf("*************************************************** testMazda323\r\n");

	EngineTestHelper eth(MAZDA_323);
	persistent_config_s persistentConfig;
	engine_configuration_s *ec = eth.ec;
	resetConfigurationExt(NULL, MAZDA_323, &eth.engine);
	assertEquals(0, eth.ec2.triggerShape.getTriggerShapeSynchPointIndex());
}

void testMazdaMianaNbDecoder(void) {
	printf("*************************************************** testMazdaMianaNbDecoder\r\n");

	EngineTestHelper eth(MAZDA_MIATA_NB);
	engine_configuration_s *ec = eth.ec;
	resetConfigurationExt(NULL, MAZDA_MIATA_NB, &eth.engine);
	assertEquals(11, eth.ec2.triggerShape.getTriggerShapeSynchPointIndex());

	TriggerState state;
	trigger_shape_s * shape = &eth.ec2.triggerShape;

	int a = 0;
	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, a + 20);
	assertFalseM("0a shaft_is_synchronized", state.shaft_is_synchronized);
	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, a + 340);
	assertFalseM("0b shaft_is_synchronized", state.shaft_is_synchronized);

	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, a + 360);
	assertFalseM("0c shaft_is_synchronized", state.shaft_is_synchronized);
	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, a + 380);
	assertFalseM("0d shaft_is_synchronized", state.shaft_is_synchronized);
	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, a + 400);
	assertTrueM("0e shaft_is_synchronized", state.shaft_is_synchronized);

	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, a + 720);
	assertTrueM("0f shaft_is_synchronized", state.shaft_is_synchronized);

	a = 720;
	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, a + 20);
	assertTrueM("1a shaft_is_synchronized", state.shaft_is_synchronized);
	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, a + 340);
	assertTrueM("1b shaft_is_synchronized", state.shaft_is_synchronized);

	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, a + 360);
	assertTrueM("1c shaft_is_synchronized", state.shaft_is_synchronized);
	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, a + 380);
	assertTrueM("1d shaft_is_synchronized", state.shaft_is_synchronized);
	assertEquals(5, state.getCurrentIndex());

	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, a + 400);
	assertTrueM("1e shaft_is_synchronized", state.shaft_is_synchronized);
	assertEquals(0, state.getCurrentIndex());

	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, a + 720);
	assertTrueM("1f shaft_is_synchronized", state.shaft_is_synchronized);

	event_trigger_position_s position;
	assertEqualsM("globalTriggerAngleOffset", 276, ec->globalTriggerAngleOffset);
	findTriggerPosition(ec, shape, &position, 0);
	assertTriggerPosition(&position, 7, 46);

	findTriggerPosition(ec, shape, &position, 180);
	assertTriggerPosition(&position, 13, 46);

	findTriggerPosition(ec, shape, &position, 360);
	assertTriggerPosition(&position, 17, 46);

	findTriggerPosition(ec, shape, &position, 444);
	assertTriggerPosition(&position, 0, 0);

	findTriggerPosition(ec, shape, &position, 444.1);
	assertTriggerPosition(&position, 0, 0.1);

	findTriggerPosition(ec, shape, &position, 445);
	assertTriggerPosition(&position, 0, 1);

	findTriggerPosition(ec, shape, &position, 494);
	assertTriggerPosition(&position, 3, 0);

	findTriggerPosition(ec, shape, &position, 719);
	assertTriggerPosition(&position, 7, 45);

	ec->globalTriggerAngleOffset = 0;
	findTriggerPosition(ec, shape, &position, 0);
	assertTriggerPosition(&position, 0, 0);

	ec->globalTriggerAngleOffset = 10;
	findTriggerPosition(ec, shape, &position, 0);
	assertTriggerPosition(&position, 0, 10);

	findTriggerPosition(ec, shape, &position, -10);
	assertTriggerPosition(&position, 0, 0);
}

static void testTriggerDecoder2(const char *msg, engine_type_e type, int synchPointIndex, float channel1duty, float channel2duty) {
	printf("*************************************************** %s\r\n", msg);

	EngineTestHelper eth(type);
	engine_configuration_s *ec = eth.ec;

	assertEquals(0, eth.ec2.triggerShape.getTriggerShapeSynchPointIndex());

	initSpeedDensity(ec);
	resetConfigurationExt(NULL, type, &eth.engine);

	assertEqualsM("synchPointIndex", synchPointIndex, eth.ec2.triggerShape.getTriggerShapeSynchPointIndex());

	assertEqualsM("channel1duty", channel1duty, eth.ec2.triggerShape.dutyCycle[0]);
	assertEqualsM("channel2duty", channel2duty, eth.ec2.triggerShape.dutyCycle[1]);
}

void testGY6_139QMB(void) {
	printf("*************************************************** testGY6_139QMB\r\n");

	EngineTestHelper eth(GY6_139QMB);
	engine_configuration_s *ec = eth.ec;
	resetConfigurationExt(NULL, GY6_139QMB, &eth.engine);

	TriggerState state;
	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized);

	trigger_shape_s * shape = &eth.ec2.triggerShape;

	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized);
	assertEquals(0, state.getCurrentIndex());

	int now = 0;
	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_UP, now++);
	assertTrueM("shaft_is_synchronized", state.shaft_is_synchronized);
	assertEquals(0, state.getCurrentIndex());

	state.decodeTriggerEvent(shape, &ec->triggerConfig, SHAFT_PRIMARY_DOWN, now++);
	assertTrueM("shaft_is_synchronized", state.shaft_is_synchronized);
	assertEquals(1, state.getCurrentIndex());
}

extern EventQueue schedulingQueue;

// this is a very dirty and sad hack. todo: eliminate
extern Engine engine;

extern int mockTps;

static void testStartupFuelPumping(void) {
	EngineTestHelper eth(FORD_INLINE_6_1995);

	StartupFuelPumping sf;

	Engine * engine = &eth.engine;
	RpmCalculator rc;
	engine->rpmCalculator = &rc;

	engine->rpmCalculator->mockRpm = 0;

	engine->engineConfiguration->tpsMin = 0;
	engine->engineConfiguration->tpsMax = 10;

	mockTps = 6;
	sf.update(engine);
	assertEqualsM("pc#1", 1, sf.pumpsCounter);

	mockTps = 3;
	sf.update(engine);
	assertEqualsM("pc#2", 1, sf.pumpsCounter);

	sf.update(engine);
	assertEqualsM("pc#3", 1, sf.pumpsCounter);

	engine->rpmCalculator->mockRpm = 10;
	sf.update(engine);
	assertEqualsM("pc#4", 0, sf.pumpsCounter);

	mockTps = 7;
	engine->rpmCalculator->mockRpm = 0;
	sf.update(engine);
	assertEqualsM("pc#5", 1, sf.pumpsCounter);

	mockTps = 3;
	sf.update(engine);
	assertEqualsM("pc#6", 1, sf.pumpsCounter);

	mockTps = 7;
	sf.update(engine);
	assertEqualsM("pc#7", 2, sf.pumpsCounter);
}

static void testRpmCalculator(void) {
	printf("*************************************************** testRpmCalculator\r\n");

	EngineTestHelper eth(FORD_INLINE_6_1995);

	efiAssertVoid(eth.engine.engineConfiguration!=NULL, "null config in engine");

	initThermistors(&eth.engine);

	engine_configuration_s *ec = &eth.persistentConfig.engineConfiguration;

	engine_configuration2_s *ec2 = &eth.ec2;

	ec->triggerConfig.customTotalToothCount = 8;
	ec->globalFuelCorrection = 3;
	eth.initTriggerShapeAndRpmCalculator();

	// this is a very dirty and sad hack.  todo: eliminate
	engine.engineConfiguration = eth.engine.engineConfiguration;
	engine.engineConfiguration->injectorLag = 0.0;


	configuration_s configuration = { ec, ec2 };
	timeNow = 0;
	assertEquals(0, eth.rpmState.rpm());

	eth.fireTriggerEvents();
	assertEqualsM("RPM", 1500, eth.rpmState.rpm());

	assertEqualsM("index #1", 15, eth.triggerCentral.triggerState.getCurrentIndex());


	static MainTriggerCallback triggerCallbackInstance;
	triggerCallbackInstance.init(&eth.engine, ec2);
	eth.triggerCentral.addEventListener((ShaftPositionListener)&onTriggerEvent, "main loop", &triggerCallbackInstance);

	engine.rpmCalculator = &eth.rpmState;
	prepareTimingMap();

	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(&eth.engine, SHAFT_PRIMARY_UP, timeNow);
	assertEqualsM("index #2", 0, eth.triggerCentral.triggerState.getCurrentIndex());
	assertEqualsM("queue size", 4, schedulingQueue.size());
	assertEqualsM("ev 1", 248000, schedulingQueue.getForUnitText(0)->momentUs);
	assertEqualsM("ev 2", 245000, schedulingQueue.getForUnitText(1)->momentUs);
	schedulingQueue.clear();

	timeNow += 5000;
	eth.triggerCentral.handleShaftSignal(&eth.engine, SHAFT_PRIMARY_DOWN, timeNow);
	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(&eth.engine, SHAFT_PRIMARY_UP, timeNow);
	timeNow += 5000;
	eth.triggerCentral.handleShaftSignal(&eth.engine, SHAFT_PRIMARY_DOWN, timeNow);
	assertEqualsM("index #3", 3, eth.triggerCentral.triggerState.getCurrentIndex());
	assertEqualsM("queue size 3", 6, schedulingQueue.size());
	assertEquals(258333, schedulingQueue.getForUnitText(0)->momentUs);
	assertEquals(257833, schedulingQueue.getForUnitText(1)->momentUs);
	assertEqualsM("ev 5", 261333, schedulingQueue.getForUnitText(2)->momentUs);
	assertEqualsM("3/3", 258333, schedulingQueue.getForUnitText(3)->momentUs);
	schedulingQueue.clear();

	timeNow += 5000;
	eth.triggerCentral.handleShaftSignal(&eth.engine, SHAFT_PRIMARY_DOWN, timeNow);
	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(&eth.engine, SHAFT_PRIMARY_UP, timeNow);
	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(&eth.engine, SHAFT_PRIMARY_UP, timeNow);
	assertEqualsM("index #4", 6, eth.triggerCentral.triggerState.getCurrentIndex());
	assertEqualsM("queue size 4", 6, schedulingQueue.size());
	assertEqualsM("4/0", 271666, schedulingQueue.getForUnitText(0)->momentUs);
	schedulingQueue.clear();

	timeNow += 5000;
	eth.triggerCentral.handleShaftSignal(&eth.engine, SHAFT_PRIMARY_DOWN, timeNow);
	assertEqualsM("queue size 5", 1, schedulingQueue.size());
	assertEqualsM("5/1", 284500, schedulingQueue.getForUnitText(0)->momentUs);
	schedulingQueue.clear();

	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(&eth.engine, SHAFT_PRIMARY_UP, timeNow);
	assertEqualsM("queue size 6", 5, schedulingQueue.size());
	assertEqualsM("6/0", 285000, schedulingQueue.getForUnitText(0)->momentUs);
	assertEqualsM("6/1", 288000, schedulingQueue.getForUnitText(1)->momentUs);
	assertEqualsM("6/0", 285000, schedulingQueue.getForUnitText(2)->momentUs);
	schedulingQueue.clear();

	timeNow += 5000;
	eth.triggerCentral.handleShaftSignal(&eth.engine, SHAFT_PRIMARY_DOWN, timeNow);
	assertEqualsM("queue size 7", 0, schedulingQueue.size());
	schedulingQueue.clear();

	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(&eth.engine, SHAFT_PRIMARY_UP, timeNow);
	assertEqualsM("queue size 8", 6, schedulingQueue.size());
	assertEqualsM("8/0", 298333, schedulingQueue.getForUnitText(0)->momentUs);
	assertEqualsM("8/1", 297833, schedulingQueue.getForUnitText(1)->momentUs);
	assertEqualsM("8/2", 301333, schedulingQueue.getForUnitText(2)->momentUs);
	assertEqualsM("8/3", 298333, schedulingQueue.getForUnitText(3)->momentUs);
	schedulingQueue.clear();

	timeNow += 5000;
	eth.triggerCentral.handleShaftSignal(&eth.engine, SHAFT_PRIMARY_DOWN, timeNow);
	assertEqualsM("queue size 9", 0, schedulingQueue.size());
	schedulingQueue.clear();

	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(&eth.engine, SHAFT_PRIMARY_UP, timeNow);
	assertEqualsM("queue size 10", 0, schedulingQueue.size());
	schedulingQueue.clear();
}

void testTriggerDecoder(void) {
	printf("*************************************************** testTriggerDecoder\r\n");

	engine_configuration2_s ec2;

	initializeSkippedToothTriggerShapeExt(&ec2.triggerShape, 2, 0, FOUR_STROKE_CAM_SENSOR);
	assertEqualsM("shape size", ec2.triggerShape.getSize(), 4);
	assertEquals(ec2.triggerShape.wave.switchTimes[0], 0.25);
	assertEquals(ec2.triggerShape.wave.switchTimes[1], 0.5);
	assertEquals(ec2.triggerShape.wave.switchTimes[2], 0.75);
	assertEquals(ec2.triggerShape.wave.switchTimes[3], 1);

	testDodgeNeonDecoder();
	testTriggerDecoder2("dodge neon", DODGE_NEON_1995, 8, 0.4931, 0.2070);

	testFordAspire();
	// lame duty cycle implementation!
	testTriggerDecoder2("ford aspire", FORD_ASPIRE_1996, 4, 0.5, 0.3841);

	test1995FordInline6TriggerDecoder();
	testMazdaMianaNbDecoder();
	testGY6_139QMB();
	testTriggerDecoder2("testFordEscortGt", FORD_ESCORT_GT, 0, 0.6280, 0);
	testTriggerDecoder2("testMiniCooper", MINI_COOPER_R50, 121, 0.5222, 0.4959);
	testTriggerDecoder2("testRoverV8", ROVER_V8, 0, 0.4861, 0);
	testTriggerDecoder2("testCitroen", CITROEN_TU3JP, 0, 0.4833, 0);
	testTriggerDecoder2("testAccordCd 3w", HONDA_ACCORD_CD, 12, 0.8146, 0.5000);
	testTriggerDecoder2("testAccordCd 2w", HONDA_ACCORD_CD_TWO_WIRES, 10, 0.8146, 0.5);
	testTriggerDecoder2("testAccordCdDip", HONDA_ACCORD_CD_DIP, 27, 0.5000, 0.5000);

	testTriggerDecoder2("testMitsu", MITSU_4G93, 3, 0.3750, 0.3889);
//	testTriggerDecoder2("miata 1990", MIATA_1990, 0, 0.6280, 0.0);
	testTriggerDecoder2("miata 1994", MIATA_1994_DEVIATOR, 11, 0.2917, 0.3716);
	testTriggerDecoder2("citroen", CITROEN_TU3JP, 0, 0.4833, 0.0);

	printGapRatio = true;
	testTriggerDecoder2("neon NGC", DODGE_NEON_2003, 5, 0.4861, 0.0);
	printGapRatio = false;

	testMazda323();

	testRpmCalculator();
	testStartupFuelPumping();
}
