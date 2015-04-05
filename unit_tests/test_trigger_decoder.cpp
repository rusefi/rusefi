/**
 * @file	test_trigger_decoder.cpp
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
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
#include "trigger_mazda.h"
#include "trigger_chrysler.h"

#include "trigger_central.h"
#include "main_trigger_callback.h"
#include "engine.h"
#include "advance_map.h"
#include "engine_test_helper.h"
#include "speed_density.h"

extern int timeNow;

extern bool printGapRatio;
extern float actualSynchGap;

extern "C" {
void sendOutConfirmation(char *value, int i);
}

void sendOutConfirmation(char *value, int i) {
	// test implementation
}

int getTheAngle(engine_type_e engineType) {
	EngineTestHelper eth(engineType);
	EXPAND_EngineTestHelper;

	initDataStructures(PASS_ENGINE_PARAMETER_F);

	TriggerShape * shape = &eth.engine.triggerShape;
	return findTriggerZeroEventIndex(shape, &engineConfiguration->trigger PASS_ENGINE_PARAMETER);
}

static void testDodgeNeonDecoder(void) {
	printf("*************************************************** testDodgeNeonDecoder\r\n");
	initTriggerDecoder();

	assertEqualsM("trigger zero index", 8, getTheAngle(DODGE_NEON_1995));

	EngineTestHelper eth(DODGE_NEON_1995);
	EXPAND_EngineTestHelper;

	engine_configuration_s *ec = eth.ec;
	TriggerShape * shape = &eth.engine.triggerShape;
	assertEquals(8, shape->getTriggerShapeSynchPointIndex());

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
	EXPAND_EngineTestHelper;

	TriggerShape * shape = &eth.engine.triggerShape;

	assertEqualsM("triggerShapeSynchPointIndex", 0, shape->getTriggerShapeSynchPointIndex());

	event_trigger_position_s position;
	assertEqualsM("globalTriggerAngleOffset", 0, engineConfiguration->globalTriggerAngleOffset);
	findTriggerPosition(&position, 0 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 0, 0);

	findTriggerPosition(&position, 200 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 3, 20);

	findTriggerPosition(&position, 360 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 6, 0);


	IgnitionEventList *ecl = &eth.ec2.ignitionEvents[0];
	assertEqualsM("ignition events size", 6, ecl->size);
	assertEqualsM("event index", 0, ecl->elements[0].dwellPosition.eventIndex);
	assertEquals(0, ecl->elements[0].dwellPosition.angleOffset);

	assertEqualsM("event index", 10, ecl->elements[5].dwellPosition.eventIndex);
	assertEquals(0, ecl->elements[5].dwellPosition.angleOffset);

	TriggerState state;

	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized);
	int r = 10;
	state.decodeTriggerEvent(SHAFT_PRIMARY_DOWN, r PASS_ENGINE_PARAMETER);
	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization
	state.decodeTriggerEvent(SHAFT_PRIMARY_UP, ++r PASS_ENGINE_PARAMETER);
	assertTrue(state.shaft_is_synchronized); // first signal rise synchronize
	assertEquals(0, state.getCurrentIndex());
	state.decodeTriggerEvent(SHAFT_PRIMARY_DOWN, r++ PASS_ENGINE_PARAMETER);
	assertEquals(1, state.getCurrentIndex());

	for (int i = 2; i < 10;) {
		state.decodeTriggerEvent(SHAFT_PRIMARY_UP, r++ PASS_ENGINE_PARAMETER);
		assertEqualsM("even", i++, state.getCurrentIndex());
		state.decodeTriggerEvent(SHAFT_PRIMARY_DOWN, r++ PASS_ENGINE_PARAMETER);
		assertEqualsM("odd", i++, state.getCurrentIndex());
	}

	state.decodeTriggerEvent(SHAFT_PRIMARY_UP, r++ PASS_ENGINE_PARAMETER);
	assertEquals(10, state.getCurrentIndex());

	state.decodeTriggerEvent(SHAFT_PRIMARY_DOWN, r++ PASS_ENGINE_PARAMETER);
	assertEquals(11, state.getCurrentIndex());

	state.decodeTriggerEvent(SHAFT_PRIMARY_UP, r++ PASS_ENGINE_PARAMETER);
	assertEquals(0, state.getCurrentIndex()); // new revolution

	assertEqualsM("running dwell", 0.5, getSparkDwellMsT(2000 PASS_ENGINE_PARAMETER));
}

void testFordAspire(void) {
	printf("*************************************************** testFordAspire\r\n");

	assertEquals(4, getTheAngle(FORD_ASPIRE_1996));

	EngineTestHelper eth(FORD_ASPIRE_1996);
	EXPAND_EngineTestHelper;

	assertEquals(4, eth.engine.triggerShape.getTriggerShapeSynchPointIndex());

	assertEquals(800, engineConfiguration->fuelRpmBins[0]);
	assertEquals(7000, engineConfiguration->fuelRpmBins[15]);

	engineConfiguration->crankingChargeAngle = 65;
	engineConfiguration->crankingTimingAngle = 31;

	assertEqualsM("cranking dwell", 54.166670, getSparkDwellMsT(200 PASS_ENGINE_PARAMETER));
	assertEqualsM("running dwell", 4, getSparkDwellMsT(2000 PASS_ENGINE_PARAMETER));

	assertEqualsM("higher rpm dwell", 3.25, getSparkDwellMsT(6000 PASS_ENGINE_PARAMETER));
}

void testMazda323(void) {
	printf("*************************************************** testMazda323\r\n");

	EngineTestHelper eth(MAZDA_323);
	assertEquals(0, eth.engine.triggerShape.getTriggerShapeSynchPointIndex());
}

void testMazdaMianaNbDecoder(void) {
	printf("*************************************************** testMazdaMianaNbDecoder\r\n");

	EngineTestHelper eth(MAZDA_MIATA_NB);
	EXPAND_EngineTestHelper;

	engine_configuration_s *ec = eth.ec;
	TriggerShape * shape = &eth.engine.triggerShape;
	assertEquals(11, shape->getTriggerShapeSynchPointIndex());

	TriggerState state;

	int a = 0;
	state.decodeTriggerEvent(SHAFT_PRIMARY_DOWN, a + 20 PASS_ENGINE_PARAMETER);
	assertFalseM("0a shaft_is_synchronized", state.shaft_is_synchronized);
	state.decodeTriggerEvent(SHAFT_PRIMARY_UP, a + 340 PASS_ENGINE_PARAMETER);
	assertFalseM("0b shaft_is_synchronized", state.shaft_is_synchronized);

	state.decodeTriggerEvent(SHAFT_PRIMARY_DOWN, a + 360 PASS_ENGINE_PARAMETER);
	assertFalseM("0c shaft_is_synchronized", state.shaft_is_synchronized);
	state.decodeTriggerEvent(SHAFT_PRIMARY_UP, a + 380 PASS_ENGINE_PARAMETER);
	assertFalseM("0d shaft_is_synchronized", state.shaft_is_synchronized);
	state.decodeTriggerEvent(SHAFT_PRIMARY_DOWN, a + 400 PASS_ENGINE_PARAMETER);
	assertTrueM("0e shaft_is_synchronized", state.shaft_is_synchronized);

	state.decodeTriggerEvent(SHAFT_PRIMARY_UP, a + 720 PASS_ENGINE_PARAMETER);
	assertTrueM("0f shaft_is_synchronized", state.shaft_is_synchronized);

	a = 720;
	state.decodeTriggerEvent(SHAFT_PRIMARY_DOWN, a + 20 PASS_ENGINE_PARAMETER);
	assertTrueM("1a shaft_is_synchronized", state.shaft_is_synchronized);
	state.decodeTriggerEvent(SHAFT_PRIMARY_UP, a + 340 PASS_ENGINE_PARAMETER);
	assertTrueM("1b shaft_is_synchronized", state.shaft_is_synchronized);

	state.decodeTriggerEvent(SHAFT_PRIMARY_DOWN, a + 360 PASS_ENGINE_PARAMETER);
	assertTrueM("1c shaft_is_synchronized", state.shaft_is_synchronized);
	state.decodeTriggerEvent(SHAFT_PRIMARY_UP, a + 380 PASS_ENGINE_PARAMETER);
	assertTrueM("1d shaft_is_synchronized", state.shaft_is_synchronized);
	assertEquals(5, state.getCurrentIndex());

	state.decodeTriggerEvent(SHAFT_PRIMARY_DOWN, a + 400 PASS_ENGINE_PARAMETER);
	assertTrueM("1e shaft_is_synchronized", state.shaft_is_synchronized);
	assertEquals(0, state.getCurrentIndex());

	state.decodeTriggerEvent(SHAFT_PRIMARY_UP, a + 720 PASS_ENGINE_PARAMETER);
	assertTrueM("1f shaft_is_synchronized", state.shaft_is_synchronized);

	event_trigger_position_s position;
	assertEqualsM("globalTriggerAngleOffset", 276, ec->globalTriggerAngleOffset);
	findTriggerPosition(&position, 0 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 7, 46);

	findTriggerPosition(&position, 180 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 13, 46);

	findTriggerPosition(&position, 360 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 17, 46);

	findTriggerPosition(&position, 444 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 0, 0);

	findTriggerPosition(&position, 444.1 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 0, 0.1);

	findTriggerPosition(&position, 445 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 0, 1);

	findTriggerPosition(&position, 494 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 3, 0);

	findTriggerPosition(&position, 719 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 7, 45);

	ec->globalTriggerAngleOffset = 0;
	findTriggerPosition(&position, 0 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 0, 0);

	ec->globalTriggerAngleOffset = 10;
	findTriggerPosition(&position, 0 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 0, 10);

	findTriggerPosition(&position, -10 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 0, 0);
}

static void testTriggerDecoder2(const char *msg, engine_type_e type, int synchPointIndex, float channel1duty, float channel2duty) {
	printf("*************************************************** %s\r\n", msg);

	EngineTestHelper eth(type);
	EXPAND_EngineTestHelper;

	engine_configuration_s *ec = eth.ec;

	initSpeedDensity(PASS_ENGINE_PARAMETER_F);

	TriggerShape *t = &eth.engine.triggerShape;

	assertEqualsM("synchPointIndex", synchPointIndex, t->getTriggerShapeSynchPointIndex());

	assertEqualsM("channel1duty", channel1duty, t->dutyCycle[0]);
	assertEqualsM("channel2duty", channel2duty, t->dutyCycle[1]);
}

static void testTriggerDecoder3(const char *msg, engine_type_e type, int synchPointIndex, float channel1duty, float channel2duty, float expectedGap) {
	printGapRatio = true;
	testTriggerDecoder2(msg, type, synchPointIndex, channel1duty, channel2duty);
	assertEqualsM2("actual gap ratio", expectedGap, actualSynchGap, 0.001);
	printGapRatio = false;
}

extern EventQueue schedulingQueue;

extern int mockTps;

static void testStartupFuelPumping(void) {
	EngineTestHelper eth(FORD_INLINE_6_1995);
	EXPAND_EngineTestHelper;

	StartupFuelPumping sf;

	engine->rpmCalculator.mockRpm = 0;

	engine->engineConfiguration->tpsMin = 0;
	engine->engineConfiguration->tpsMax = 10;

	mockTps = 6;
	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#1", 1, sf.pumpsCounter);

	mockTps = 3;
	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#2", 1, sf.pumpsCounter);

	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#3", 1, sf.pumpsCounter);

	engine->rpmCalculator.mockRpm = 10;
	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#4", 0, sf.pumpsCounter);

	mockTps = 7;
	engine->rpmCalculator.mockRpm = 0;
	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#5", 1, sf.pumpsCounter);

	mockTps = 3;
	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#6", 1, sf.pumpsCounter);

	mockTps = 7;
	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#7", 2, sf.pumpsCounter);
}

static void assertREquals(void *expected, void *actual) {
	assertEquals((float)(uint64_t)expected, (float)(uint64_t)actual);
}

extern engine_pins_s enginePins;

static void testRpmCalculator(void) {
	printf("*************************************************** testRpmCalculator\r\n");

	EngineTestHelper eth(FORD_INLINE_6_1995);
	EXPAND_EngineTestHelper;

	assertEquals(720, eth.engine.engineConfiguration->engineCycle);
	assertEquals(720, eth.ec->engineCycle);

	efiAssertVoid(eth.engine.engineConfiguration!=NULL, "null config in engine");

	initThermistors(NULL PASS_ENGINE_PARAMETER);
	engine->updateSlowSensors();

	engineConfiguration->trigger.customTotalToothCount = 8;
	engineConfiguration->globalFuelCorrection = 3;
	eth.initTriggerShapeAndRpmCalculator();

	// this is a very dirty and sad hack.  todo: eliminate
//	engine.engineConfiguration = eth.engine.engineConfiguration;
	eth.engine.engineConfiguration->injector.lag = 0.0;

	engine->updateSlowSensors();
	timeNow = 0;
	assertEquals(0, eth.engine.rpmCalculator.rpm(PASS_ENGINE_PARAMETER_F));

	eth.fireTriggerEvents();
	assertEqualsM("RPM", 1500, eth.engine.rpmCalculator.rpm(PASS_ENGINE_PARAMETER_F));
	assertEqualsM("index #1", 15, eth.triggerCentral.triggerState.getCurrentIndex());

	static MainTriggerCallback triggerCallbackInstance;
	triggerCallbackInstance.init(&eth.engine);
	eth.triggerCentral.addEventListener(mainTriggerCallback, "main loop", &eth.engine);

//	engine.rpmCalculator = &eth.rpmState;
	prepareTimingMap(PASS_ENGINE_PARAMETER_F);

	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_UP PASS_ENGINE_PARAMETER);
	assertEqualsM("index #2", 0, eth.triggerCentral.triggerState.getCurrentIndex());
	assertEqualsM("queue size", 6, schedulingQueue.size());
	scheduling_s *ev1 = schedulingQueue.getForUnitText(0);
	assertREquals((void*)ev1->callback, (void*)turnPinLow);
	assertREquals((void*)&enginePins.coils[0], ev1->param);

	assertEqualsM("ev 1", 246444, ev1->momentX);
	assertEqualsM("ev 2", 245944, schedulingQueue.getForUnitText(1)->momentX);
	schedulingQueue.clear();

	timeNow += 5000;
	eth.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_DOWN PASS_ENGINE_PARAMETER);
	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_UP PASS_ENGINE_PARAMETER);
	timeNow += 5000;
	eth.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_DOWN PASS_ENGINE_PARAMETER);
	assertEqualsM("index #3", 3, eth.triggerCentral.triggerState.getCurrentIndex());
	assertEqualsM("queue size 3", 6, schedulingQueue.size());
	assertEqualsM("ev 3", 259777, schedulingQueue.getForUnitText(0)->momentX);
	assertEquals(259277, schedulingQueue.getForUnitText(1)->momentX);
	assertEqualsM2("ev 5", 261362, schedulingQueue.getForUnitText(2)->momentX, 2);
	assertEqualsM("3/3", 258333, schedulingQueue.getForUnitText(3)->momentX);
	schedulingQueue.clear();

	timeNow += 5000;
	eth.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_DOWN PASS_ENGINE_PARAMETER);
	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_UP PASS_ENGINE_PARAMETER);
	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_UP PASS_ENGINE_PARAMETER);
	assertEqualsM("index #4", 6, eth.triggerCentral.triggerState.getCurrentIndex());
	assertEqualsM("queue size 4", 6, schedulingQueue.size());
	assertEqualsM("4/0", 273111, schedulingQueue.getForUnitText(0)->momentX);
	schedulingQueue.clear();

	timeNow += 5000;
	eth.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_DOWN PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 5", 0, schedulingQueue.size());
//	assertEqualsM("5/1", 284500, schedulingQueue.getForUnitText(0)->momentUs);
	schedulingQueue.clear();

	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_UP PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 6", 6, schedulingQueue.size());
	assertEqualsM("6/0", 286444, schedulingQueue.getForUnitText(0)->momentX);
	assertEqualsM("6/1", 285944, schedulingQueue.getForUnitText(1)->momentX);
	assertEqualsM2("6/2", 288029, schedulingQueue.getForUnitText(2)->momentX, 1);
	schedulingQueue.clear();

	timeNow += 5000;
	eth.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_DOWN PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 7", 0, schedulingQueue.size());
	schedulingQueue.clear();

	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_UP PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 8", 6, schedulingQueue.size());
	assertEqualsM("8/0", 299777, schedulingQueue.getForUnitText(0)->momentX);
	assertEqualsM("8/1", 299277, schedulingQueue.getForUnitText(1)->momentX);
	assertEqualsM2("8/2", 301362, schedulingQueue.getForUnitText(2)->momentX, 1);
	assertEqualsM("8/3", 298333, schedulingQueue.getForUnitText(3)->momentX);
	schedulingQueue.clear();

	timeNow += 5000;
	eth.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_DOWN PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 9", 0, schedulingQueue.size());
	schedulingQueue.clear();

	timeNow += 5000; // 5ms
	eth.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_UP PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 10", 0, schedulingQueue.size());
	schedulingQueue.clear();
}

void testTriggerDecoder(void) {
	printf("*************************************************** testTriggerDecoder\r\n");

	persistent_config_s config;
	Engine engine(&config);
	TriggerShape * s = &engine.triggerShape;


	initializeSkippedToothTriggerShapeExt(s, 2, 0, FOUR_STROKE_CAM_SENSOR);
	assertEqualsM("shape size", s->getSize(), 4);
	assertEquals(s->wave.switchTimes[0], 0.25);
	assertEquals(s->wave.switchTimes[1], 0.5);
	assertEquals(s->wave.switchTimes[2], 0.75);
	assertEquals(s->wave.switchTimes[3], 1);

	testDodgeNeonDecoder();
	testTriggerDecoder2("dodge neon", DODGE_NEON_1995, 8, 0.4931, 0.2070);

	testFordAspire();
	// lame duty cycle implementation!
	testTriggerDecoder2("ford aspire", FORD_ASPIRE_1996, 4, 0.5, 0.3841);

	test1995FordInline6TriggerDecoder();
	testMazdaMianaNbDecoder();
	testTriggerDecoder2("testGY6_139QMB", GY6_139QMB, 0, 0.4375, 0.0);
	testTriggerDecoder2("testSubary", SUBARU_2003_WRX, 0, 0.4000, 0.0);

	testTriggerDecoder2("testFordEscortGt", FORD_ESCORT_GT, 0, 0.8096, 0.3844);
	testTriggerDecoder2("testMiniCooper", MINI_COOPER_R50, 121, 0.5222, 0.4959);
	testTriggerDecoder2("testRoverV8", ROVER_V8, 0, 0.4861, 0);

	testTriggerDecoder2("test1+1", CUSTOM_ENGINE, 0, 0.7500, 0.2500);

	testTriggerDecoder2("testCitroen", CITROEN_TU3JP, 0, 0.4833, 0);
	testTriggerDecoder2("testAccordCd 3w", HONDA_ACCORD_CD, 12, 0.8146, 0.5000);
	testTriggerDecoder2("testAccordCd 2w", HONDA_ACCORD_CD_TWO_WIRES, 10, 0.8146, 0.5);
	testTriggerDecoder2("testAccordCdDip", HONDA_ACCORD_CD_DIP, 27, 0.5000, 0.5000);

	testTriggerDecoder2("testMitsu", MITSU_4G93, 3, 0.3750, 0.3889);
//	testTriggerDecoder2("miata 1990", MIATA_1990, 0, 0.6280, 0.0);
	testTriggerDecoder3("miata 1994", MIATA_1994_DEVIATOR, 11, 0.2985, 0.3890, MIATA_NA_GAP);
	testTriggerDecoder3("citroen", CITROEN_TU3JP, 0, 0.4833, 0.0, 2.9994);

	testTriggerDecoder3("neon NGC", DODGE_NEON_2003, 0, 0.4786, 0.0, CHRYSLER_NGC_GAP);
	testTriggerDecoder2("sachs", SACHS, 0, 0.4800, 0.000);

	testMazda323();

	testRpmCalculator();
	testStartupFuelPumping();
}
