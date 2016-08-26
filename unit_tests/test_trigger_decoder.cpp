/**
 * @file	test_trigger_decoder.cpp
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2016
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
#include "engine_test_helper.h"
#include "speed_density.h"
#include "fuel_math.h"

extern int timeNow;
extern float unitTestValue;
extern float testMafValue;

extern bool printTriggerDebug;
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
	return findTriggerZeroEventIndex(&eth.engine.triggerCentral.triggerState, shape, &engineConfiguration->trigger PASS_ENGINE_PARAMETER);
}

static void testDodgeNeonDecoder(void) {
	printf("*************************************************** testDodgeNeonDecoder\r\n");
	initTriggerDecoder();

	assertEqualsM("trigger zero index", 8, getTheAngle(DODGE_NEON_1995));

	EngineTestHelper eth(DODGE_NEON_1995);
	EXPAND_EngineTestHelper;

	TriggerShape * shape = &eth.engine.triggerShape;
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

	assertEqualsM("triggerIndex ", 0, getTheAngle(FORD_INLINE_6_1995));

	initTriggerDecoder();

	EngineTestHelper eth(FORD_INLINE_6_1995);
	EXPAND_EngineTestHelper;

	TriggerShape * shape = &eth.engine.triggerShape;

	assertEqualsM("triggerShapeSynchPointIndex", 0, shape->getTriggerShapeSynchPointIndex());

	// this is needed to have valid CLT and IAT. todo: extract method
	engine->updateSlowSensors(PASS_ENGINE_PARAMETER_F);

	event_trigger_position_s position;
	assertEqualsM("globalTriggerAngleOffset", 0, engineConfiguration->globalTriggerAngleOffset);
	findTriggerPosition(&position, 0 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 0, 0);

	findTriggerPosition(&position, 200 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 3, 20);

	findTriggerPosition(&position, 360 PASS_ENGINE_PARAMETER);
	assertTriggerPosition(&position, 6, 0);

	eth.initTriggerShapeAndRpmCalculator();
	eth.engine.triggerCentral.addEventListener(mainTriggerCallback, "main loop", &eth.engine);

	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_F);
	eth.fireTriggerEvents(48);
	assertEquals(2000, eth.engine.rpmCalculator.rpmValue);
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_F);
	eth.fireTriggerEvents(48);

	IgnitionEventList *ecl = &eth.ec2.ignitionEvents[0];
	assertEqualsM("ford inline ignition events size", 6, ecl->size);
	assertEqualsM("event index", 0, ecl->elements[0].dwellPosition.eventIndex);
	assertEqualsM("angle offset#1", 7, ecl->elements[0].dwellPosition.angleOffset);

	assertEqualsM("event index", 10, ecl->elements[5].dwellPosition.eventIndex);
	assertEqualsM("angle offset#2", 7, ecl->elements[5].dwellPosition.angleOffset);

	TriggerState state;

	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized);
	int r = 10;
	state.decodeTriggerEvent(SHAFT_PRIMARY_FALLING, r PASS_ENGINE_PARAMETER);
	assertFalseM("shaft_is_synchronized", state.shaft_is_synchronized); // still no synchronization
	state.decodeTriggerEvent(SHAFT_PRIMARY_RISING, ++r PASS_ENGINE_PARAMETER);
	assertTrue(state.shaft_is_synchronized); // first signal rise synchronize
	assertEquals(0, state.getCurrentIndex());
	state.decodeTriggerEvent(SHAFT_PRIMARY_FALLING, r++ PASS_ENGINE_PARAMETER);
	assertEquals(1, state.getCurrentIndex());

	for (int i = 2; i < 10;) {
		state.decodeTriggerEvent(SHAFT_PRIMARY_RISING, r++ PASS_ENGINE_PARAMETER);
		assertEqualsM("even", i++, state.getCurrentIndex());
		state.decodeTriggerEvent(SHAFT_PRIMARY_FALLING, r++ PASS_ENGINE_PARAMETER);
		assertEqualsM("odd", i++, state.getCurrentIndex());
	}

	state.decodeTriggerEvent(SHAFT_PRIMARY_RISING, r++ PASS_ENGINE_PARAMETER);
	assertEquals(10, state.getCurrentIndex());

	state.decodeTriggerEvent(SHAFT_PRIMARY_FALLING, r++ PASS_ENGINE_PARAMETER);
	assertEquals(11, state.getCurrentIndex());

	state.decodeTriggerEvent(SHAFT_PRIMARY_RISING, r++ PASS_ENGINE_PARAMETER);
	assertEquals(0, state.getCurrentIndex()); // new revolution

	assertEqualsM("running dwell", 0.5, getSparkDwell(2000 PASS_ENGINE_PARAMETER));
}

void testFordAspire(void) {
	printf("*************************************************** testFordAspire\r\n");

	assertEquals(4, getTheAngle(FORD_ASPIRE_1996));

	EngineTestHelper eth(FORD_ASPIRE_1996);
	EXPAND_EngineTestHelper;

	assertEquals(4, eth.engine.triggerShape.getTriggerShapeSynchPointIndex());

	assertEquals(800, config->fuelRpmBins[0]);
	assertEquals(7000, config->fuelRpmBins[15]);

	engineConfiguration->crankingChargeAngle = 65;
	engineConfiguration->crankingTimingAngle = 31;

	assertEqualsM("cranking dwell", 54.166670, getSparkDwell(200 PASS_ENGINE_PARAMETER));
	assertEqualsM("running dwell", 4, getSparkDwell(2000 PASS_ENGINE_PARAMETER));

	assertEqualsM("higher rpm dwell", 3.25, getSparkDwell(6000 PASS_ENGINE_PARAMETER));
}

static void testTriggerDecoder2(const char *msg, engine_type_e type, int synchPointIndex, float channel1duty, float channel2duty) {
	printf("*************************************************** %s\r\n", msg);

	EngineTestHelper eth(type);
	EXPAND_EngineTestHelper;

	initSpeedDensity(PASS_ENGINE_PARAMETER_F);

	TriggerShape *t = &eth.engine.triggerShape;

	assertEqualsM("synchPointIndex", synchPointIndex, t->getTriggerShapeSynchPointIndex());

	assertEqualsM("channel1duty", channel1duty, t->dutyCycle[0]);
	assertEqualsM("channel2duty", channel2duty, t->dutyCycle[1]);
}

static void testTriggerDecoder3(const char *msg, engine_type_e type, int synchPointIndex, float channel1duty, float channel2duty, float expectedGap) {
	printTriggerDebug = true;
	testTriggerDecoder2(msg, type, synchPointIndex, channel1duty, channel2duty);
	assertEqualsM2("actual gap ratio", expectedGap, actualSynchGap, 0.001);
	printTriggerDebug = false;
}

extern EventQueue schedulingQueue;

extern int mockTps;

void testStartupFuelPumping(void) {
	printf("*************************************************** testStartupFuelPumping\r\n");
	EngineTestHelper eth(FORD_INLINE_6_1995);
	EXPAND_EngineTestHelper;

	StartupFuelPumping sf;

	engine->rpmCalculator.mockRpm = 0;

	engine->engineConfiguration->tpsMin = 0;
	engine->engineConfiguration->tpsMax = 10;

	mockTps = TPS_TS_CONVERSION * 6;
	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#1", 1, sf.pumpsCounter);

	mockTps = TPS_TS_CONVERSION * 3;
	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#2", 1, sf.pumpsCounter);

	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#3", 1, sf.pumpsCounter);

	engine->rpmCalculator.mockRpm = 10;
	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#4", 0, sf.pumpsCounter);

	mockTps = TPS_TS_CONVERSION * 7;
	engine->rpmCalculator.mockRpm = 0;
	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#5", 1, sf.pumpsCounter);

	mockTps = TPS_TS_CONVERSION * 3;
	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#6", 1, sf.pumpsCounter);

	mockTps = TPS_TS_CONVERSION * 7;
	sf.update(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("pc#7", 2, sf.pumpsCounter);
}

static void assertREquals(void *expected, void *actual) {
	assertEquals((float)(uint64_t)expected, (float)(uint64_t)actual);
}

static void assertREqualsM(const char *msg, void *expected, void *actual) {
	assertEqualsM(msg, (float)(uint64_t)expected, (float)(uint64_t)actual);
}

extern bool_t debugSignalExecutor;
extern engine_pins_s enginePins;

void testRpmCalculator(void) {
	printf("*************************************************** testRpmCalculator\r\n");
	timeNow = 0;
	schedulingQueue.clear();

	EngineTestHelper eth(FORD_INLINE_6_1995);
	EXPAND_EngineTestHelper;
	IgnitionEventList *ilist = &eth.engine.engineConfiguration2->ignitionEvents[1];
	assertEqualsM("size #1", 0, ilist->size);

	assertEqualsM("engineCycle", 720, eth.engine.engineCycle);

	efiAssertVoid(eth.engine.engineConfiguration!=NULL, "null config in engine");

	engineConfiguration->trigger.customTotalToothCount = 8;
	engineConfiguration->globalFuelCorrection = 3;
	eth.initTriggerShapeAndRpmCalculator();

	setInjectorLag(0 PASS_ENGINE_PARAMETER);

	engine->updateSlowSensors(PASS_ENGINE_PARAMETER_F);
	timeNow = 0;
	assertEquals(0, eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_F));

	assertEquals(4, engine->triggerShape.triggerIndexByAngle[240]);
	assertEquals(4, engine->triggerShape.triggerIndexByAngle[241]);

	eth.fireTriggerEvents(48);

	assertEqualsM("RPM", 1500, eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_F));
	assertEqualsM("index #1", 15, eth.engine.triggerCentral.triggerState.getCurrentIndex());

	static MainTriggerCallback triggerCallbackInstance;
	triggerCallbackInstance.init(&eth.engine);
	eth.engine.triggerCentral.addEventListener(mainTriggerCallback, "main loop", &eth.engine);

	assertEqualsM("queue size/0", 0, schedulingQueue.size());

	debugSignalExecutor = true;

	assertEquals(eth.engine.triggerCentral.triggerState.shaft_is_synchronized, 1);

	timeNow += 5000; // 5ms

	int st = timeNow;
	assertEqualsM("st value", 485000, st);

	// todo: why is this required here? we already have one 'prepareOutputSignals' in constructor, what's wrong with it?
	prepareOutputSignals(PASS_ENGINE_PARAMETER_F);

	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_F);

	assertEqualsM("fuel #1", 4.5450, eth.engine.fuelMs);
	InjectionEvent *ie0 = &eth.engine.engineConfiguration2->injectionEvents->injectionEvents.elements[0];
	assertEqualsM("injection angle", 31.365, ie0->injectionStart.angleOffset);

	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER);
	assertEquals(1500, eth.engine.rpmCalculator.rpmValue);

	assertEqualsM("dwell", 4.5, eth.engine.engineState.dwellAngle);
	assertEqualsM("fuel #2", 4.5450, eth.engine.fuelMs);
	assertEqualsM("one degree", 111.1111, eth.engine.rpmCalculator.oneDegreeUs);
	assertEqualsM("size #2", 6, ilist->size);
	assertEqualsM("dwell angle", 0, ilist->elements[0].dwellPosition.eventAngle);
	assertEqualsM("dwell offset", 8.5, ilist->elements[0].dwellPosition.angleOffset);

	assertEqualsM("index #2", 0, eth.engine.triggerCentral.triggerState.getCurrentIndex());
	assertEqualsM("queue size/2", 2, schedulingQueue.size());
	{
	scheduling_s *ev0 = schedulingQueue.getForUnitText(0);

	assertREqualsM("Call@0", (void*)ev0->callback, (void*)turnSparkPinHigh);
	assertEqualsM("ev 0", st + 944, ev0->momentX);
	assertEqualsLM("o 0", (long)&enginePins.coils[0], (long)ev0->param);

	scheduling_s *ev1 = schedulingQueue.getForUnitText(1);
	assertREqualsM("Call@1", (void*)ev1->callback, (void*)turnSparkPinLow);
	assertEqualsM("ev 1", st + 1444, ev1->momentX);
	assertEqualsLM("o 1", (long)&enginePins.coils[0], (long)ev1->param);

	}

	schedulingQueue.clear();

	timeNow += 5000;
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING PASS_ENGINE_PARAMETER);
	timeNow += 5000; // 5ms
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER);
	timeNow += 5000;
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING PASS_ENGINE_PARAMETER);
	assertEqualsM("index #3", 3, eth.engine.triggerCentral.triggerState.getCurrentIndex());
	assertEqualsM("queue size 3", 6, schedulingQueue.size());
	assertEqualsM("ev 3", st + 13333 - 1515, schedulingQueue.getForUnitText(0)->momentX);
	assertEqualsM("ev 4", st + 13333 - 1515, schedulingQueue.getForUnitText(1)->momentX);
	assertEqualsM2("ev 5", st + 14277, schedulingQueue.getForUnitText(2)->momentX, 2);
	assertEqualsM("3/3", st + 14777, schedulingQueue.getForUnitText(3)->momentX);
	schedulingQueue.clear();

	assertEquals(5, engine->triggerShape.triggerIndexByAngle[240]);
	assertEquals(5, engine->triggerShape.triggerIndexByAngle[241]);

	timeNow += 5000;
	assertEqualsM("Size 4.1", 6, engine->engineConfiguration2->injectionEvents->eventsCount);
	assertFalseM("No squirts expected 4.1", engine->engineConfiguration2->injectionEvents->hasEvents[4]);
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 4.1", 0, schedulingQueue.size());

	timeNow += 5000; // 5ms
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 4.2", 6, schedulingQueue.size());

	timeNow += 5000; // 5ms
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 4.3", 6, schedulingQueue.size());

	assertEqualsM("dwell", 4.5, eth.engine.engineState.dwellAngle);
	assertEqualsM("fuel #3", 4.5450, eth.engine.fuelMs);
	assertEquals(1500, eth.engine.rpmCalculator.rpmValue);
	{
	scheduling_s *ev0 = schedulingQueue.getForUnitText(0);

	assertREqualsM("turnHigh", (void*)ev0->callback, (void*)seTurnPinHigh);
	assertEqualsM("ev 0/2", st + 26666 - 1515, ev0->momentX);
	assertEqualsLM("o 0/2", (long)&enginePins.injectors[2], (long)ev0->param);

	scheduling_s *ev1 = schedulingQueue.getForUnitText(1);
	assertEqualsM("ev 1/2", st + 26666 - 1515, ev1->momentX);
	assertEqualsLM("o 1/2", (long)&enginePins.injectors[5], (long)ev1->param);
	}

	assertEqualsM("index #4", 6, eth.engine.triggerCentral.triggerState.getCurrentIndex());
	assertEqualsM("queue size 4", 6, schedulingQueue.size());
	schedulingQueue.clear();

	timeNow += 5000;
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 5", 4, schedulingQueue.size());
// todo: assert queue elements
	schedulingQueue.clear();

	timeNow += 5000; // 5ms
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 6", 2, schedulingQueue.size());
	assertEqualsM("6/0", st + 40944, schedulingQueue.getForUnitText(0)->momentX);
	assertEqualsM("6/1", st + 41444, schedulingQueue.getForUnitText(1)->momentX);
	schedulingQueue.clear();

	timeNow += 5000;
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 7", 0, schedulingQueue.size());
	schedulingQueue.clear();

	timeNow += 5000; // 5ms
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 8", 6, schedulingQueue.size());
	// todo: assert queue elements completely
	assertEqualsM("8/0", st + 53333 - 1515, schedulingQueue.getForUnitText(0)->momentX);
	assertEqualsM("8/1", st + 53333 - 1515, schedulingQueue.getForUnitText(1)->momentX);
	assertEqualsM2("8/2", st + 54277, schedulingQueue.getForUnitText(2)->momentX, 0);
	assertEqualsM2("8/3", st + 54777, schedulingQueue.getForUnitText(3)->momentX, 0);
	schedulingQueue.clear();

	timeNow += 5000;
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_FALLING PASS_ENGINE_PARAMETER);
	assertEqualsM("queue size 9", 0, schedulingQueue.size());
	schedulingQueue.clear();

	timeNow += 5000; // 5ms
	eth.engine.triggerCentral.handleShaftSignal(SHAFT_PRIMARY_RISING PASS_ENGINE_PARAMETER);
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
	testTriggerDecoder2("ford aspire", FORD_ASPIRE_1996, 4, 0.5000, 0.3841);

	testTriggerDecoder2("dodge ram", DODGE_RAM, 16, 0.5000, 0.06);

	//testTriggerDecoder2("bmw", BMW_E34, 0, 0.9750, 0.5167);
	testTriggerDecoder2("bmw", BMW_E34, 0, 0.4833, 0.0);

	test1995FordInline6TriggerDecoder();
	testTriggerDecoder2("Miata NB", MAZDA_MIATA_NB, 12, 0.0833, 0.0444);

	testTriggerDecoder2("test engine", TEST_ENGINE, 0, 0.7500, 0.2500);
	testTriggerDecoder2("testGY6_139QMB", GY6_139QMB, 0, 0.4375, 0.0);
	testTriggerDecoder2("testSubary", SUBARU_2003_WRX, 0, 0.4000, 0.0);

	testTriggerDecoder2("testFordEscortGt", FORD_ESCORT_GT, 0, 0.8096, 0.3844);
	testTriggerDecoder2("testMiniCooper", MINI_COOPER_R50, 121, 0.5222, 0.4959);
	testTriggerDecoder2("testRoverV8", ROVER_V8, 0, 0.4861, 0);

	testTriggerDecoder2("SATURN_ION_2004", SATURN_ION_2004, 4, 0.5, 0.3841);

	testTriggerDecoder2("test1+1", CUSTOM_ENGINE, 0, 0.7500, 0.2500);

	testTriggerDecoder2("testCitroen", CITROEN_TU3JP, 0, 0.4833, 0);
	testTriggerDecoder2("testAccordCd 3w", HONDA_ACCORD_CD, 12, 0.8146, 0.5000);
	testTriggerDecoder2("testAccordCd 2w", HONDA_ACCORD_CD_TWO_WIRES, 2, 0.9167, 0.5);
	testTriggerDecoder2("testAccordCdDip", HONDA_ACCORD_CD_DIP, 27, 0.5000, 0.5000);

	testTriggerDecoder2("testMitsu", MITSU_4G93, 3, 0.3750, 0.3889);
	{
		EngineTestHelper eth(MITSU_4G93);
		EXPAND_EngineTestHelper;

		initSpeedDensity(PASS_ENGINE_PARAMETER_F);

		TriggerShape *t = &eth.engine.triggerShape;
		assertEquals(56, t->eventAngles[1]);
		assertEqualsM("index at 0", 0, t->triggerIndexByAngle[56]);
		assertEqualsM("index at 1", 1, t->triggerIndexByAngle[57]);

		assertEquals(270, t->eventAngles[5]);
		assertEqualsM("index at 269", 4, t->triggerIndexByAngle[269]);
		assertEqualsM("index at 270", 5, t->triggerIndexByAngle[270]);
		assertEqualsM("index at 271", 5, t->triggerIndexByAngle[271]);

		assertEquals(306, t->eventAngles[6]);
		assertEquals(5, t->triggerIndexByAngle[305]);
		assertEquals(6, t->triggerIndexByAngle[306]);
		assertEquals(6, t->triggerIndexByAngle[307]);

		assertEquals(666, t->eventAngles[11]);
		assertEqualsM("index for 665", 10, t->triggerIndexByAngle[665]);
		assertEqualsM("index for 668", 11, t->triggerIndexByAngle[668]);


		eth.persistentConfig.engineConfiguration.useOnlyRisingEdgeForTrigger = false;
		eth.persistentConfig.engineConfiguration.bc.sensorChartMode = SC_DETAILED_RPM;
		applyNonPersistentConfiguration(NULL PASS_ENGINE_PARAMETER);

//		assertEqualsM2("rpm#1", 16666.9746, eth.engine.triggerCentral.triggerState.instantRpmValue[0], 0.5);
//		assertEqualsM2("rpm#2", 16666.3750, eth.engine.triggerCentral.triggerState.instantRpmValue[1], 0.5);

	}
	testTriggerDecoder2("miata 1990", MIATA_1990, 11, 0.2985, 0.3890);
	testTriggerDecoder3("miata 1994", MIATA_1994_DEVIATOR, 11, 0.2985, 0.3890, MIATA_NA_GAP);
	testTriggerDecoder3("citroen", CITROEN_TU3JP, 0, 0.4833, 0.0, 2.9994);

	testTriggerDecoder2("MAZDA_323", MAZDA_323, 0, 0.4833, 0);

	testTriggerDecoder3("neon NGC4", DODGE_NEON_2003, 6, 0.5000, 0.0, CHRYSLER_NGC4_GAP);

	{
		printTriggerDebug = true;

		EngineTestHelper eth(DODGE_NEON_2003);
		EXPAND_EngineTestHelper;

		printf("!!!!!!!!!!!!!!!!!! Now trying with only rising edges !!!!!!!!!!!!!!!!!\r\n");
		engineConfiguration->useOnlyRisingEdgeForTrigger = true;

		applyNonPersistentConfiguration(NULL PASS_ENGINE_PARAMETER);
		prepareShapes(PASS_ENGINE_PARAMETER_F);

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

void testFuelSchedulerBug299(void) {
	printf("*************************************************** testFuelSchedulerBug299\r\n");
	EngineTestHelper eth(TEST_ENGINE);
	EXPAND_EngineTestHelper;

	timeNow = 0;
	schedulingQueue.clear();

	setArrayValues(config->cltFuelCorrBins, CLT_CURVE_SIZE, 1);
	setArrayValues(engineConfiguration->injector.battLagCorr, VBAT_INJECTOR_CURVE_SIZE, 0);
	// this is needed to update injectorLag
	engine->updateSlowSensors(PASS_ENGINE_PARAMETER_F);

	assertEqualsM("CLT", 70, engine->engineState.clt);

	engineConfiguration->trigger.type = TT_ONE;
	incrementGlobalConfigurationVersion();

	eth.initTriggerShapeAndRpmCalculator();

	assertEqualsM("RPM=0", 0, eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_F));

	eth.fireTriggerEvents2(2, MS2US(20));

	assertEquals(LM_PLAIN_MAF, engineConfiguration->algorithm);

	testMafValue = 0;
	assertEqualsM("maf", 0, getMaf(PASS_ENGINE_PARAMETER_F));

	assertEqualsM("iatC", 1, engine->engineState.iatFuelCorrection);
	assertEqualsM("cltC", 1, engine->engineState.cltFuelCorrection);
	assertEqualsM("lag", 0, engine->engineState.injectorLag);

	assertEqualsM("RPM", 3000, eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_F));

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("fuel#1", 3, engine->fuelMs);

	assertEqualsM("duty for maf=0", 7.5, getInjectorDutyCycle(eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_F) PASS_ENGINE_PARAMETER));

	testMafValue = 3;
	assertEqualsM("maf", 3, getMaf(PASS_ENGINE_PARAMETER_F));

	int engineLoadIndex = findIndex(config->fuelLoadBins, FUEL_LOAD_COUNT, testMafValue);
	assertEquals(8, engineLoadIndex);
	setArrayValues(fuelMap.pointers[engineLoadIndex], FUEL_RPM_COUNT, 35);
	setArrayValues(fuelMap.pointers[engineLoadIndex + 1], FUEL_RPM_COUNT, 35);

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_F);
	assertEqualsM("fuel#2", 35, engine->fuelMs);
	assertEqualsM("duty for maf=3", 87.5, getInjectorDutyCycle(eth.engine.rpmCalculator.getRpm(PASS_ENGINE_PARAMETER_F) PASS_ENGINE_PARAMETER));



	unitTestValue = 0;
	testMafValue = 0;
}
