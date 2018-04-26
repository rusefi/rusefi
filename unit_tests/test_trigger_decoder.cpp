/**
 * @file	test_trigger_decoder.cpp
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "test_trigger_decoder.h"
#include "trigger_decoder.h"
#include "engine_math.h"
#include "allsensors.h"

#include "ford_aspire.h"
#include "dodge_neon.h"
#include "ford_1995_inline_6.h"
#include "mazda_323.h"
#include "rpm_calculator.h"
#include "event_queue.h"
#include "algo.h"
#include "trigger_mazda.h"
#include "trigger_chrysler.h"
#include "tps.h"

#include "trigger_central.h"
#include "main_trigger_callback.h"
#include "engine.h"
#include "advance_map.h"
#include "speed_density.h"
#include "fuel_math.h"
#include "spark_logic.h"
#include "trigger_universal.h"

extern int timeNowUs;
extern float unitTestValue;
extern float testMafValue;
extern int unitTestWarningCounter;
extern bool printTriggerDebug;
extern float actualSynchGap;

extern "C" {
void sendOutConfirmation(char *value, int i);
}

void sendOutConfirmation(char *value, int i) {
	// test implementation
}

static int getTriggerZeroEventIndex(engine_type_e engineType) {
	EngineTestHelper eth(engineType);
	EXPAND_EngineTestHelper;

	initDataStructures(PASS_ENGINE_PARAMETER_SIGNATURE);

	TriggerShape * shape = &eth.engine.triggerCentral.triggerShape;
	return findTriggerZeroEventIndex(&eth.engine.triggerCentral.triggerState, shape, &engineConfiguration->trigger PASS_ENGINE_PARAMETER_SUFFIX);
}

static void testDodgeNeonDecoder(void) {
	printf("*************************************************** testDodgeNeonDecoder95\r\n");
	initTriggerDecoder();

	assertEqualsM("DODGE_NEON_1995: trigger zero index", 8, getTriggerZeroEventIndex(DODGE_NEON_1995));

	EngineTestHelper eth(DODGE_NEON_1995);
	EXPAND_EngineTestHelper;

	TriggerShape * shape = &eth.engine.triggerCentral.triggerShape;
	assertEquals(8, shape->getTriggerShapeSynchPointIndex());

	TriggerState state;

	assertFalseM("1 shaft_is_synchronized", state.shaft_is_synchronized);

//	int r = 0;
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_RISING, r + 60);
//	assertFalseM("2 shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization

//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_FALLING, r + 210);
//	assertFalseM("3 shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization
//
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_RISING, r + 420);
//	assertFalseM("4 shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization
//
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_FALLING, r + 630);
//	assertFalse(state.shaft_is_synchronized); // still no synchronization
//
//	printf("2nd camshaft revolution\r\n");
//	r = 720;
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_RISING, r + 60);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_FALLING, r + 210);
//	assertTrue(state.shaft_is_synchronized);
//	assertEquals(0, state.current_index);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_RISING, r + 420);
//	assertEquals(1, state.current_index);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_FALLING, r + 630);
//	assertEquals(2, state.current_index);
//
//	printf("3rd camshaft revolution\r\n");
//	r = 2 * 720;
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_RISING, r + 60);
//	assertEqualsM("current index", 3, state.current_index);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_FALLING, r + 210);
//	assertTrue(state.shaft_is_synchronized);
//	assertEqualsM("current index", 0, state.current_index);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_RISING, r + 420);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_FALLING, r + 630);
}

static void assertTriggerPosition(event_trigger_position_s *position, int eventIndex, float angleOffset) {
	assertEqualsM("eventIndex", eventIndex, position->eventIndex);
	assertEqualsM("angleOffset", angleOffset, position->angleOffset);
}

void test1995FordInline6TriggerDecoder(void) {
	printf("*************************************************** test1995FordInline6TriggerDecoder\r\n");

	assertEqualsM("triggerIndex ", 0, getTriggerZeroEventIndex(FORD_INLINE_6_1995));

	initTriggerDecoder();

	EngineTestHelper eth(FORD_INLINE_6_1995);
	EXPAND_EngineTestHelper;

	TriggerShape * shape = &eth.engine.triggerCentral.triggerShape;

	assertEqualsM("triggerShapeSynchPointIndex", 0, shape->getTriggerShapeSynchPointIndex());

	// this is needed to have valid CLT and IAT. todo: extract method
	engine->updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);

	event_trigger_position_s position;
	assertEqualsM("globalTriggerAngleOffset", 0, engineConfiguration->globalTriggerAngleOffset);
	TRIGGER_SHAPE(findTriggerPosition(&position, 0 PASS_ENGINE_PARAMETER_SUFFIX));
	assertTriggerPosition(&position, 0, 0);

	TRIGGER_SHAPE(findTriggerPosition(&position, 200 PASS_ENGINE_PARAMETER_SUFFIX));
	assertTriggerPosition(&position, 3, 20);

	TRIGGER_SHAPE(findTriggerPosition(&position, 360 PASS_ENGINE_PARAMETER_SUFFIX));
	assertTriggerPosition(&position, 6, 0);

	eth.applyTriggerShape();

	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	eth.fireTriggerEvents(48);
	assertEquals(2000, eth.engine.rpmCalculator.rpmValue);
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	eth.fireTriggerEvents(48);

	IgnitionEventList *ecl = &eth.engine.ignitionEvents;
	assertEqualsM("ford inline ignition events size", 1, ecl->isReady);
	assertEqualsM("event index", 0, ecl->elements[0].dwellPosition.eventIndex);
	assertEqualsM("angle offset#1", 7, ecl->elements[0].dwellPosition.angleOffset);

	assertEqualsM("event index", 10, ecl->elements[5].dwellPosition.eventIndex);
	assertEqualsM("angle offset#2", 7, ecl->elements[5].dwellPosition.angleOffset);

	TriggerState state;

	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized);
	int r = 10;
	state.decodeTriggerEvent(SHAFT_PRIMARY_FALLING, r PASS_ENGINE_PARAMETER_SUFFIX);
	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization
	state.decodeTriggerEvent(SHAFT_PRIMARY_RISING, ++r PASS_ENGINE_PARAMETER_SUFFIX);
	assertTrue(state.shaft_is_synchronized); // first signal rise synchronize
	assertEquals(0, state.getCurrentIndex());
	state.decodeTriggerEvent(SHAFT_PRIMARY_FALLING, r++ PASS_ENGINE_PARAMETER_SUFFIX);
	assertEquals(1, state.getCurrentIndex());

	for (int i = 2; i < 10;) {
		state.decodeTriggerEvent(SHAFT_PRIMARY_RISING, r++ PASS_ENGINE_PARAMETER_SUFFIX);
		assertEqualsM("even", i++, state.getCurrentIndex());
		state.decodeTriggerEvent(SHAFT_PRIMARY_FALLING, r++ PASS_ENGINE_PARAMETER_SUFFIX);
		assertEqualsM("odd", i++, state.getCurrentIndex());
	}

	state.decodeTriggerEvent(SHAFT_PRIMARY_RISING, r++ PASS_ENGINE_PARAMETER_SUFFIX);
	assertEquals(10, state.getCurrentIndex());

	state.decodeTriggerEvent(SHAFT_PRIMARY_FALLING, r++ PASS_ENGINE_PARAMETER_SUFFIX);
	assertEquals(11, state.getCurrentIndex());

	state.decodeTriggerEvent(SHAFT_PRIMARY_RISING, r++ PASS_ENGINE_PARAMETER_SUFFIX);
	assertEquals(0, state.getCurrentIndex()); // new revolution

	assertEqualsM("running dwell", 0.5, getSparkDwell(2000 PASS_ENGINE_PARAMETER_SUFFIX));
}

void testFordAspire(void) {
	printf("*************************************************** testFordAspire\r\n");

	assertEqualsM("getTriggerZeroEventIndex", 4, getTriggerZeroEventIndex(FORD_ASPIRE_1996));

	EngineTestHelper eth(FORD_ASPIRE_1996);
	EXPAND_EngineTestHelper;

	assertEqualsM("getTriggerShapeSynchPointIndex", 4, TRIGGER_SHAPE(getTriggerShapeSynchPointIndex()));

	assertEquals(800, config->fuelRpmBins[0]);
	assertEquals(7000, config->fuelRpmBins[15]);

	engineConfiguration->crankingChargeAngle = 65;
	engineConfiguration->crankingTimingAngle = 31;

	engine->rpmCalculator.setRpmValue(200 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("cranking dwell", 54.166670, getSparkDwell(200 PASS_ENGINE_PARAMETER_SUFFIX));
	engine->rpmCalculator.setRpmValue(2000 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("running dwell", 4, getSparkDwell(2000 PASS_ENGINE_PARAMETER_SUFFIX));

	engine->rpmCalculator.setRpmValue(6000 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("higher rpm dwell", 3.25, getSparkDwell(6000 PASS_ENGINE_PARAMETER_SUFFIX));
}

void testTriggerDecoder2(const char *msg, engine_type_e type, int synchPointIndex, float channel1duty, float channel2duty) {
	printf("*************************************************** %s\r\n", msg);

	EngineTestHelper eth(type);
	EXPAND_EngineTestHelper;

	initSpeedDensity(PASS_ENGINE_PARAMETER_SIGNATURE);

	TriggerShape *t = &eth.engine.triggerCentral.triggerShape;

	assertFalseM("isError", t->shapeDefinitionError);

	assertEqualsM("synchPointIndex", synchPointIndex, t->getTriggerShapeSynchPointIndex());

	assertEqualsM5(msg, " channel1duty", channel1duty, t->expectedDutyCycle[0], 0.0001);
	assertEqualsM5(msg, " channel2duty", channel2duty, t->expectedDutyCycle[1], 0.0001);
}

static void testTriggerDecoder3(const char *msg, engine_type_e type, int synchPointIndex, float channel1duty, float channel2duty, float expectedGap) {
	printTriggerDebug = true;
	testTriggerDecoder2(msg, type, synchPointIndex, channel1duty, channel2duty);
	assertEqualsM2("actual gap ratio", expectedGap, actualSynchGap, 0.001);
	printTriggerDebug = false;
}

extern EventQueue schedulingQueue;

void testStartupFuelPumping(void) {
	printf("*************************************************** testStartupFuelPumping\r\n");
	EngineTestHelper eth(FORD_INLINE_6_1995);
	EXPAND_EngineTestHelper;

	StartupFuelPumping sf;

	engine->rpmCalculator.mockRpm = 0;

	engine->engineConfiguration->tpsMin = 0;
	engine->engineConfiguration->tpsMax = 10;

	setMockTpsPosition(6);
	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("pc#1", 1, sf.pumpsCounter);

	setMockTpsPosition(3);
	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("pumpsCounter#2", 1, sf.pumpsCounter);

	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("pc#3", 1, sf.pumpsCounter);

	engine->rpmCalculator.mockRpm = 10;
	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("pc#4", 0, sf.pumpsCounter);

	setMockTpsPosition(7);
	engine->rpmCalculator.mockRpm = 0;
	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("pc#5", 1, sf.pumpsCounter);

	setMockTpsPosition(3);
	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("pc#6", 1, sf.pumpsCounter);

	setMockTpsPosition(7);
	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("pc#7", 2, sf.pumpsCounter);
}

static void assertREquals(void *expected, void *actual) {
	assertEquals((float)(uint64_t)expected, (float)(uint64_t)actual);
}

static void assertREqualsM(const char *msg, void *expected, void *actual) {
	assertEqualsM(msg, (float)(uint64_t)expected, (float)(uint64_t)actual);
}

extern bool_t debugSignalExecutor;
extern EnginePins enginePins;

void testRpmCalculator(void) {
	printf("*************************************************** testRpmCalculator\r\n");
	timeNowUs = 0;
	schedulingQueue.clear();

	EngineTestHelper eth(FORD_INLINE_6_1995);
	EXPAND_EngineTestHelper;
	IgnitionEventList *ilist = &eth.engine.ignitionEvents;
	assertEqualsM("size #1", 0, ilist->isReady);

	assertEqualsM("engineCycle", 720, eth.engine.engineCycle);

	efiAssertVoid(eth.engine.engineConfiguration!=NULL, "null config in engine");

	engineConfiguration->trigger.customTotalToothCount = 8;
	engineConfiguration->globalFuelCorrection = 3;
	eth.applyTriggerShape();

	setFlatInjectorLag(0 PASS_ENGINE_PARAMETER_SUFFIX);

	engine->updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);
	timeNowUs = 0;
	assertEquals(0, eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));

	// triggerIndexByAngle update is now fixed! prepareOutputSignals() wasn't reliably called
	assertEquals(5, TRIGGER_SHAPE(triggerIndexByAngle[240]));
	assertEquals(5, TRIGGER_SHAPE(triggerIndexByAngle[241]));

	eth.fireTriggerEvents(48);

	assertEqualsM("RPM", 1500, eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));
	assertEqualsM("index #1", 15, eth.engine.triggerCentral.triggerState.getCurrentIndex());


	eth.clearQueue();

	debugSignalExecutor = true;

	assertEquals(eth.engine.triggerCentral.triggerState.shaft_is_synchronized, 1);

	timeNowUs += MS2US(5); // 5ms

	int st = timeNowUs;
	assertEqualsM("st value", 485000, st);

	// todo: why is this required here? we already have one 'prepareOutputSignals' in constructor, what's wrong with it?
	prepareOutputSignals(PASS_ENGINE_PARAMETER_SIGNATURE);

	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	assertEqualsM("fuel #1", 4.5450, eth.engine.injectionDuration);
	InjectionEvent *ie0 = &eth.engine.injectionEvents.elements[0];
	assertEqualsM("injection angle", 31.365, ie0->injectionStart.angleOffset);

	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER_SUFFIX);
	assertEquals(1500, eth.engine.rpmCalculator.rpmValue);

	assertEqualsM("dwell", 4.5, eth.engine.engineState.dwellAngle);
	assertEqualsM("fuel #2", 4.5450, eth.engine.injectionDuration);
	assertEqualsM("one degree", 111.1111, eth.engine.rpmCalculator.oneDegreeUs);
	assertEqualsM("size #2", 1, ilist->isReady);
	assertEqualsM("dwell angle", 0, ilist->elements[0].dwellPosition.eventAngle);
	assertEqualsM("dwell offset", 8.5, ilist->elements[0].dwellPosition.angleOffset);

	assertEqualsM("index #2", 0, eth.engine.triggerCentral.triggerState.getCurrentIndex());
	assertEqualsM("queue size/2", 2, schedulingQueue.size());
	{
	scheduling_s *ev0 = schedulingQueue.getForUnitText(0);

	assertREqualsM("Call@0", (void*)ev0->callback, (void*)turnSparkPinHigh);
	assertEqualsM("ev 0", st + 944, ev0->momentX);
	assertEqualsLM("coil 0", (long)&enginePins.coils[0], (long)((IgnitionEvent*)ev0->param)->outputs[0]);

	scheduling_s *ev1 = schedulingQueue.getForUnitText(1);
	assertREqualsM("Call@1", (void*)ev1->callback, (void*)turnSparkPinLow);
	assertEqualsM("ev 1", st + 1444, ev1->momentX);
	assertEqualsLM("coil 1", (long)&enginePins.coils[0], (long)((IgnitionEvent*)ev1->param)->outputs[0]);

	}

	schedulingQueue.clear();

	timeNowUs += MS2US(5);
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING PASS_ENGINE_PARAMETER_SUFFIX);
	timeNowUs += MS2US(5); // 5ms
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER_SUFFIX);
	timeNowUs += MS2US(5);
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("index #3", 3, eth.engine.triggerCentral.triggerState.getCurrentIndex());
	assertEqualsM("queue size 3", 4, schedulingQueue.size());
	assertEqualsM("ev 3", st + 13333 - 1515, schedulingQueue.getForUnitText(0)->momentX);
	assertEqualsM2("ev 5", st + 14277, schedulingQueue.getForUnitText(1)->momentX, 2);
	assertEqualsM("3/3", st + 14777, schedulingQueue.getForUnitText(2)->momentX);
	schedulingQueue.clear();

	assertEquals(5, TRIGGER_SHAPE(triggerIndexByAngle[240]));
	assertEquals(5, TRIGGER_SHAPE(triggerIndexByAngle[241]));

	timeNowUs += MS2US(5);
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("queue size 4.1", 0, schedulingQueue.size());

	timeNowUs += MS2US(5); // 5ms
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("queue size 4.2", 4, schedulingQueue.size());

	timeNowUs += MS2US(5); // 5ms
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("queue size 4.3", 4, schedulingQueue.size());

	assertEqualsM("dwell", 4.5, eth.engine.engineState.dwellAngle);
	assertEqualsM("fuel #3", 4.5450, eth.engine.injectionDuration);
	assertEquals(1500, eth.engine.rpmCalculator.rpmValue);

	assertInjectorUpEvent("ev 0/2", 0, -4849, 2 PASS_ENGINE_PARAMETER_SUFFIX);


	assertEqualsM("index #4", 6, eth.engine.triggerCentral.triggerState.getCurrentIndex());
	assertEqualsM("queue size 4", 4, schedulingQueue.size());
	schedulingQueue.clear();

	timeNowUs += 5000;
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("queue size 5", 2, schedulingQueue.size());
// todo: assert queue elements
	schedulingQueue.clear();

	timeNowUs += 5000; // 5ms
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("queue size 6", 2, schedulingQueue.size());
	assertEqualsM("6/0", st + 40944, schedulingQueue.getForUnitText(0)->momentX);
	assertEqualsM("6/1", st + 41444, schedulingQueue.getForUnitText(1)->momentX);
	schedulingQueue.clear();

	timeNowUs += 5000;
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("queue size 7", 0, schedulingQueue.size());
	schedulingQueue.clear();

	timeNowUs += 5000; // 5ms
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("queue size 8", 4, schedulingQueue.size());
	// todo: assert queue elements completely
	assertEqualsM("8/0", st + 53333 - 1515, schedulingQueue.getForUnitText(0)->momentX);
	assertEqualsM2("8/1", st + 54277, schedulingQueue.getForUnitText(1)->momentX, 0);
	assertEqualsM2("8/2", st + 54777, schedulingQueue.getForUnitText(2)->momentX, 0);
	schedulingQueue.clear();

	timeNowUs += 5000;
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("queue size 9", 0, schedulingQueue.size());
	schedulingQueue.clear();

	timeNowUs += 5000; // 5ms
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("queue size 10", 0, schedulingQueue.size());
	schedulingQueue.clear();
}

void testTriggerDecoder(void) {
	printf("*************************************************** testTriggerDecoder\r\n");

	persistent_config_s c;
	Engine e(&c);
	TriggerShape * s = &e.triggerCentral.triggerShape;


	persistent_config_s *config = &c;
	Engine *engine = &e;

	engine_configuration_s *engineConfiguration = &c.engineConfiguration;
	board_configuration_s *boardConfiguration = &c.engineConfiguration.bc;

	initializeSkippedToothTriggerShapeExt(s, 2, 0, FOUR_STROKE_CAM_SENSOR PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("shape size", s->getSize(), 4);
	assertEquals(s->wave.switchTimes[0], 0.25);
	assertEquals(s->wave.switchTimes[1], 0.5);
	assertEquals(s->wave.switchTimes[2], 0.75);
	assertEquals(s->wave.switchTimes[3], 1);

	testNoiselessDecoder();

	testDodgeNeonDecoder();
	testTriggerDecoder2("Dodge Neon 1995", DODGE_NEON_1995, 8, 0.4931, 0.2070);

	testFordAspire();
	testTriggerDecoder2("ford aspire", FORD_ASPIRE_1996, 4, 0.0000, 0.5);

	testTriggerDecoder2("dodge ram", DODGE_RAM, 16, 0.5000, 0.06);

	//testTriggerDecoder2("bmw", BMW_E34, 0, 0.9750, 0.5167);
	testTriggerDecoder2("bmw", BMW_E34, 0, 0.4667, 0.0);

	test1995FordInline6TriggerDecoder();

	testTriggerDecoder2("Miata NB", MAZDA_MIATA_NB1, 12, 0.0833, 0.0444);
	testTriggerDecoder2("Miata 2003", MAZDA_MIATA_2003, 3, 0.0444, 0.0);

	testTriggerDecoder2("Civic 4/0 both", TEST_CIVIC_4_0_BOTH, 0, 0.5000, 0.0);
	testTriggerDecoder2("Civic 4/0 rise", TEST_CIVIC_4_0_RISE, 0, 0.5000, 0.0);

	testTriggerDecoder2("test 2/1 both", TEST_ISSUE_366_BOTH, 0, 0.2500, 0.0);
	testTriggerDecoder2("test 2/1 rise", TEST_ISSUE_366_RISE, 0, 0.0000, 0.0);

	testTriggerDecoder2("test engine", TEST_ENGINE, 0, 0.7500, 0.2500);
	testTriggerDecoder2("testGY6_139QMB", GY6_139QMB, 0, 0.4375, 0.0);
	testTriggerDecoder2("testSubary", SUBARU_2003_WRX, 0, 0.4000, 0.0);

	testTriggerDecoder2("testFordEscortGt", FORD_ESCORT_GT, 0, 0.8096, 0.3844);
	testTriggerDecoder2("testMiniCooper", MINI_COOPER_R50, 121, 0.5222, 0.4959);
	testTriggerDecoder2("testRoverV8", ROVER_V8, 0, 0.4861, 0);

	testTriggerDecoder2("SATURN_ION_2004", SATURN_ION_2004, 0, 0.9028, 0.0);

	testTriggerDecoder2("NISSAN_PRIMERA", NISSAN_PRIMERA, 2, 0.9611, 0.0);

	testTriggerDecoder2("test1+1", CUSTOM_ENGINE, 0, 0.7500, 0.2500);

	testTriggerDecoder2("testCitroen", CITROEN_TU3JP, 0, 0.4833, 0);
	testTriggerDecoder2("testAccordCd 3w", HONDA_ACCORD_CD, 12, 0.8146, 0.5000);
	testTriggerDecoder2("testAccordCd 2w", HONDA_ACCORD_CD_TWO_WIRES, 2, 0.9167, 0.5);
	testTriggerDecoder2("testAccordCdDip", HONDA_ACCORD_CD_DIP, 27, 0.5000, 0.5000);

	testTriggerDecoder2("testMitsu", MITSU_4G93, 0, 0.3553, 0.3752);
	{
		EngineTestHelper eth(MITSU_4G93);
		EXPAND_EngineTestHelper;

		initSpeedDensity(PASS_ENGINE_PARAMETER_SIGNATURE);

//		TriggerShape *t = &eth.engine.triggerShape;
//		assertEquals(1, t->eventAngles[1]);
//		assertEqualsM("index at 0", 0, t->triggerIndexByAngle[56]);
//		assertEqualsM("index at 1", 1, t->triggerIndexByAngle[57]);
//
//		assertEquals(270, t->eventAngles[5]);
//		assertEqualsM("index at 269", 4, t->triggerIndexByAngle[269]);
//		assertEqualsM("index at 270", 5, t->triggerIndexByAngle[270]);
//		assertEqualsM("index at 271", 5, t->triggerIndexByAngle[271]);
//
//		assertEquals(306, t->eventAngles[6]);
//		assertEquals(5, t->triggerIndexByAngle[305]);
//		assertEquals(6, t->triggerIndexByAngle[306]);
//		assertEquals(6, t->triggerIndexByAngle[307]);
//
//		assertEquals(666, t->eventAngles[11]);
//		assertEqualsM("index for 665", 10, t->triggerIndexByAngle[665]);
//		assertEqualsM("index for 668", 11, t->triggerIndexByAngle[668]);


		eth.persistentConfig.engineConfiguration.useOnlyRisingEdgeForTrigger = false;
		eth.persistentConfig.engineConfiguration.bc.sensorChartMode = SC_DETAILED_RPM;
		applyNonPersistentConfiguration(NULL PASS_ENGINE_PARAMETER_SUFFIX);

//		assertEqualsM2("rpm#1", 16666.9746, eth.engine.triggerCentral.triggerState.instantRpmValue[0], 0.5);
//		assertEqualsM2("rpm#2", 16666.3750, eth.engine.triggerCentral.triggerState.instantRpmValue[1], 0.5);

	}
	testTriggerDecoder2("miata 1990", MIATA_1990, 11, 0.2985, 0.3890);
	testTriggerDecoder3("miata 1994", MIATA_1994_DEVIATOR, 11, 0.2985, 0.3890, MIATA_NA_GAP);
	testTriggerDecoder3("citroen", CITROEN_TU3JP, 0, 0.4833, 0.0, 2.9994);

	testTriggerDecoder2("MAZDA_323", MAZDA_323, 0, 0.4833, 0);

	testTriggerDecoder3("neon NGC4", DODGE_NEON_2003_CAM, 6, 0.5000, 0.0, CHRYSLER_NGC4_GAP);

	{
		printTriggerDebug = true;

		EngineTestHelper eth(DODGE_NEON_2003_CAM);
		EXPAND_EngineTestHelper;

		printf("!!!!!!!!!!!!!!!!!! Now trying with only rising edges !!!!!!!!!!!!!!!!!\r\n");
		engineConfiguration->useOnlyRisingEdgeForTrigger = true;

		applyNonPersistentConfiguration(NULL PASS_ENGINE_PARAMETER_SUFFIX);
		prepareShapes(PASS_ENGINE_PARAMETER_SIGNATURE);

		printTriggerDebug = false;
	}

	testTriggerDecoder2("sachs", SACHS, 0, 0.4800, 0.000);

	printTriggerDebug = true;
	testTriggerDecoder3("36+2+2+2", DAIHATSU,  28, 0.5000, 0.0, 0.5);
	testTriggerDecoder3("stratus NGC6", DODGE_STRATUS, 0, 0.8833, 0.0, CHRYSLER_NGC6_GAP);

	testTriggerDecoder2("vw ABA", VW_ABA, 114, 0.5000, 0.0);


	testStartupFuelPumping();
	testRpmCalculator();
}

extern fuel_Map3D_t fuelMap;

scheduling_s * assertEvent5(const char *msg, int index, void *callback, efitime_t start, efitime_t momentX) {
	assertTrueM(msg, schedulingQueue.size() > index);
	scheduling_s *event = schedulingQueue.getForUnitText(index);
	assertEqualsM4(msg, " up/down", (void*)event->callback == (void*) callback, 1);
	assertEqualsM(msg, momentX, event->momentX - start);
	return event;
}

void assertEvent(const char *msg, int index, void *callback, efitime_t start, efitime_t momentX, long param) {
	scheduling_s *event = assertEvent5(msg, index, callback, start, momentX);

	InjectionSignalPair *eventPair = (InjectionSignalPair *)event->param;

	InjectionSignalPair *expectedPair = (InjectionSignalPair *)param;

	assertEqualsLM(msg, expectedPair->outputs[0], (long)eventPair->outputs[0]);
// but this would not work	assertEqualsLM(msg, expectedPair, (long)eventPair);
}

void assertInjectorUpEvent(const char *msg, int eventIndex, efitime_t momentX, long injectorIndex DECLARE_ENGINE_PARAMETER_SUFFIX) {
	InjectionSignalPair *pair = &ENGINE(fuelActuators[injectorIndex]);
	assertEvent(msg, eventIndex, (void*)seTurnPinHigh, timeNowUs, momentX, (long)pair);
}

void assertInjectorDownEvent(const char *msg, int eventIndex, efitime_t momentX, long injectorIndex DECLARE_ENGINE_PARAMETER_SUFFIX) {
	InjectionSignalPair *pair = &ENGINE(fuelActuators[injectorIndex]);
	assertEvent(msg, eventIndex, (void*)seTurnPinLow, timeNowUs, momentX, (long)pair);
}

static void assertInjectionEvent(const char *msg, InjectionEvent *ev, int injectorIndex, int eventIndex, angle_t angleOffset, bool isOverlapping) {
	assertEqualsM4(msg, "inj index", injectorIndex, ev->outputs[0]->injectorIndex);
	assertEqualsM4(msg, " event index", eventIndex, ev->injectionStart.eventIndex);
	assertEqualsM4(msg, " event offset", angleOffset, ev->injectionStart.angleOffset);
}

void setupSimpleTestEngineWithMafAndTT_ONE_trigger(EngineTestHelper *eth, injection_mode_e injMode) {
	Engine *engine = &eth->engine;
	EXPAND_Engine

	timeNowUs = 0;
	eth->clearQueue();

	assertEquals(LM_PLAIN_MAF, engineConfiguration->fuelAlgorithm);
	engineConfiguration->isIgnitionEnabled = false; // let's focus on injection
	engineConfiguration->specs.cylindersCount = 4;
	// a bit of flexibility - the mode may be changed by some tests
	engineConfiguration->injectionMode = injMode;
	// set cranking mode (it's used by getCurrentInjectionMode())
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;

	setArrayValues(config->cltFuelCorrBins, CLT_CURVE_SIZE, 1);
	setArrayValues(engineConfiguration->injector.battLagCorr, VBAT_INJECTOR_CURVE_SIZE, 0);
	// this is needed to update injectorLag
	engine->updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);

	assertEqualsM("CLT", 70, engine->sensors.clt);
	assertEqualsM("trigger #1", 0, readIfTriggerConfigChangedForUnitTest());

	engineConfiguration->trigger.type = TT_ONE;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("trigger #2", 1, readIfTriggerConfigChangedForUnitTest());

	eth->applyTriggerShape();

}

static void setTestBug299(EngineTestHelper *eth) {
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(eth);
	Engine *engine = &eth->engine;
	EXPAND_Engine


	assertEqualsM("RPM=0", 0, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));
	assertEqualsM("setTestBug299 EL", 0, getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE));
	assertEqualsM("setTestBug299 IAT", 30, engine->sensors.iat);
	eth->fireTriggerEvents2(1, MS2US(20));
	// still no RPM since need to cycles measure cycle duration
	assertEqualsM("setTestBug299: RPM#1", 0, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));
	eth->fireTriggerEvents2(1, MS2US(20));
	assertEqualsM("setTestBug299: RPM#2", 3000, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));

	eth->clearQueue();

	/**
	 * Trigger up - scheduling fuel for full engine cycle
	 */
	timeNowUs += MS2US(20);
	eth->firePrimaryTriggerRise();
	// fuel schedule - short pulses.
	// time...|0.......|10......|20......|30......|40
	// inj #0 |.......#|........|.......#|........|
	// inj #1 |........|.......#|........|.......#|
	assertEqualsM("qs#00", 4, schedulingQueue.size());
	assertEqualsM("rev cnt#3", 3, engine->rpmCalculator.getRevolutionCounter());
	assertInjectorUpEvent("setTestBug299: 1@0", 0, MS2US(8.5), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("@1", 1, MS2US(10), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorUpEvent("1@2", 2, MS2US(18.5), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("1@3", 3, MS2US(20), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("exec#0", 0, schedulingQueue.executeAll(timeNowUs));

	FuelSchedule * t = &ENGINE(injectionEvents);

	assertInjectionEvent("#0", &t->elements[0], 0, 1, 153, false);
	assertInjectionEvent("#1_i_@", &t->elements[1], 1, 1, 333, false);
	assertInjectionEvent("#2@", &t->elements[2], 0, 0, 153, false);
	assertInjectionEvent("inj#3@", &t->elements[3], 1, 0, 153 + 180, false);

	/**
	 * Trigger down - no new events, executing some
	 */
	timeNowUs += MS2US(20);
	eth->firePrimaryTriggerFall();
	// same exact picture
	// time...|-20.....|-10.....|0.......|10......|20
	// inj #0 |.......#|........|.......#|........|
	// inj #1 |........|.......#|........|.......#|
	assertEqualsM("qs#0", 8, schedulingQueue.size());
	assertEqualsM("rev cnt#3", 3, engine->rpmCalculator.getRevolutionCounter());
	assertInjectorUpEvent("02@0", 0, MS2US(-11.5), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("@1", 1, MS2US(-10), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorUpEvent("@2", 2, MS2US(-1.5), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("02@3", 3, MS2US(0), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorUpEvent("02@4", 4, MS2US(8.5), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("@5", 5, MS2US(10), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorUpEvent("02@6", 6, MS2US(18.5), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("@7", 7, MS2US(20), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("exec#1", 4, schedulingQueue.executeAll(timeNowUs));


	/**
	 * Trigger up again
	 */
	timeNowUs += MS2US(20);
	assertInjectorUpEvent("22@0", 0, MS2US(-11.5), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("22@1", 1, MS2US(-10), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorUpEvent("22@2", 2, MS2US(-1.5), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("22@3", 3, MS2US(0), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("exec#20", 4, schedulingQueue.executeAll(timeNowUs));

	eth->firePrimaryTriggerRise();
	assertEqualsM("qs#0-2", 4, schedulingQueue.size());
	// fuel schedule - short pulses. and more realistic schedule this time
	// time...|-20.....|-10.....|0.......|10......|20
	// inj #0 |.......#|........|.......#|........|
	// inj #1 |........|.......#|........|.......#|
	assertInjectorUpEvent("2@0", 0, MS2US(8.5), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("@1", 1, MS2US(10), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorUpEvent("@2", 2, MS2US(18.5), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("2@3", 3, MS2US(20), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("exec#2", 0, schedulingQueue.executeAll(timeNowUs));


	timeNowUs += MS2US(20);
	schedulingQueue.executeAll(timeNowUs);
	eth->firePrimaryTriggerFall();
	// fuel schedule - short pulses. and more realistic schedule this time
	// time...|-20.....|-10.....|0.......|10......|20
	// inj #0 |.......#|........|........|........|
	// inj #1 |........|.......#|........|........|
	assertEqualsM("qs#0-2", 4, schedulingQueue.size());
	assertEqualsM("rev cnt#4", 4, engine->rpmCalculator.getRevolutionCounter());
	assertInjectorUpEvent("0@0", 0, MS2US(8.5), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("0@1", 1, MS2US(10), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorUpEvent("0@2", 2, MS2US(18.5), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("0@3", 3, MS2US(20), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("exec#3", 0, schedulingQueue.executeAll(timeNowUs));


	testMafValue = 0;
	assertEqualsM("maf", 0, getMaf(PASS_ENGINE_PARAMETER_SIGNATURE));

	assertEqualsM("iatC", 1, engine->engineState.iatFuelCorrection);
	assertEqualsM("cltC", 1, engine->engineState.cltFuelCorrection);
	assertEqualsM("lag", 0, engine->engineState.injectorLag);

	assertEqualsM("setTestBug299: RPM", 3000, engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));

	assertEqualsM("fuel#1", 1.5, engine->injectionDuration);

	assertEqualsM("duty for maf=0", 7.5, getInjectorDutyCycle(engine->rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE) PASS_ENGINE_PARAMETER_SUFFIX));

	testMafValue = 3;
	assertEqualsM("maf", 3, getMaf(PASS_ENGINE_PARAMETER_SIGNATURE));
}

static void assertInjectors(const char *msg, int value0, int value1) {
	assertEqualsM4(msg, "inj#0", value0, enginePins.injectors[0].currentLogicValue);
	assertEqualsM4(msg, "inj#1", value1, enginePins.injectors[1].currentLogicValue);
}

void testFuelSchedulerBug299smallAndMedium(void) {
	printf("*************************************************** testFuelSchedulerBug299 small to medium\r\n");

	EngineTestHelper eth(TEST_ENGINE);
	EXPAND_EngineTestHelper
	setTestBug299(&eth);

	FuelSchedule * t;

	assertInjectors("#0_inj", 0, 0);


	int engineLoadIndex = findIndex(config->fuelLoadBins, FUEL_LOAD_COUNT, testMafValue);
	assertEquals(8, engineLoadIndex);
	setArrayValues(fuelMap.pointers[engineLoadIndex], FUEL_RPM_COUNT, 25);
	setArrayValues(fuelMap.pointers[engineLoadIndex + 1], FUEL_RPM_COUNT, 25);

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("fuel#2", 12.5, engine->injectionDuration);
	assertEqualsM("duty for maf=3", 62.5, getInjectorDutyCycle(eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE) PASS_ENGINE_PARAMETER_SUFFIX));

	assertEqualsM("qs#1", 4, schedulingQueue.size());
	timeNowUs += MS2US(20);
	assertEqualsM("exec#2#0", 4, schedulingQueue.executeAll(timeNowUs));
	assertEqualsM("qs#1#2", 0, schedulingQueue.size());


	assertEqualsM("rev cnt#4#0", 4, engine->rpmCalculator.getRevolutionCounter());
	eth.firePrimaryTriggerRise();
	assertEqualsM("rev cnt#4#1", 5, engine->rpmCalculator.getRevolutionCounter());
	// time...|0.......|10......|20......|30......|40......|50......|60......|
	// inj #0 |########|##...###|########|.....###|########|........|........|
	// inj #1 |.....###|########|....####|########|........|........|........|
	assertEqualsM("qs#4", 6, schedulingQueue.size());
//todo	assertInjectorUpEvent("04@0", 0, MS2US(0), 0);
//	assertInjectorUpEvent("04@1", 1, MS2US(7.5), 1);
//	assertInjectorDownEvent("04@2", 2, MS2US(12.5), 0);
//	assertInjectorUpEvent("04@3", 3, MS2US(17.5), 0);
//	assertInjectorDownEvent("04@4", 4, MS2US(20), 1);
//	assertInjectorDownEvent("04@5", 5, MS2US(30), 0);
//	assertInjectorDownEvent("04@6", 6, MS2US(30), 0);
//	assertInjectorUpEvent("04@7", 7, MS2US(37.5), 0);
//	assertInjectorDownEvent("04@8", 8, MS2US(40.0), 1);
//	assertInjectorDownEvent("04@9", 9, MS2US(50.0), 0);

//	{
//		scheduling_s *ev = schedulingQueue.getForUnitText(9);
//		assertEqualsM("rev cnt#4#2", 5, engine->rpmCalculator.getRevolutionCounter());
//		assertTrueM("down 50", ev == &engine->engineConfiguration2->fuelActuators[2].signalPair[1].signalTimerDown);
//	}


	assertEqualsM("exec#4", 0, schedulingQueue.executeAll(timeNowUs));


	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();
	assertEqualsM("qs#2#1", 8, schedulingQueue.size());
	assertEqualsM("rev cnt#5", 5, engine->rpmCalculator.getRevolutionCounter());
	// using old fuel schedule - but already wider pulses
	// time...|-20.....|-10.....|0.......|10......|20......|30......|40......|
	// inj #0 |........|.....###|########|.....###|########|........|........|
	// inj #1 |.....###|########|.....###|########|........|........|........|
//	assertInjectorUpEvent("5@0", 0, MS2US(-12.5), 1);
//	assertInjectorDownEvent("5@1", 1, MS2US(-7.5), 0);
//	assertInjectorUpEvent("5@2", 2, MS2US(-2.5), 0);
//	assertInjectorDownEvent("5@3", 3, MS2US(0), 1);
//	assertInjectorUpEvent("5@4", 4, MS2US(7.5), 1);
//
//	assertInjectorDownEvent("5@4", 5, MS2US(10), 0);
//	assertInjectorUpEvent("5@6", 6, MS2US(17.5), 0);
//	assertInjectorDownEvent("5@7", 7, MS2US(20.0), 1);
//	assertInjectorDownEvent("5@8", 8, MS2US(30.0), 0);
	assertEqualsM("exec#5", 3, schedulingQueue.executeAll(timeNowUs));

	/**
	 * one more revolution
	 */
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);


	t = &ENGINE(injectionEvents);

	assertInjectionEvent("#0", &t->elements[0], 0, 0, 315, false);
	assertInjectionEvent("#1__", &t->elements[1], 1, 1, 135, false);
	assertInjectionEvent("inj#2", &t->elements[2], 0, 0, 153, false);
	assertInjectionEvent("inj#3", &t->elements[3], 1, 0, 333, false);

	timeNowUs += MS2US(20);
	assertEqualsM("qs#02", 5, schedulingQueue.size());
//	assertInjectorUpEvent("6@0", 0, MS2US(-12.5), 1);
//	assertInjectorDownEvent("6@1", 1, MS2US(-10.0), 0);
//	assertInjectorUpEvent("6@2", 2, MS2US(-2.5), 0);
//	assertInjectorDownEvent("6@3", 3, MS2US(0), 1);
//	assertInjectorDownEvent("6@4", 4, MS2US(10.0), 0);

	// so placing this 'executeAll' changes much?
	assertEqualsM("exec#07", 5, schedulingQueue.executeAll(timeNowUs));
	assertEqualsM("qs#07", 0, schedulingQueue.size());
//	assertInjectorDownEvent("26@0", 0, MS2US(10.0), 0);

	eth.firePrimaryTriggerRise();
	assertEqualsM("qs#2#2", 4, schedulingQueue.size());
	assertEqualsM("rev cnt6", 6, engine->rpmCalculator.getRevolutionCounter());
	// time...|-20.....|-10.....|0.......|10......|20......|30......|40......|
	// inj #0 |########|.....###|########|....####|........|........|........|
	// inj #1 |.....###|########|.....###|########|........|........|........|
//	assertInjectorDownEvent("06@5", 5, MS2US(30.0), 0);
//	assertInjectorUpEvent("06@6", 6, MS2US(37.5), 0);
//	assertInjectorDownEvent("06@7", 7, MS2US(40.0), 1);

	assertEqualsM("exec#7", 0, schedulingQueue.executeAll(timeNowUs));

	assertInjectors("#1_ij_", 0, 0);

	timeNowUs += MS2US(20);

	// time...|-20.....|-10.....|0.......|10......|20......|30......|40......|
	// inj #0 |########|.......#|........|........|........|........|........|
	// inj #1 |....####|########|........|........|........|........|........|
	assertEqualsM("qs#022", 4, schedulingQueue.size());
//	assertInjectorUpEvent("7@0", 0, MS2US(-12.5), 1);
//	assertInjectorDownEvent("7@1", 1, MS2US(-10.0), 0);
//	assertInjectorUpEvent("7@2", 2, MS2US(-2.5), 0);
//	assertInjectorDownEvent("7@3", 3, MS2US(0), 1);
//	assertInjectorDownEvent("7@4", 4, MS2US(10), 0);
////	assertInjectorDownEvent("i7@5", 5, MS2US(20.0), 0);
////	assertInjectorUpEvent("7@6", 6, MS2US(17.5), 0);
////	assertInjectorDownEvent("7@7", 7, MS2US(20), 1);
//	// todo index 8

	assertEqualsM("executed #06", 3, schedulingQueue.executeAll(timeNowUs));
	assertInjectors("#4", 1, 0);
	assertEqualsM("qs#06", 1, schedulingQueue.size());
	assertInjectorDownEvent("17@0", 0, MS2US(10), 0 PASS_ENGINE_PARAMETER_SUFFIX);
//	assertInjectorDownEvent("17@1", 1, MS2US(10.0), 0);
//	assertInjectorUpEvent("17@2", 2, MS2US(17.5), 0);
//	assertInjectorDownEvent("17@3", 3, MS2US(20), 1);
	// todo index 4

	eth.firePrimaryTriggerFall();

	assertEqualsM("qs#3", 5, schedulingQueue.size());
	assertEqualsM("rev cnt6", 6, engine->rpmCalculator.getRevolutionCounter());
	assertEqualsM("executed #6", 0, schedulingQueue.executeAll(timeNowUs));


	timeNowUs += MS2US(20);
	assertEqualsM("executed #06", 4, schedulingQueue.executeAll(timeNowUs));
	assertEqualsM("qs#06", 1, schedulingQueue.size());
	assertInjectors("inj#2", 1, 0);

	eth.firePrimaryTriggerRise();
	assertEqualsM("Queue.size#03", 5, schedulingQueue.size());
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertInjectorUpEvent("07@0", 0, MS2US(7.5), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("07@1", 1, MS2US(10), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorUpEvent("07@2", 2, MS2US(17.5), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("07@3", 3, MS2US(20), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("07@4", 4, MS2US(30), 0 PASS_ENGINE_PARAMETER_SUFFIX);
//	assertInjectorDownEvent("07@5", 5, MS2US(30), 0);
//	assertInjectorUpEvent("07@6", 6, MS2US(37.5), 0);
//	assertInjectorDownEvent("07@7", 7, MS2US(40), 1);
//	assertInjectorDownEvent("07@8", 8, MS2US(50), 0);

	assertEqualsM("executeAll#3", 0, schedulingQueue.executeAll(timeNowUs));
	timeNowUs += MS2US(20);
	assertEqualsM("executeAll#4", 4, schedulingQueue.executeAll(timeNowUs));

	t = &ENGINE(injectionEvents);

	assertInjectionEvent("#0#", &t->elements[0], 0, 0, 315, false);
	assertInjectionEvent("#1#", &t->elements[1], 1, 1, 135, false);
	assertInjectionEvent("#2#", &t->elements[2], 0, 1, 315, true);
	assertInjectionEvent("#3#", &t->elements[3], 1, 0, 45 + 90, false);

	setArrayValues(fuelMap.pointers[engineLoadIndex], FUEL_RPM_COUNT, 35);
	setArrayValues(fuelMap.pointers[engineLoadIndex + 1], FUEL_RPM_COUNT, 35);
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("fuel#3", 17.5, engine->injectionDuration);
	// duty cycle above 75% is a special use-case because 'special' fuel event overlappes the next normal event in batch mode
	assertEqualsM("duty for maf=3", 87.5, getInjectorDutyCycle(eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE) PASS_ENGINE_PARAMETER_SUFFIX));


	assertInjectionEvent("#03", &t->elements[0], 0, 0, 315, false);

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);


	assertEqualsM("inj#0", 1, enginePins.injectors[0].currentLogicValue);

	assertEqualsM("Queue.size#04", 1, schedulingQueue.size());
	assertInjectorDownEvent("08@0", 0, MS2US(10), 0 PASS_ENGINE_PARAMETER_SUFFIX);
//	assertInjectorDownEvent("08@1", 1, MS2US(10), 0);
//	assertInjectorUpEvent("08@2", 2, MS2US(17.5), 0);
//	assertInjectorDownEvent("08@3", 3, MS2US(20), 1);
//	assertInjectorDownEvent("08@4", 4, MS2US(30), 0);

	eth.firePrimaryTriggerFall();


	schedulingQueue.executeAll(timeNowUs);
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();
	assertEqualsM("Queue.size#05", 7, schedulingQueue.size());
	schedulingQueue.executeAll(timeNowUs);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();
	schedulingQueue.executeAll(timeNowUs);

	timeNowUs += MS2US(20);
	schedulingQueue.executeAll(timeNowUs);
	eth.firePrimaryTriggerRise();

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	t = &ENGINE(injectionEvents);

	assertInjectionEvent("#00", &t->elements[0], 0, 0, 225, false); // 87.5 duty cycle
	assertInjectionEvent("#10", &t->elements[1], 1, 1, 45, false);
	assertInjectionEvent("#20", &t->elements[2], 0, 1, 225, true);
	assertInjectionEvent("#30", &t->elements[3], 1, 0, 45, false);

	 // todo: what's what? a mix of new something and old something?
	assertEqualsM("qs#5", 4, schedulingQueue.size());
//	assertInjectorDownEvent("8@0", 0, MS2US(5.0), 1);
//	assertInjectorUpEvent("8@1", 1, MS2US(7.5), 1);
//	assertInjectorDownEvent("8@2", 2, MS2US(15.0), 0);
//	assertInjectorUpEvent("8@3", 3, MS2US(17.5), 0);
//	assertInjectorDownEvent("8@4", 4, MS2US(25), 1);
//	assertInjectorDownEvent("8@5", 5, MS2US(35), 0);
////	assertInjectorDownEvent("8@6", 6, MS2US(35), 0);
////	assertInjectorUpEvent("8@7", 7, MS2US(37.5), 0);
////	assertInjectorDownEvent("8@8", 8, MS2US(45), 1);
////	assertInjectorDownEvent("8@9", 9, MS2US(55), 0);

	schedulingQueue.executeAll(timeNowUs);

	unitTestValue = 0;
	testMafValue = 0;
}

static void setInjectionMode(int value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	engineConfiguration->injectionMode = (injection_mode_e) value;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}


void testDifferentInjectionModes(void) {
	printf("*************************************************** testDifferentInjectionModes\r\n");

	EngineTestHelper eth(TEST_ENGINE);
	EXPAND_EngineTestHelper
	setTestBug299(&eth);
	assertEqualsM("Lqs#0", 4, schedulingQueue.size());

	// set fuel map values - extract method?
	int engineLoadIndex = findIndex(config->fuelLoadBins, FUEL_LOAD_COUNT, testMafValue);
	assertEquals(8, engineLoadIndex);
	setArrayValues(fuelMap.pointers[engineLoadIndex], FUEL_RPM_COUNT, 40);
	setArrayValues(fuelMap.pointers[engineLoadIndex + 1], FUEL_RPM_COUNT, 40);

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("injectionMode IM_BATCH", (int)IM_BATCH, (int)engineConfiguration->injectionMode);
	assertEqualsM("injection while batch", 20, engine->injectionDuration);

	setInjectionMode((int)IM_SIMULTANEOUS PASS_ENGINE_PARAMETER_SUFFIX);
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("injection while simultaneous", 10, engine->injectionDuration);

	setInjectionMode((int)IM_SEQUENTIAL PASS_ENGINE_PARAMETER_SUFFIX);
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("injection while IM_SEQUENTIAL", 40, engine->injectionDuration);

	setInjectionMode((int)IM_SINGLE_POINT PASS_ENGINE_PARAMETER_SUFFIX);
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("injection while IM_SINGLE_POINT", 40, engine->injectionDuration);
}

void testFuelSchedulerBug299smallAndLarge(void) {
	printf("*************************************************** testFuelSchedulerBug299 small to large\r\n");

	EngineTestHelper eth(TEST_ENGINE);
	EXPAND_EngineTestHelper
	setTestBug299(&eth);
	assertEqualsM("Lqs#0", 4, schedulingQueue.size());

	// set fuel map values - extract method?
	int engineLoadIndex = findIndex(config->fuelLoadBins, FUEL_LOAD_COUNT, testMafValue);
	assertEquals(8, engineLoadIndex);
	setArrayValues(fuelMap.pointers[engineLoadIndex], FUEL_RPM_COUNT, 35);
	setArrayValues(fuelMap.pointers[engineLoadIndex + 1], FUEL_RPM_COUNT, 35);

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("Lfuel#2", 17.5, engine->injectionDuration);
	assertEqualsM("Lduty for maf=3", 87.5, getInjectorDutyCycle(eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE) PASS_ENGINE_PARAMETER_SUFFIX));


	assertEqualsM("Lqs#1", 4, schedulingQueue.size());
	timeNowUs += MS2US(20);
	schedulingQueue.executeAll(timeNowUs);

	// injector #1 is low before the test
	assertFalseM("injector@0", enginePins.injectors[0].currentLogicValue);

	eth.firePrimaryTriggerRise();

	// time...|0.......|10......|20......|30......|40......|50......|60......|
	// inj #0 |########|########|########|.....###|########|........|........|
	// inj #1 |..######|########|....####|########|........|........|........|
	assertEqualsM("Lqs#4", 6, schedulingQueue.size());
	assertInjectorUpEvent("L04@0", 0, MS2US(8.5), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorUpEvent("L04@1", 1, MS2US(12.5), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	// special overlapping injection is merged with one of the scheduled injections
	assertInjectorUpEvent("L04@2", 2, MS2US(18.5), 1 PASS_ENGINE_PARAMETER_SUFFIX);

	assertInjectorDownEvent("L04@3", 3, MS2US(26), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("L04@4", 4, MS2US(30), 0 PASS_ENGINE_PARAMETER_SUFFIX);

//	assertInjectorDownEvent("L04@5", 5, MS2US(30), 0);
//	assertInjectorUpEvent("L04@6", 6, MS2US(32.5), 0);
//	assertInjectorDownEvent("L04@7", 7, MS2US(40.0), 1);
//	assertInjectorDownEvent("L04@8", 8, MS2US(50.0), 0);


	schedulingQueue.executeAll(timeNowUs + 1);
	// injector goes high...
	assertFalseM("injector@1", enginePins.injectors[0].currentLogicValue);

	schedulingQueue.executeAll(timeNowUs + MS2US(17.5) + 1);
	// injector does not go low too soon, that's a feature :)
	assertTrueM("injector@2", enginePins.injectors[0].currentLogicValue);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();

	assertEqualsM("Lqs#04", 6, schedulingQueue.size());
	assertInjectorUpEvent("L015@0", 0, MS2US(-1.5), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorUpEvent("L015@1", 1, MS2US(2.5), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("L015@2", 2, MS2US(6), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("L015@3", 3, MS2US(10), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("L015@4", 4, MS2US(16), 1 PASS_ENGINE_PARAMETER_SUFFIX);
//todo	assertInjectorDownEvent("L015@5", 5, MS2US(30), 0);


	schedulingQueue.executeAll(timeNowUs + MS2US(10) + 1);
	// end of combined injection
	assertFalseM("injector@3", enginePins.injectors[0].currentLogicValue);


	timeNowUs += MS2US(20);
	schedulingQueue.executeAll(timeNowUs);
	assertEqualsM("Lqs#04", 0, schedulingQueue.size());

	setArrayValues(fuelMap.pointers[engineLoadIndex], FUEL_RPM_COUNT, 4);
	setArrayValues(fuelMap.pointers[engineLoadIndex + 1], FUEL_RPM_COUNT, 4);

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("Lfuel#4", 2, engine->injectionDuration);
	assertEqualsM("Lduty for maf=3", 10, getInjectorDutyCycle(eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE) PASS_ENGINE_PARAMETER_SUFFIX));


	eth.firePrimaryTriggerRise();

	//todoassertEqualsM("Lqs#05", 5, schedulingQueue.size());
	//todo	assertInjectorUpEvent("L016@0", 0, MS2US(8), 0);
	//todo	assertInjectorDownEvent("L016@1", 1, MS2US(10), 0);
	//todo	assertInjectorDownEvent("L016@2", 2, MS2US(10), 0);


	timeNowUs += MS2US(20);
	schedulingQueue.executeAll(timeNowUs); // issue here
	eth.firePrimaryTriggerFall();


	timeNowUs += MS2US(20);
	schedulingQueue.executeAll(timeNowUs);
	eth.firePrimaryTriggerRise();

	assertEqualsM("Lqs#5", 4, schedulingQueue.size());
	assertInjectorUpEvent("L05@0", 0, MS2US(8), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("L05@1", 1, MS2US(10), 0 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorUpEvent("L05@2", 2, MS2US(18), 1 PASS_ENGINE_PARAMETER_SUFFIX);
	assertInjectorDownEvent("L05@3", 3, MS2US(20), 1 PASS_ENGINE_PARAMETER_SUFFIX);

	timeNowUs += MS2US(20);
	schedulingQueue.executeAll(timeNowUs);
}

void testSparkReverseOrderBug319(void) {
	printf("*************************************************** testSparkReverseOrderBug319 small to medium\r\n");

	EngineTestHelper eth(TEST_ENGINE);
	EXPAND_EngineTestHelper

	engineConfiguration->useOnlyRisingEdgeForTrigger = false;
	engineConfiguration->isInjectionEnabled = false;
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	setConstantDwell(45 PASS_ENGINE_PARAMETER_SUFFIX);

	// this is needed to update injectorLag
	engine->updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);

	assertEqualsM("CLT", 70, engine->sensors.clt);

	engineConfiguration->trigger.type = TT_ONE;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);

	eth.applyTriggerShape();
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);


	timeNowUs = 0;
	setWholeTimingTable(0 PASS_ENGINE_PARAMETER_SUFFIX);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();

	schedulingQueue.executeAll(timeNowUs);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();

	assertEqualsM("testSparkReverseOrderBug319: RPM", 3000, eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));


	assertEqualsM("testSparkReverseOrderBug319: queue size", 7, schedulingQueue.size());
	schedulingQueue.executeAll(timeNowUs);
	printf("***************************************************\r\n");

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();
	schedulingQueue.executeAll(timeNowUs);


	timeNowUs += 100; // executing new signal too early
	eth.firePrimaryTriggerFall();
	schedulingQueue.executeAll(timeNowUs);

	assertEqualsM("out-of-order #1", 1, enginePins.coils[3].outOfOrder);


	timeNowUs += MS2US(200); // moving time forward to execute all pending actions
	schedulingQueue.executeAll(timeNowUs);

	assertEqualsM("out-of-order #2", 0, enginePins.coils[3].outOfOrder);

	printf("*************************************************** now let's have a good engine cycle and confirm things work\r\n");

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();
	schedulingQueue.executeAll(timeNowUs);

	assertEqualsM("RPM#2", 545, eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));

	assertEqualsM("out-of-order #3", 0, enginePins.coils[3].outOfOrder);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();
	schedulingQueue.executeAll(timeNowUs);
	assertEqualsM("out-of-order #4", 1, enginePins.coils[3].outOfOrder);

	printf("*************************************************** (rpm is back) now let's have a good engine cycle and confirm things work\r\n");

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();
	schedulingQueue.executeAll(timeNowUs);

	assertEqualsM("RPM#3", 3000, eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));

	assertEqualsM("out-of-order #5 on c4", 1, enginePins.coils[3].outOfOrder);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();
	schedulingQueue.executeAll(timeNowUs);
	assertEqualsM("out-of-order #6 on c4", 1, enginePins.coils[3].outOfOrder);

	printf("*************************************************** (rpm is back 2) now let's have a good engine cycle and confirm things work\r\n");

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();
	schedulingQueue.executeAll(timeNowUs);

	assertEqualsM("RPM#4", 3000, eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_SIGNATURE));

	assertEqualsM("out-of-order #7", 1, enginePins.coils[3].outOfOrder);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();
	schedulingQueue.executeAll(timeNowUs);
	assertEqualsM("out-of-order #8", 0, enginePins.coils[3].outOfOrder);
}

void testMissedSpark299(void) {
	printf("*************************************************** testMissedSpark299\r\n");

	EngineTestHelper eth(TEST_ENGINE);
	EXPAND_EngineTestHelper
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->useOnlyRisingEdgeForTrigger = false;
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	engineConfiguration->isIgnitionEnabled = true;
	engineConfiguration->isInjectionEnabled = false;

	assertEqualsM("warningCounter#0", 4, unitTestWarningCounter);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();
	schedulingQueue.executeAll(timeNowUs);
	assertEqualsM("ci#0", 0, eth.engine.triggerCentral.triggerState.currentCycle.current_index);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();
	schedulingQueue.executeAll(timeNowUs);
	assertEqualsM("ci#1", 1, eth.engine.triggerCentral.triggerState.currentCycle.current_index);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();
	schedulingQueue.executeAll(timeNowUs);
	assertEqualsM("ci#2", 0, eth.engine.triggerCentral.triggerState.currentCycle.current_index);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();
	schedulingQueue.executeAll(timeNowUs);
	assertEqualsM("ci#3", 1, eth.engine.triggerCentral.triggerState.currentCycle.current_index);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();
	schedulingQueue.executeAll(timeNowUs);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();
	schedulingQueue.executeAll(timeNowUs);
	assertEqualsM("ci#5", 1, eth.engine.triggerCentral.triggerState.currentCycle.current_index);


	printf("*************************************************** testMissedSpark299 start\r\n");

	assertEquals(3000, eth.engine.rpmCalculator.rpmValue);

	setWholeTimingTable(3 PASS_ENGINE_PARAMETER_SUFFIX);
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);


	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();
	schedulingQueue.executeAll(timeNowUs);
	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();
	schedulingQueue.executeAll(timeNowUs);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();
	schedulingQueue.executeAll(timeNowUs);
	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();
	schedulingQueue.executeAll(timeNowUs);

	setWholeTimingTable(-5 PASS_ENGINE_PARAMETER_SUFFIX);
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();
	schedulingQueue.executeAll(timeNowUs);
	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();
	schedulingQueue.executeAll(timeNowUs);

	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerRise();
	schedulingQueue.executeAll(timeNowUs);
	timeNowUs += MS2US(20);
	eth.firePrimaryTriggerFall();
	schedulingQueue.executeAll(timeNowUs);

	assertEqualsM("warningCounter#1", 5, unitTestWarningCounter);
}
