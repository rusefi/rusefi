/**
 * @file	test_trigger_decoder.cpp
 *
 * @date Dec 24, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "engine_test_helper.h"
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

extern float testMafValue;
extern WarningCodeState unitTestWarningCodeState;
extern bool printTriggerDebug;
extern float actualSynchGap;

extern "C" {
void sendOutConfirmation(char *value, int i);
}

void sendOutConfirmation(char *value, int i) {
	// test implementation
}

static int getTriggerZeroEventIndex(engine_type_e engineType) {
	WITH_ENGINE_TEST_HELPER(engineType);

	initDataStructures(PASS_ENGINE_PARAMETER_SIGNATURE);

	TriggerShape * shape = &eth.engine.triggerCentral.triggerShape;
	return findTriggerZeroEventIndex(&eth.engine.triggerCentral.triggerState, shape, &engineConfiguration->trigger PASS_ENGINE_PARAMETER_SUFFIX);
}

static void testDodgeNeonDecoder(void) {
	printf("*************************************************** testDodgeNeonDecoder95\r\n");
	initTriggerDecoder();

	ASSERT_EQ( 8,  getTriggerZeroEventIndex(DODGE_NEON_1995)) << "DODGE_NEON_1995: trigger zero index";

	WITH_ENGINE_TEST_HELPER(DODGE_NEON_1995);

	TriggerShape * shape = &eth.engine.triggerCentral.triggerShape;
	ASSERT_EQ(8, shape->getTriggerShapeSynchPointIndex());

	TriggerState state;

	ASSERT_FALSE(state.shaft_is_synchronized) << "1 shaft_is_synchronized";

//	int r = 0;
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_RISING, r + 60);
//	ASSERT_FALSE(state.shaft_is_synchronized) << "2 shaft_is_synchronized"; // still no synchronization

//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_FALLING, r + 210);
//	ASSERT_FALSE(state.shaft_is_synchronized) << "3 shaft_is_synchronized"; // still no synchronization
//
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_RISING, r + 420);
//	ASSERT_FALSE(state.shaft_is_synchronized) << "4 shaft_is_synchronized"; // still no synchronization
//
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_FALLING, r + 630);
//	ASSERT_FALSE(state.shaft_is_synchronized); // still no synchronization
//
//	printf("2nd camshaft revolution\r\n");
//	r = 720;
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_RISING, r + 60);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_FALLING, r + 210);
//	ASSERT_TRUE(state.shaft_is_synchronized);
//	ASSERT_EQ(0, state.current_index);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_RISING, r + 420);
//	ASSERT_EQ(1, state.current_index);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_FALLING, r + 630);
//	ASSERT_EQ(2, state.current_index);
//
//	printf("3rd camshaft revolution\r\n");
//	r = 2 * 720;
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_RISING, r + 60);
//	ASSERT_EQ( 3,  state.current_index) << "current index";
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_FALLING, r + 210);
//	ASSERT_TRUE(state.shaft_is_synchronized);
//	ASSERT_EQ( 0,  state.current_index) << "current index";
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_RISING, r + 420);
//	processTriggerEvent(&state, shape, &ec->triggerConfig, SHAFT_PRIMARY_FALLING, r + 630);
}

static void assertTriggerPosition(event_trigger_position_s *position, int eventIndex, float angleOffset) {
	assertEqualsM("eventIndex", eventIndex, position->eventIndex);
	assertEqualsM("angleOffset", angleOffset, position->angleOffset);
}

TEST(misc, testSomethingWeird) {
	printf("*************************************************** testSomethingWeird\r\n");

	WITH_ENGINE_TEST_HELPER(FORD_INLINE_6_1995);

	TriggerState state_;
	TriggerState *sta = &state_;


	ASSERT_FALSE(sta->shaft_is_synchronized) << "shaft_is_synchronized";
	int r = 10;
	sta->decodeTriggerEvent(SHAFT_PRIMARY_FALLING, r PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_FALSE(sta->shaft_is_synchronized) << "shaft_is_synchronized"; // still no synchronization
	sta->decodeTriggerEvent(SHAFT_PRIMARY_RISING, ++r PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_TRUE(sta->shaft_is_synchronized); // first signal rise synchronize
	ASSERT_EQ(0, sta->getCurrentIndex());
	sta->decodeTriggerEvent(SHAFT_PRIMARY_FALLING, r++ PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ(1, sta->getCurrentIndex());

	for (int i = 2; i < 10;) {
		sta->decodeTriggerEvent(SHAFT_PRIMARY_RISING, r++ PASS_ENGINE_PARAMETER_SUFFIX);
		assertEqualsM("even", i++, sta->getCurrentIndex());
		sta->decodeTriggerEvent(SHAFT_PRIMARY_FALLING, r++ PASS_ENGINE_PARAMETER_SUFFIX);
		assertEqualsM("odd", i++, sta->getCurrentIndex());
	}

	sta->decodeTriggerEvent(SHAFT_PRIMARY_RISING, r++ PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ(10, sta->getCurrentIndex());

	sta->decodeTriggerEvent(SHAFT_PRIMARY_FALLING, r++ PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ(11, sta->getCurrentIndex());

	sta->decodeTriggerEvent(SHAFT_PRIMARY_RISING, r++ PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ(0, sta->getCurrentIndex()); // new revolution
}

TEST(misc, test1995FordInline6TriggerDecoder) {

	printf("*************************************************** test1995FordInline6TriggerDecoder\r\n");

	ASSERT_EQ( 0,  getTriggerZeroEventIndex(FORD_INLINE_6_1995)) << "triggerIndex ";

	WITH_ENGINE_TEST_HELPER(FORD_INLINE_6_1995);

	TriggerShape * shape = &engine->triggerCentral.triggerShape;

	ASSERT_EQ( 0,  shape->getTriggerShapeSynchPointIndex()) << "triggerShapeSynchPointIndex";

	event_trigger_position_s position;
	ASSERT_EQ( 0,  engineConfiguration->globalTriggerAngleOffset) << "globalTriggerAngleOffset";
	TRIGGER_SHAPE(findTriggerPosition(&position, 0, engineConfiguration->globalTriggerAngleOffset));
	assertTriggerPosition(&position, 0, 0);

	TRIGGER_SHAPE(findTriggerPosition(&position, 200, engineConfiguration->globalTriggerAngleOffset));
	assertTriggerPosition(&position, 3, 20);

	TRIGGER_SHAPE(findTriggerPosition(&position, 360, engineConfiguration->globalTriggerAngleOffset));
	assertTriggerPosition(&position, 6, 0);

	eth.applyTriggerShape();

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	eth.fireTriggerEvents(48);
	eth.assertRpm(2000, "rpm");
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	eth.fireTriggerEvents(48);

	IgnitionEventList *ecl = &engine->ignitionEvents;
	ASSERT_EQ( 1,  ecl->isReady) << "ford inline ignition events size";
	ASSERT_EQ( 0,  ecl->elements[0].dwellPosition.eventIndex) << "event index";
	ASSERT_NEAR(7.8621, ecl->elements[0].dwellPosition.angleOffset, EPS4D) << "angle offset#1";

	ASSERT_EQ( 10,  ecl->elements[5].dwellPosition.eventIndex) << "event index";
	ASSERT_NEAR(7.8621, ecl->elements[5].dwellPosition.angleOffset, EPS4D) << "angle offset#2";


	ASSERT_FLOAT_EQ(0.5, getSparkDwell(2000 PASS_ENGINE_PARAMETER_SUFFIX)) << "running dwell";
}

TEST(misc, testFordAspire) {
	printf("*************************************************** testFordAspire\r\n");

	ASSERT_EQ( 4,  getTriggerZeroEventIndex(FORD_ASPIRE_1996)) << "getTriggerZeroEventIndex";

	WITH_ENGINE_TEST_HELPER(FORD_ASPIRE_1996);

	ASSERT_EQ( 4,  TRIGGER_SHAPE(getTriggerShapeSynchPointIndex())) << "getTriggerShapeSynchPointIndex";

	ASSERT_EQ(800, config->fuelRpmBins[0]);
	ASSERT_EQ(7000, config->fuelRpmBins[15]);

	engineConfiguration->crankingChargeAngle = 65;
	engineConfiguration->crankingTimingAngle = 31;
	engineConfiguration->useConstantDwellDuringCranking = false;

	engine->rpmCalculator.setRpmValue(200 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("cranking dwell", 54.166670, getSparkDwell(200 PASS_ENGINE_PARAMETER_SUFFIX));
	engine->rpmCalculator.setRpmValue(2000 PASS_ENGINE_PARAMETER_SUFFIX);
	ASSERT_EQ( 4,  getSparkDwell(2000 PASS_ENGINE_PARAMETER_SUFFIX)) << "running dwell";

	engine->rpmCalculator.setRpmValue(6000 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("higher rpm dwell", 3.25, getSparkDwell(6000 PASS_ENGINE_PARAMETER_SUFFIX));
}

static void testTriggerDecoder2(const char *msg, engine_type_e type, int synchPointIndex, float channel1duty, float channel2duty) {
	printf("====================================================================================== testTriggerDecoder2 msg=%s\r\n", msg);

	WITH_ENGINE_TEST_HELPER(type);

	TriggerShape *t = &ENGINE(triggerCentral.triggerShape);

	ASSERT_FALSE(t->shapeDefinitionError) << "isError";

	assertEqualsM("synchPointIndex", synchPointIndex, t->getTriggerShapeSynchPointIndex());

	ASSERT_NEAR(channel1duty, t->expectedDutyCycle[0], 0.0001) << msg << " channel1duty";
	ASSERT_NEAR(channel2duty, t->expectedDutyCycle[1], 0.0001) << msg << " channel2duty";
}

static void testTriggerDecoder3(const char *msg, engine_type_e type, int synchPointIndex, float channel1duty, float channel2duty, float expectedGap) {
	printTriggerDebug = true;
	testTriggerDecoder2(msg, type, synchPointIndex, channel1duty, channel2duty);
	assertEqualsM2("actual gap ratio", expectedGap, actualSynchGap, 0.001);
	printTriggerDebug = false;
}

TEST(misc, testStartupFuelPumping) {
	printf("*************************************************** testStartupFuelPumping\r\n");
	WITH_ENGINE_TEST_HELPER(FORD_INLINE_6_1995);

	StartupFuelPumping sf;

	engine->rpmCalculator.mockRpm = 0;

	engineConfiguration->tpsMin = 0;
	engineConfiguration->tpsMax = 10;

	setMockTpsAdc(6 PASS_ENGINE_PARAMETER_SUFFIX);
	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 1,  sf.pumpsCounter) << "pc#1";

	setMockTpsAdc(3 PASS_ENGINE_PARAMETER_SUFFIX);
	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 1,  sf.pumpsCounter) << "pumpsCounter#2";

	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 1,  sf.pumpsCounter) << "pc#3";

	engine->rpmCalculator.mockRpm = 10;
	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 0,  sf.pumpsCounter) << "pc#4";

	setMockTpsAdc(7 PASS_ENGINE_PARAMETER_SUFFIX);
	engine->rpmCalculator.mockRpm = 0;
	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 1,  sf.pumpsCounter) << "pc#5";

	setMockTpsAdc(3 PASS_ENGINE_PARAMETER_SUFFIX);
	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 1,  sf.pumpsCounter) << "pc#6";

	setMockTpsAdc(7 PASS_ENGINE_PARAMETER_SUFFIX);
	sf.update(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 2,  sf.pumpsCounter) << "pc#7";
}

static void assertREquals(void *expected, void *actual) {
	ASSERT_EQ((float)(uint64_t)expected, (float)(uint64_t)actual);
}

static void assertREqualsM(const char *msg, void *expected, void *actual) {
	assertEqualsM(msg, (float)(uint64_t)expected, (float)(uint64_t)actual);
}

extern bool_t debugSignalExecutor;
extern EnginePins enginePins;

TEST(misc, testRpmCalculator) {
	printf("*************************************************** testRpmCalculator\r\n");

	WITH_ENGINE_TEST_HELPER(FORD_INLINE_6_1995);
	IgnitionEventList *ilist = &engine->ignitionEvents;
	ASSERT_EQ( 0,  ilist->isReady) << "size #1";

	ASSERT_EQ( 720,  engine->engineCycle) << "engineCycle";

	efiAssertVoid(CUSTOM_ERR_6670, engineConfiguration!=NULL, "null config in engine");

	engineConfiguration->trigger.customTotalToothCount = 8;
	engineConfiguration->globalFuelCorrection = 3;
	eth.applyTriggerShape();

	setFlatInjectorLag(0 PASS_CONFIG_PARAMETER_SUFFIX);

	engine->updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);
	engine->sensors.clt = 70; // 'testCltValue' does not give us exact number so we have to hack here. todo: migrate test
	engine->sensors.iat = 30; // 'testIatValue' does not give us exact number so we have to hack here. todo: migrate test

	ASSERT_EQ(0, GET_RPM());

	// triggerIndexByAngle update is now fixed! prepareOutputSignals() wasn't reliably called
	ASSERT_EQ(5, TRIGGER_SHAPE(triggerIndexByAngle[240]));
	ASSERT_EQ(5, TRIGGER_SHAPE(triggerIndexByAngle[241]));

	eth.fireTriggerEvents(/* count */ 48);

	ASSERT_EQ( 1500,  GET_RPM()) << "RPM";
	ASSERT_EQ( 15,  engine->triggerCentral.triggerState.getCurrentIndex()) << "index #1";


	eth.clearQueue();

	debugSignalExecutor = true;

	ASSERT_EQ(engine->triggerCentral.triggerState.shaft_is_synchronized, 1);

	eth.moveTimeForwardMs(5 /*ms*/);

	int start = eth.getTimeNowUs();
	ASSERT_EQ( 485000,  start) << "start value";

	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	assertEqualsM("fuel #1", 4.5450, engine->injectionDuration);
	InjectionEvent *ie0 = &engine->injectionEvents.elements[0];
	assertEqualsM("injection angle", 31.365, ie0->injectionStart.angleOffset);

	eth.firePrimaryTriggerRise();
	ASSERT_EQ(1500, eth.engine.rpmCalculator.rpmValue);

	assertEqualsM("dwell", 4.5, engine->engineState.dwellAngle);
	assertEqualsM("fuel #2", 4.5450, engine->injectionDuration);
	assertEqualsM("one degree", 111.1111, engine->rpmCalculator.oneDegreeUs);
	ASSERT_EQ( 1,  ilist->isReady) << "size #2";
	ASSERT_EQ( 0,  ilist->elements[0].dwellPosition.eventAngle) << "dwell angle";
	assertEqualsM("dwell offset", 8.5, ilist->elements[0].dwellPosition.angleOffset);

	ASSERT_EQ( 0,  eth.engine.triggerCentral.triggerState.getCurrentIndex()) << "index #2";
	ASSERT_EQ( 2,  engine->executor.size()) << "queue size/2";
	{
	scheduling_s *ev0 = engine->executor.getForUnitTest(0);

	assertREqualsM("Call@0", (void*)ev0->callback, (void*)turnSparkPinHigh);
	assertEqualsM("ev 0", start + 944, ev0->momentX);
	assertEqualsLM("coil 0", (long)&enginePins.coils[0], (long)((IgnitionEvent*)ev0->param)->outputs[0]);

	scheduling_s *ev1 = engine->executor.getForUnitTest(1);
	assertREqualsM("Call@1", (void*)ev1->callback, (void*)fireSparkAndPrepareNextSchedule);
	assertEqualsM("ev 1", start + 1444, ev1->momentX);
	assertEqualsLM("coil 1", (long)&enginePins.coils[0], (long)((IgnitionEvent*)ev1->param)->outputs[0]);

	}

	engine->executor.clear();

	eth.fireFall(5);
	eth.fireRise(5);
	eth.fireFall(5);
	ASSERT_EQ( 3,  eth.engine.triggerCentral.triggerState.getCurrentIndex()) << "index #3";
	ASSERT_EQ( 4,  engine->executor.size()) << "queue size 3";
	assertEqualsM("ev 3", start + 13333 - 1515, engine->executor.getForUnitTest(0)->momentX);
	assertEqualsM2("ev 5", start + 14277, engine->executor.getForUnitTest(1)->momentX, 2);
	assertEqualsM("3/3", start + 14777, engine->executor.getForUnitTest(2)->momentX);
	engine->executor.clear();

	ASSERT_EQ(5, TRIGGER_SHAPE(triggerIndexByAngle[240]));
	ASSERT_EQ(5, TRIGGER_SHAPE(triggerIndexByAngle[241]));


	eth.fireFall(5);
	ASSERT_EQ( 0,  engine->executor.size()) << "queue size 4.1";


	eth.fireRise(5);
	ASSERT_EQ( 4,  engine->executor.size()) << "queue size 4.2";


	eth.fireRise(5);
	ASSERT_EQ( 4,  engine->executor.size()) << "queue size 4.3";

	assertEqualsM("dwell", 4.5, eth.engine.engineState.dwellAngle);
	assertEqualsM("fuel #3", 4.5450, eth.engine.injectionDuration);
	ASSERT_EQ(1500, eth.engine.rpmCalculator.rpmValue);

	eth.assertInjectorUpEvent("ev 0/2", 0, -4849, 2);


	ASSERT_EQ( 6,  eth.engine.triggerCentral.triggerState.getCurrentIndex()) << "index #4";
	ASSERT_EQ( 4,  engine->executor.size()) << "queue size 4";
	engine->executor.clear();

	eth.fireFall(5);
	ASSERT_EQ( 2,  engine->executor.size()) << "queue size 5";
// todo: assert queue elements
	engine->executor.clear();


	eth.fireRise(5);
	ASSERT_EQ( 2,  engine->executor.size()) << "queue size 6";
	assertEqualsM("6/0", start + 40944, engine->executor.getForUnitTest(0)->momentX);
	assertEqualsM("6/1", start + 41444, engine->executor.getForUnitTest(1)->momentX);
	engine->executor.clear();

	eth.fireFall(5);
	ASSERT_EQ( 0,  engine->executor.size()) << "queue size 7";
	engine->executor.clear();

	eth.fireRise(5 /*ms*/);
	ASSERT_EQ( 4,  engine->executor.size()) << "queue size 8";
	// todo: assert queue elements completely
	assertEqualsM("8/0", start + 53333 - 1515, engine->executor.getForUnitTest(0)->momentX);
	assertEqualsM2("8/1", start + 54277, engine->executor.getForUnitTest(1)->momentX, 0);
	assertEqualsM2("8/2", start + 54777, engine->executor.getForUnitTest(2)->momentX, 0);
	engine->executor.clear();


	eth.fireFall(5);
	ASSERT_EQ( 0,  engine->executor.size()) << "queue size 9";
	engine->executor.clear();


	eth.fireRise(5);
	ASSERT_EQ( 0,  engine->executor.size()) << "queue size 10";
	engine->executor.clear();
}

TEST(misc, testTriggerDecoder) {
	printf("====================================================================================== testTriggerDecoder\r\n");

	persistent_config_s c;
	Engine e(&c);
	TriggerShape * s = &e.triggerCentral.triggerShape;


	persistent_config_s *config = &c;
	Engine *engine = &e;

	engine_configuration_s *engineConfiguration = &c.engineConfiguration;
	board_configuration_s *boardConfiguration = &c.engineConfiguration.bc;

	initializeSkippedToothTriggerShapeExt(s, 2, 0, FOUR_STROKE_CAM_SENSOR);
	assertEqualsM("shape size", s->getSize(), 4);
	ASSERT_EQ(s->wave.switchTimes[0], 0.25);
	ASSERT_EQ(s->wave.switchTimes[1], 0.5);
	ASSERT_EQ(s->wave.switchTimes[2], 0.75);
	ASSERT_EQ(s->wave.switchTimes[3], 1);

	printf("====================================================================================== testTriggerDecoder part 2\r\n");
	testDodgeNeonDecoder();
	testTriggerDecoder2("Dodge Neon 1995", DODGE_NEON_1995, 8, 0.4931, 0.2070);

	testTriggerDecoder2("ford aspire", FORD_ASPIRE_1996, 4, 0.0000, 0.5);

	testTriggerDecoder2("dodge ram", DODGE_RAM, 16, 0.5000, 0.06);

	//testTriggerDecoder2("bmw", BMW_E34, 0, 0.9750, 0.5167);
	testTriggerDecoder2("bmw", BMW_E34, 0, 0.4667, 0.0);


	testTriggerDecoder2("Miata NB", MAZDA_MIATA_NB1, 12, 0.0833, 0.0444);
	testTriggerDecoder2("Miata 2003", MAZDA_MIATA_2003, 3, 0.0444, 0.0);

	printf("====================================================================================== testTriggerDecoder part 3\r\n");
	testTriggerDecoder2("Civic 4/0 both", TEST_CIVIC_4_0_BOTH, 0, 0.5000, 0.0);
	testTriggerDecoder2("Civic 4/0 rise", TEST_CIVIC_4_0_RISE, 0, 0.5000, 0.0);

	testTriggerDecoder2("test 2/1 both", TEST_ISSUE_366_BOTH, 0, 0.2500, 0.0);
	testTriggerDecoder2("test 2/1 rise", TEST_ISSUE_366_RISE, 0, 0.0000, 0.0);

	testTriggerDecoder2("test engine", TEST_ENGINE, 0, 0.7500, 0.2500);
	testTriggerDecoder2("testGY6_139QMB", GY6_139QMB, 0, 0.4375, 0.0);
	testTriggerDecoder2("testSubary", SUBARU_2003_WRX, 0, 0.4000, 0.0);

	testTriggerDecoder2("testFordEscortGt", FORD_ESCORT_GT, 0, 0.8096, 0.3844);
	testTriggerDecoder2("testRoverV8", ROVER_V8, 0, 0.4861, 0);

	testTriggerDecoder2("NISSAN_PRIMERA", NISSAN_PRIMERA, 2, 0.9611, 0.0);

	testTriggerDecoder2("test1+1", DEFAULT_FRANKENSO, 0, 0.7500, 0.2500);

	testTriggerDecoder2("testCitroen", CITROEN_TU3JP, 0, 0.4833, 0);
	testTriggerDecoder2("testAccordCd 3w", HONDA_ACCORD_CD, 12, 0.8146, 0.5000);
	testTriggerDecoder2("testAccordCd 2w", HONDA_ACCORD_CD_TWO_WIRES, 2, 0.9167, 0.5);
	testTriggerDecoder2("testAccordCdDip", HONDA_ACCORD_CD_DIP, 27, 0.5000, 0.5000);

	testTriggerDecoder2("testMitsu", MITSU_4G93, 0, 0.3553, 0.3752);
	{
		WITH_ENGINE_TEST_HELPER(MITSU_4G93);


//		TriggerShape *t = &eth.engine.triggerShape;
//		ASSERT_EQ(1, t->eventAngles[1]);
//		ASSERT_EQ( 0,  t->triggerIndexByAngle[56]) << "index at 0";
//		ASSERT_EQ( 1,  t->triggerIndexByAngle[57]) << "index at 1";
//
//		ASSERT_EQ(270, t->eventAngles[5]);
//		ASSERT_EQ( 4,  t->triggerIndexByAngle[269]) << "index at 269";
//		ASSERT_EQ( 5,  t->triggerIndexByAngle[270]) << "index at 270";
//		ASSERT_EQ( 5,  t->triggerIndexByAngle[271]) << "index at 271";
//
//		ASSERT_EQ(306, t->eventAngles[6]);
//		ASSERT_EQ(5, t->triggerIndexByAngle[305]);
//		ASSERT_EQ(6, t->triggerIndexByAngle[306]);
//		ASSERT_EQ(6, t->triggerIndexByAngle[307]);
//
//		ASSERT_EQ(666, t->eventAngles[11]);
//		ASSERT_EQ( 10,  t->triggerIndexByAngle[665]) << "index for 665";
//		ASSERT_EQ( 11,  t->triggerIndexByAngle[668]) << "index for 668";


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
	testTriggerDecoder2("CAMARO_4", CAMARO_4, 34, 0.5, 0);

	testTriggerDecoder3("neon NGC4", DODGE_NEON_2003_CAM, 6, 0.5000, 0.0, CHRYSLER_NGC4_GAP);

	{
		printTriggerDebug = true;

		WITH_ENGINE_TEST_HELPER(DODGE_NEON_2003_CAM);

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
}

extern fuel_Map3D_t fuelMap;

static void assertInjectionEvent(const char *msg, InjectionEvent *ev, int injectorIndex, int eventIndex, angle_t angleOffset, bool isOverlapping) {
	assertEqualsM4(msg, "inj index", injectorIndex, ev->outputs[0]->injectorIndex);
	assertEqualsM4(msg, " event index", eventIndex, ev->injectionStart.eventIndex);
	assertEqualsM4(msg, " event offset", angleOffset, ev->injectionStart.angleOffset);
}

static void setTestBug299(EngineTestHelper *eth) {
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(eth);
	Engine *engine = &eth->engine;
	EXPAND_Engine


	eth->assertRpm(0, "RPM=0");
	ASSERT_EQ( 0,  getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE)) << "setTestBug299 EL";
	ASSERT_NEAR( 30,  engine->sensors.iat, EPS4D) << "setTestBug299 IAT";
	eth->fireTriggerEventsWithDuration(20);
	// still no RPM since need to cycles measure cycle duration
	eth->assertRpm(0, "setTestBug299: RPM#1");
	eth->fireTriggerEventsWithDuration(20);
	eth->assertRpm(3000, "setTestBug299: RPM#2");

	eth->clearQueue();

	/**
	 * Trigger up - scheduling fuel for full engine cycle
	 */
	eth->fireRise(20);
	// fuel schedule - short pulses.
	// time...|0.......|10......|20......|30......|40
	// inj #0 |.......#|........|.......#|........|
	// inj #1 |........|.......#|........|.......#|
	ASSERT_EQ( 4,  engine->executor.size()) << "qs#00";
	ASSERT_EQ( 3,  engine->rpmCalculator.getRevolutionCounter()) << "rev cnt#3";
	eth->assertInjectorUpEvent("setTestBug299: 1@0", 0, MS2US(8.5), 0);
	eth->assertInjectorDownEvent("@1", 1, MS2US(10), 0);
	eth->assertInjectorUpEvent("1@2", 2, MS2US(18.5), 1);
	eth->assertInjectorDownEvent("1@3", 3, MS2US(20), 1);
	ASSERT_EQ( 0,  eth->executeActions()) << "exec#0";

	FuelSchedule * t = &ENGINE(injectionEvents);

	assertInjectionEvent("#0", &t->elements[0], 0, 1, 153, false);
	assertInjectionEvent("#1_i_@", &t->elements[1], 1, 1, 333, false);
	assertInjectionEvent("#2@", &t->elements[2], 0, 0, 153, false);
	assertInjectionEvent("inj#3@", &t->elements[3], 1, 0, 153 + 180, false);

	/**
	 * Trigger down - no new events, executing some
	 */
	eth->fireFall(20);
	// same exact picture
	// time...|-20.....|-10.....|0.......|10......|20
	// inj #0 |.......#|........|.......#|........|
	// inj #1 |........|.......#|........|.......#|
	ASSERT_EQ( 8,  engine->executor.size()) << "qs#0";
	ASSERT_EQ( 3,  engine->rpmCalculator.getRevolutionCounter()) << "rev cnt#3";
	eth->assertInjectorUpEvent("02@0", 0, MS2US(-11.5), 0);
	eth->assertInjectorDownEvent("@1", 1, MS2US(-10), 0);
	eth->assertInjectorUpEvent("@2", 2, MS2US(-1.5), 1);
	eth->assertInjectorDownEvent("02@3", 3, MS2US(0), 1);
	eth->assertInjectorUpEvent("02@4", 4, MS2US(8.5), 0);
	eth->assertInjectorDownEvent("@5", 5, MS2US(10), 0);
	eth->assertInjectorUpEvent("02@6", 6, MS2US(18.5), 1);
	eth->assertInjectorDownEvent("@7", 7, MS2US(20), 1);
	ASSERT_EQ( 4,  eth->executeActions()) << "exec#1";


	/**
	 * Trigger up again
	 */
	eth->moveTimeForwardMs(20 /*ms*/);
	eth->assertInjectorUpEvent("22@0", 0, MS2US(-11.5), 0);
	eth->assertInjectorDownEvent("22@1", 1, MS2US(-10), 0);
	eth->assertInjectorUpEvent("22@2", 2, MS2US(-1.5), 1);
	eth->assertInjectorDownEvent("22@3", 3, MS2US(0), 1);
	ASSERT_EQ( 4,  eth->executeActions()) << "exec#20";

	eth->firePrimaryTriggerRise();
	ASSERT_EQ( 4,  engine->executor.size()) << "qs#0-2";
	// fuel schedule - short pulses. and more realistic schedule this time
	// time...|-20.....|-10.....|0.......|10......|20
	// inj #0 |.......#|........|.......#|........|
	// inj #1 |........|.......#|........|.......#|
	eth->assertInjectorUpEvent("2@0", 0, MS2US(8.5), 0);
	eth->assertInjectorDownEvent("@1", 1, MS2US(10), 0);
	eth->assertInjectorUpEvent("@2", 2, MS2US(18.5), 1);
	eth->assertInjectorDownEvent("2@3", 3, MS2US(20), 1);
	ASSERT_EQ( 0,  eth->executeActions()) << "exec#2";


	eth->moveTimeForwardUs(MS2US(20));
	eth->executeActions();
	eth->firePrimaryTriggerFall();
	// fuel schedule - short pulses. and more realistic schedule this time
	// time...|-20.....|-10.....|0.......|10......|20
	// inj #0 |.......#|........|........|........|
	// inj #1 |........|.......#|........|........|
	ASSERT_EQ( 4,  engine->executor.size()) << "qs#0-2";
	ASSERT_EQ( 4,  engine->rpmCalculator.getRevolutionCounter()) << "rev cnt#4";
	eth->assertInjectorUpEvent("0@0", 0, MS2US(8.5), 0);
	eth->assertInjectorDownEvent("0@1", 1, MS2US(10), 0);
	eth->assertInjectorUpEvent("0@2", 2, MS2US(18.5), 1);
	eth->assertInjectorDownEvent("0@3", 3, MS2US(20), 1);
	ASSERT_EQ( 0,  eth->executeActions()) << "exec#3";


	ASSERT_EQ( 1,  engine->engineState.iatFuelCorrection) << "iatC";
	ASSERT_EQ( 1,  engine->engineState.cltFuelCorrection) << "cltC";
	ASSERT_EQ( 0,  engine->engineState.injectorLag) << "lag";

	testMafValue = 0;
	ASSERT_EQ( 0,  getMafVoltage(PASS_ENGINE_PARAMETER_SIGNATURE)) << "maf";

	ASSERT_EQ( 3000,  GET_RPM()) << "setTestBug299: RPM";

	assertEqualsM("fuel#1", 1.5, engine->injectionDuration);
	assertEqualsM("duty for maf=0", 7.5, getInjectorDutyCycle(GET_RPM() PASS_ENGINE_PARAMETER_SUFFIX));

	testMafValue = 3;
	ASSERT_EQ( 3,  getMafVoltage(PASS_ENGINE_PARAMETER_SIGNATURE)) << "maf";
}

static void assertInjectors(const char *msg, int value0, int value1) {
	assertEqualsM4(msg, "inj#0", value0, enginePins.injectors[0].currentLogicValue);
	assertEqualsM4(msg, "inj#1", value1, enginePins.injectors[1].currentLogicValue);
}

void doTestFuelSchedulerBug299smallAndMedium(int startUpDelayMs) {
	printf("*************************************************** testFuelSchedulerBug299 small to medium\r\n");

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	eth.moveTimeForwardMs(startUpDelayMs); // nice to know that same test works the same with different anount of idle time on start
	setTestBug299(&eth);

	FuelSchedule * t;

	assertInjectors("#0_inj", 0, 0);


	int engineLoadIndex = findIndex(config->fuelLoadBins, FUEL_LOAD_COUNT, testMafValue);
	ASSERT_EQ(8, engineLoadIndex);
	setArrayValues(fuelMap.pointers[engineLoadIndex], FUEL_RPM_COUNT, 25);
	setArrayValues(fuelMap.pointers[engineLoadIndex + 1], FUEL_RPM_COUNT, 25);

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_FLOAT_EQ(12.5, engine->injectionDuration) << "fuel#2_0";
	assertEqualsM("duty for maf=3", 62.5, getInjectorDutyCycle(GET_RPM() PASS_ENGINE_PARAMETER_SUFFIX));

	ASSERT_EQ( 4,  engine->executor.size()) << "qs#1";
	eth.moveTimeForwardUs(MS2US(20));
	ASSERT_EQ( 4,  eth.executeActions()) << "exec#2#0";
	ASSERT_EQ( 0,  engine->executor.size()) << "qs#1#2";


	ASSERT_EQ( 4,  engine->rpmCalculator.getRevolutionCounter()) << "rev cnt#4#0";
	eth.firePrimaryTriggerRise();
	ASSERT_EQ( 5,  engine->rpmCalculator.getRevolutionCounter()) << "rev cnt#4#1";
	// time...|0.......|10......|20......|30......|40......|50......|60......|
	// inj #0 |########|##...###|########|.....###|########|........|........|
	// inj #1 |.....###|########|....####|########|........|........|........|
	ASSERT_EQ( 6,  engine->executor.size()) << "qs#4";
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
//		scheduling_s *ev = engine->executor.getForUnitTest(9);
//		ASSERT_EQ( 5,  engine->rpmCalculator.getRevolutionCounter()) << "rev cnt#4#2";
//		ASSERT_TRUE(ev == &engineConfiguration->fuelActuators[2].signalPair[1].signalTimerDown) << "down 50";
//	}


	ASSERT_EQ( 0,  eth.executeActions()) << "exec#4";


	eth.fireFall(20);
	ASSERT_EQ( 8,  engine->executor.size()) << "qs#2#1";
	ASSERT_EQ( 5,  engine->rpmCalculator.getRevolutionCounter()) << "rev cnt#5";
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
	ASSERT_EQ( 3,  eth.executeActions()) << "exec#5";

	/**
	 * one more revolution
	 */
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);


	t = &ENGINE(injectionEvents);

	assertInjectionEvent("#0", &t->elements[0], 0, 0, 315, false);
	assertInjectionEvent("#1__", &t->elements[1], 1, 1, 135, false);
	assertInjectionEvent("inj#2", &t->elements[2], 0, 0, 153, false);
	assertInjectionEvent("inj#3", &t->elements[3], 1, 0, 333, false);

	eth.moveTimeForwardUs(MS2US(20));
	ASSERT_EQ( 5,  engine->executor.size()) << "qs#02";
//	assertInjectorUpEvent("6@0", 0, MS2US(-12.5), 1);
//	assertInjectorDownEvent("6@1", 1, MS2US(-10.0), 0);
//	assertInjectorUpEvent("6@2", 2, MS2US(-2.5), 0);
//	assertInjectorDownEvent("6@3", 3, MS2US(0), 1);
//	assertInjectorDownEvent("6@4", 4, MS2US(10.0), 0);

	// so placing this 'executeAll' changes much?
	ASSERT_EQ( 5,  eth.executeActions()) << "exec#07";
	ASSERT_EQ( 0,  engine->executor.size()) << "qs#07";
//	assertInjectorDownEvent("26@0", 0, MS2US(10.0), 0);

	eth.firePrimaryTriggerRise();
	ASSERT_EQ( 4,  engine->executor.size()) << "qs#2#2";
	ASSERT_EQ( 6,  engine->rpmCalculator.getRevolutionCounter()) << "rev cnt6";
	// time...|-20.....|-10.....|0.......|10......|20......|30......|40......|
	// inj #0 |########|.....###|########|....####|........|........|........|
	// inj #1 |.....###|########|.....###|########|........|........|........|
//	assertInjectorDownEvent("06@5", 5, MS2US(30.0), 0);
//	assertInjectorUpEvent("06@6", 6, MS2US(37.5), 0);
//	assertInjectorDownEvent("06@7", 7, MS2US(40.0), 1);

	ASSERT_EQ( 0,  eth.executeActions()) << "exec#7";

	assertInjectors("#1_ij_", 0, 0);

	eth.moveTimeForwardUs(MS2US(20));

	// time...|-20.....|-10.....|0.......|10......|20......|30......|40......|
	// inj #0 |########|.......#|........|........|........|........|........|
	// inj #1 |....####|########|........|........|........|........|........|
	ASSERT_EQ( 4,  engine->executor.size()) << "qs#022";
//	assertInjectorUpEvent("7@0", 0, MS2US(-12.5), 1);
//	assertInjectorDownEvent("7@1", 1, MS2US(-10.0), 0);
//	assertInjectorUpEvent("7@2", 2, MS2US(-2.5), 0);
//	assertInjectorDownEvent("7@3", 3, MS2US(0), 1);
//	assertInjectorDownEvent("7@4", 4, MS2US(10), 0);
////	assertInjectorDownEvent("i7@5", 5, MS2US(20.0), 0);
////	assertInjectorUpEvent("7@6", 6, MS2US(17.5), 0);
////	assertInjectorDownEvent("7@7", 7, MS2US(20), 1);
//	// todo index 8

	ASSERT_EQ( 3,  eth.executeActions()) << "executed #06";
	assertInjectors("#4", 1, 0);
	ASSERT_EQ( 1,  engine->executor.size()) << "qs#06";
	eth.assertInjectorDownEvent("17@0", 0, MS2US(10), 0);
//	assertInjectorDownEvent("17@1", 1, MS2US(10.0), 0);
//	assertInjectorUpEvent("17@2", 2, MS2US(17.5), 0);
//	assertInjectorDownEvent("17@3", 3, MS2US(20), 1);
	// todo index 4

	eth.firePrimaryTriggerFall();

	ASSERT_EQ( 5,  engine->executor.size()) << "qs#3";
	ASSERT_EQ( 6,  engine->rpmCalculator.getRevolutionCounter()) << "rev cnt6";
	ASSERT_EQ( 0,  eth.executeActions()) << "executed #6";


	eth.moveTimeForwardUs(MS2US(20));
	ASSERT_EQ( 4,  eth.executeActions()) << "executed #06";
	ASSERT_EQ( 1,  engine->executor.size()) << "qs#06";
	assertInjectors("inj#2", 1, 0);

	eth.firePrimaryTriggerRise();
	ASSERT_EQ( 5,  engine->executor.size()) << "Queue.size#03";
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	eth.assertInjectorUpEvent("07@0", 0, MS2US(7.5), 1);
	eth.assertInjectorDownEvent("07@1", 1, MS2US(10), 0);
	eth.assertInjectorUpEvent("07@2", 2, MS2US(17.5), 0);
	eth.assertInjectorDownEvent("07@3", 3, MS2US(20), 1);
	eth.assertInjectorDownEvent("07@4", 4, MS2US(30), 0);
//	assertInjectorDownEvent("07@5", 5, MS2US(30), 0);
//	assertInjectorUpEvent("07@6", 6, MS2US(37.5), 0);
//	assertInjectorDownEvent("07@7", 7, MS2US(40), 1);
//	assertInjectorDownEvent("07@8", 8, MS2US(50), 0);

	ASSERT_EQ( 0,  eth.executeActions()) << "executeAll#3";
	eth.moveTimeForwardUs(MS2US(20));
	ASSERT_EQ( 4,  eth.executeActions()) << "executeAll#4";

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
	assertEqualsM("duty for maf=3", 87.5, getInjectorDutyCycle(GET_RPM() PASS_ENGINE_PARAMETER_SUFFIX));


	assertInjectionEvent("#03", &t->elements[0], 0, 0, 315, false);

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);


	ASSERT_EQ( 1,  enginePins.injectors[0].currentLogicValue) << "inj#0";

	ASSERT_EQ( 1,  engine->executor.size()) << "Queue.size#04";
	eth.assertInjectorDownEvent("08@0", 0, MS2US(10), 0);
//	assertInjectorDownEvent("08@1", 1, MS2US(10), 0);
//	assertInjectorUpEvent("08@2", 2, MS2US(17.5), 0);
//	assertInjectorDownEvent("08@3", 3, MS2US(20), 1);
//	assertInjectorDownEvent("08@4", 4, MS2US(30), 0);

	eth.firePrimaryTriggerFall();


	eth.executeActions();
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);


	eth.fireRise(20);
	ASSERT_EQ( 7,  engine->executor.size()) << "Queue.size#05";
	eth.executeActions();


	eth.fireFall(20);
	eth.executeActions();

	eth.moveTimeForwardUs(MS2US(20));
	eth.executeActions();
	eth.firePrimaryTriggerRise();

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	t = &ENGINE(injectionEvents);

	assertInjectionEvent("#00", &t->elements[0], 0, 0, 225, false); // 87.5 duty cycle
	assertInjectionEvent("#10", &t->elements[1], 1, 1, 45, false);
	assertInjectionEvent("#20", &t->elements[2], 0, 1, 225, true);
	assertInjectionEvent("#30", &t->elements[3], 1, 0, 45, false);

	 // todo: what's what? a mix of new something and old something?
	ASSERT_EQ( 4,  engine->executor.size()) << "qs#5";
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

	eth.executeActions();

	engine->mockMapValue = 0;
	testMafValue = 0;
	ASSERT_EQ( 1,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testFuelSchedulerBug299smallAndMedium";
	ASSERT_EQ(CUSTOM_OBD_SKIPPED_FUEL, unitTestWarningCodeState.recentWarnings.get(0));
}

static void setInjectionMode(int value DECLARE_ENGINE_PARAMETER_SUFFIX) {
	engineConfiguration->injectionMode = (injection_mode_e) value;
	incrementGlobalConfigurationVersion(PASS_ENGINE_PARAMETER_SIGNATURE);
}

TEST(big, testFuelSchedulerBug299smallAndMedium) {
	doTestFuelSchedulerBug299smallAndMedium(0);
	doTestFuelSchedulerBug299smallAndMedium(1000);
}

TEST(big, testDifferentInjectionModes) {
	printf("*************************************************** testDifferentInjectionModes\r\n");

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	setTestBug299(&eth);
	ASSERT_EQ( 4,  engine->executor.size()) << "Lqs#0";

	// set fuel map values - extract method?
	int engineLoadIndex = findIndex(config->fuelLoadBins, FUEL_LOAD_COUNT, testMafValue);
	ASSERT_EQ(8, engineLoadIndex);
	setArrayValues(fuelMap.pointers[engineLoadIndex], FUEL_RPM_COUNT, 40);
	setArrayValues(fuelMap.pointers[engineLoadIndex + 1], FUEL_RPM_COUNT, 40);

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("injectionMode IM_BATCH", (int)IM_BATCH, (int)engineConfiguration->injectionMode);
	ASSERT_EQ( 20,  engine->injectionDuration) << "injection while batch";

	setInjectionMode((int)IM_SIMULTANEOUS PASS_ENGINE_PARAMETER_SUFFIX);
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 10,  engine->injectionDuration) << "injection while simultaneous";

	setInjectionMode((int)IM_SEQUENTIAL PASS_ENGINE_PARAMETER_SUFFIX);
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 40,  engine->injectionDuration) << "injection while IM_SEQUENTIAL";

	setInjectionMode((int)IM_SINGLE_POINT PASS_ENGINE_PARAMETER_SUFFIX);
	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 40,  engine->injectionDuration) << "injection while IM_SINGLE_POINT";
	ASSERT_EQ( 0,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testDifferentInjectionModes";
}

TEST(big, testFuelSchedulerBug299smallAndLarge) {
	printf("*************************************************** testFuelSchedulerBug299 small to large\r\n");

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	setTestBug299(&eth);
	ASSERT_EQ( 4,  engine->executor.size()) << "Lqs#0";

	// set fuel map values - extract method?
	int engineLoadIndex = findIndex(config->fuelLoadBins, FUEL_LOAD_COUNT, testMafValue);
	ASSERT_EQ(8, engineLoadIndex);
	setArrayValues(fuelMap.pointers[engineLoadIndex], FUEL_RPM_COUNT, 35);
	setArrayValues(fuelMap.pointers[engineLoadIndex + 1], FUEL_RPM_COUNT, 35);

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_FLOAT_EQ(17.5, engine->injectionDuration) << "Lfuel#2_1";
	assertEqualsM("Lduty for maf=3", 87.5, getInjectorDutyCycle(GET_RPM() PASS_ENGINE_PARAMETER_SUFFIX));


	ASSERT_EQ( 4,  engine->executor.size()) << "Lqs#1";
	eth.moveTimeForwardUs(MS2US(20));
	eth.executeActions();

	// injector #1 is low before the test
	ASSERT_FALSE(enginePins.injectors[0].currentLogicValue) << "injector@0";

	eth.firePrimaryTriggerRise();

	// time...|0.......|10......|20......|30......|40......|50......|60......|
	// inj #0 |########|########|########|.....###|########|........|........|
	// inj #1 |..######|########|....####|########|........|........|........|
	ASSERT_EQ( 6,  engine->executor.size()) << "Lqs#4";
	eth.assertInjectorUpEvent("L04@0", 0, MS2US(8.5), 0);
	eth.assertInjectorUpEvent("L04@1", 1, MS2US(12.5), 0);
	// special overlapping injection is merged with one of the scheduled injections
	eth.assertInjectorUpEvent("L04@2", 2, MS2US(18.5), 1);

	eth.assertInjectorDownEvent("L04@3", 3, MS2US(26), 0);
	eth.assertInjectorDownEvent("L04@4", 4, MS2US(30), 0);

//	assertInjectorDownEvent("L04@5", 5, MS2US(30), 0);
//	assertInjectorUpEvent("L04@6", 6, MS2US(32.5), 0);
//	assertInjectorDownEvent("L04@7", 7, MS2US(40.0), 1);
//	assertInjectorDownEvent("L04@8", 8, MS2US(50.0), 0);


	engine->executor.executeAll(eth.getTimeNowUs() + 1);
	// injector goes high...
	ASSERT_FALSE(enginePins.injectors[0].currentLogicValue) << "injector@1";

	engine->executor.executeAll(eth.getTimeNowUs() + MS2US(17.5) + 1);
	// injector does not go low too soon, that's a feature :)
	ASSERT_TRUE(enginePins.injectors[0].currentLogicValue) << "injector@2";


	eth.fireFall(20);

	ASSERT_EQ( 6,  engine->executor.size()) << "Lqs#04";
	eth.assertInjectorUpEvent("L015@0", 0, MS2US(-1.5), 1);
	eth.assertInjectorUpEvent("L015@1", 1, MS2US(2.5), 1);
	eth.assertInjectorDownEvent("L015@2", 2, MS2US(6), 0);
	eth.assertInjectorDownEvent("L015@3", 3, MS2US(10), 0);
	eth.assertInjectorDownEvent("L015@4", 4, MS2US(16), 1);
//todo	assertInjectorDownEvent("L015@5", 5, MS2US(30), 0);


	engine->executor.executeAll(eth.getTimeNowUs() + MS2US(10) + 1);
	// end of combined injection
	ASSERT_FALSE(enginePins.injectors[0].currentLogicValue) << "injector@3";


	eth.moveTimeForwardUs(MS2US(20));
	eth.executeActions();
	ASSERT_EQ( 0,  engine->executor.size()) << "Lqs#04";

	setArrayValues(fuelMap.pointers[engineLoadIndex], FUEL_RPM_COUNT, 4);
	setArrayValues(fuelMap.pointers[engineLoadIndex + 1], FUEL_RPM_COUNT, 4);

	engine->periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	ASSERT_EQ( 2,  engine->injectionDuration) << "Lfuel#4";
	ASSERT_EQ( 10,  getInjectorDutyCycle(GET_RPM() PASS_ENGINE_PARAMETER_SUFFIX)) << "Lduty for maf=3";


	eth.firePrimaryTriggerRise();

	//todoASSERT_EQ( 5,  engine->executor.size()) << "Lqs#05";
	//todo	assertInjectorUpEvent("L016@0", 0, MS2US(8), 0);
	//todo	assertInjectorDownEvent("L016@1", 1, MS2US(10), 0);
	//todo	assertInjectorDownEvent("L016@2", 2, MS2US(10), 0);


	eth.moveTimeForwardUs(MS2US(20));
	eth.executeActions(); // issue here
	eth.firePrimaryTriggerFall();


	eth.moveTimeForwardUs(MS2US(20));
	eth.executeActions();
	eth.firePrimaryTriggerRise();

	ASSERT_EQ( 4,  engine->executor.size()) << "Lqs#5";
	eth.assertInjectorUpEvent("L05@0", 0, MS2US(8), 0);
	eth.assertInjectorDownEvent("L05@1", 1, MS2US(10), 0);
	eth.assertInjectorUpEvent("L05@2", 2, MS2US(18), 1);
	eth.assertInjectorDownEvent("L05@3", 3, MS2US(20), 1);

	eth.moveTimeForwardUs(MS2US(20));
	eth.executeActions();
	ASSERT_EQ( 1,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#testFuelSchedulerBug299smallAndLarge";
	ASSERT_EQ(CUSTOM_OBD_SKIPPED_FUEL, unitTestWarningCodeState.recentWarnings.get(0));
}

TEST(big, testSparkReverseOrderBug319) {
	printf("*************************************************** testSparkReverseOrderBug319 small to medium\r\n");

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	engineConfiguration->useOnlyRisingEdgeForTrigger = false;
	engineConfiguration->isInjectionEnabled = false;
	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	setConstantDwell(45 PASS_CONFIG_PARAMETER_SUFFIX);

	// this is needed to update injectorLag
	engine->updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);

	ASSERT_NEAR( 70,  engine->sensors.clt, EPS4D) << "CLT";

	eth.setTriggerType(TT_ONE PASS_ENGINE_PARAMETER_SUFFIX);
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);

	setWholeTimingTable(0);

	eth.fireRise(20);
	eth.fireFall(20);

	eth.executeActions();

	eth.fireRise(20);
	eth.fireFall(20);

	ASSERT_EQ( 3000,  GET_RPM()) << "testSparkReverseOrderBug319: RPM";


	ASSERT_EQ( 7,  engine->executor.size()) << "testSparkReverseOrderBug319: queue size";
	eth.executeActions();
	printf("***************************************************\r\n");


	eth.fireRise(20);
	eth.executeActions();

	/**
	 * here we throw scheduling logic off
	 */
	eth.fireFall(0.1); // executing new signal too early
	eth.executeActions();

	ASSERT_EQ( 1,  enginePins.coils[3].outOfOrder) << "out-of-order #1";


	eth.moveTimeForwardUs(MS2US(200)); // moving time forward to execute all pending actions
	eth.executeActions();

	ASSERT_EQ( 0,  enginePins.coils[3].outOfOrder) << "out-of-order #2";

	printf("*************************************************** now let's have a good engine cycle and confirm things work\r\n");


	eth.fireRise(20);
	eth.executeActions();

	ASSERT_EQ( 545,  GET_RPM()) << "RPM#2";

	ASSERT_EQ( 0,  enginePins.coils[3].outOfOrder) << "out-of-order #3";


	eth.fireFall(20);
	eth.executeActions();
	ASSERT_EQ( 1,  enginePins.coils[3].outOfOrder) << "out-of-order #4";

	printf("*************************************************** (rpm is back) now let's have a good engine cycle and confirm things work\r\n");

	eth.fireRise(20);
	eth.executeActions();

	ASSERT_EQ( 3000,  GET_RPM()) << "RPM#3";

	ASSERT_EQ( 1,  enginePins.coils[3].outOfOrder) << "out-of-order #5 on c4";


	eth.fireFall(20);
	eth.executeActions();
	ASSERT_EQ( 1,  enginePins.coils[3].outOfOrder) << "out-of-order #6 on c4";

	printf("*************************************************** (rpm is back 2) now let's have a good engine cycle and confirm things work\r\n");

	eth.fireRise(20);
	eth.executeActions();

	ASSERT_EQ( 3000,  GET_RPM()) << "RPM#4";

	ASSERT_EQ( 1,  enginePins.coils[3].outOfOrder) << "out-of-order #7";


	eth.fireFall(20);
	eth.executeActions();
	ASSERT_EQ( 0,  enginePins.coils[3].outOfOrder) << "out-of-order #8";
	ASSERT_EQ( 2,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#SparkReverseOrderBug319";
	ASSERT_EQ(CUSTOM_DWELL_TOO_LONG, unitTestWarningCodeState.recentWarnings.get(0)) << "warning @0";
	ASSERT_EQ(CUSTOM_OUT_OF_ORDER_COIL, unitTestWarningCodeState.recentWarnings.get(1));
}

TEST(big, testMissedSpark299) {
	printf("*************************************************** testMissedSpark299\r\n");

	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->useOnlyRisingEdgeForTrigger = false;
	setupSimpleTestEngineWithMafAndTT_ONE_trigger(&eth);
	engineConfiguration->isIgnitionEnabled = true;
	engineConfiguration->isInjectionEnabled = false;

	ASSERT_EQ( 0,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#0";


	eth.fireRise(20);
	eth.executeActions();
	ASSERT_EQ( 0,  eth.engine.triggerCentral.triggerState.currentCycle.current_index) << "ci#0";


	eth.fireFall(20);
	eth.executeActions();
	ASSERT_EQ( 1,  eth.engine.triggerCentral.triggerState.currentCycle.current_index) << "ci#1";


	eth.fireRise(20);
	eth.executeActions();
	ASSERT_EQ( 0,  eth.engine.triggerCentral.triggerState.currentCycle.current_index) << "ci#2";


	eth.fireFall(20);
	eth.executeActions();
	ASSERT_EQ( 1,  eth.engine.triggerCentral.triggerState.currentCycle.current_index) << "ci#3";


	eth.fireRise(20);
	eth.executeActions();


	eth.fireFall(20);
	eth.executeActions();
	ASSERT_EQ( 1,  eth.engine.triggerCentral.triggerState.currentCycle.current_index) << "ci#5";


	printf("*************************************************** testMissedSpark299 start\r\n");

	ASSERT_EQ(3000, eth.engine.rpmCalculator.rpmValue);

	setWholeTimingTable(3);
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);



	eth.fireRise(20);
	eth.executeActions();

	eth.fireFall(20);
	eth.executeActions();


	eth.fireRise(20);
	eth.executeActions();
	eth.fireFall(20);
	eth.executeActions();

	setWholeTimingTable(-5);
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);


	eth.fireRise(20);
	eth.executeActions();

	eth.fireFall(20);
	eth.executeActions();


	eth.fireRise(20);
	eth.executeActions();

	eth.fireFall(20);
	eth.executeActions();

	ASSERT_EQ( 0,  unitTestWarningCodeState.recentWarnings.getCount()) << "warningCounter#1";
}
