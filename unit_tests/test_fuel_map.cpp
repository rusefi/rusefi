/**
 * @file	test_fuel_map.cpp
 *
 * @date Nov 6, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "test_fuel_map.h"
#include "main.h"
#include "engine_configuration.h"
#include "fuel_math.h"
#include "trigger_structure.h"
#include "allsensors.h"
#include "engine_math.h"
#include "trigger_decoder.h"
#include "engine_test_helper.h"
#include "efiGpio.h"
#include "advance_map.h"

extern float testMafValue;

void testMafFuelMath(void) {
	printf("*************************************************** testMafFuelMath\r\n");
	EngineTestHelper eth(FORD_ASPIRE_1996);
	EXPAND_EngineTestHelper;

	engineConfiguration->fuelAlgorithm = LM_REAL_MAF;
	engineConfiguration->injector.flow = 200;

	setAfrMap(config->afrTable, 13);

	float fuelMs = getRealMafFuel(300, 6000 PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("fuelMs", 26.7099, fuelMs);
}

void testFuelMap(void) {
	printf("*************************************************** testFuelMap\r\n");

	printf("Setting up FORD_ASPIRE_1996\r\n");
	EngineTestHelper eth(FORD_ASPIRE_1996);
	EXPAND_EngineTestHelper;

	printf("Filling fuel map\r\n");
	for (int k = 0; k < FUEL_LOAD_COUNT; k++) {
		for (int r = 0; r < FUEL_RPM_COUNT; r++) {
			eth.engine.config->fuelTable[k][r] = k * 200 + r;
		}
	}
	for (int i = 0; i < FUEL_LOAD_COUNT; i++)
		eth.engine.config->fuelLoadBins[i] = i;
	for (int i = 0; i < FUEL_RPM_COUNT; i++)
		eth.engine.config->fuelRpmBins[i] = i;

	assertEqualsM("base fuel table", 1005, getBaseTableFuel(5, 5));

	printf("*************************************************** initThermistors\r\n");


	printf("*** getInjectorLag\r\n");
//	engine->engineState.vb
	assertEqualsM("lag", 1.04, getInjectorLag(12 PASS_ENGINE_PARAMETER_SUFFIX));

	for (int i = 0; i < VBAT_INJECTOR_CURVE_SIZE; i++) {
		eth.engine.engineConfiguration->injector.battLagCorrBins[i] = i;
		eth.engine.engineConfiguration->injector.battLagCorr[i] = 0.5 + 2 * i;
	}

	eth.engine.updateSlowSensors(PASS_ENGINE_PARAMETER_SIGNATURE);

	// because all the correction tables are zero
	printf("*************************************************** getRunningFuel 1\r\n");
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	float baseFuel = getBaseTableFuel(5, getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE));
	assertEqualsM("base fuel", 5.05, getRunningFuel(baseFuel PASS_ENGINE_PARAMETER_SUFFIX));

	printf("*************************************************** setting IAT table\r\n");
	for (int i = 0; i < IAT_CURVE_SIZE; i++) {
		eth.engine.config->iatFuelCorrBins[i] = i;
		eth.engine.config->iatFuelCorr[i] = 2 * i;
	}
	eth.engine.config->iatFuelCorr[0] = 2;

	printf("*************************************************** setting CLT table\r\n");
	for (int i = 0; i < CLT_CURVE_SIZE; i++) {
		eth.engine.config->cltFuelCorrBins[i] = i;
		eth.engine.config->cltFuelCorr[i] = 100;
	}

	setFlatInjectorLag(0 PASS_ENGINE_PARAMETER_SUFFIX);

	assertEquals(NAN, getIntakeAirTemperature(PASS_ENGINE_PARAMETER_SIGNATURE));
	float iatCorrection = getIatFuelCorrection(-KELV PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("IAT", 2, iatCorrection);
	engine->sensors.clt = getCoolantTemperature(PASS_ENGINE_PARAMETER_SIGNATURE);
	float cltCorrection = getCltFuelCorrection(PASS_ENGINE_PARAMETER_SIGNATURE);
	assertEqualsM("CLT", 1, cltCorrection);
	float injectorLag = getInjectorLag(getVBatt(PASS_ENGINE_PARAMETER_SIGNATURE) PASS_ENGINE_PARAMETER_SUFFIX);
	assertEqualsM("injectorLag", 0, injectorLag);

	testMafValue = 5;

	// 1005 * 2 for IAT correction
	printf("*************************************************** getRunningFuel 2\r\n");
	eth.engine.periodicFastCallback(PASS_ENGINE_PARAMETER_SIGNATURE);
	baseFuel = getBaseTableFuel(5, getEngineLoadT(PASS_ENGINE_PARAMETER_SIGNATURE));
	assertEqualsM("v1", 30150, getRunningFuel(baseFuel PASS_ENGINE_PARAMETER_SUFFIX));

	testMafValue = 0;

	engineConfiguration->cranking.baseFuel = 4;

	printf("*************************************************** getStartingFuel\r\n");
	// NAN in case we have issues with the CLT sensor
	assertEqualsM("getStartingFuel nan", 4, getCrankingFuel3(NAN, 0 PASS_ENGINE_PARAMETER_SUFFIX));
	assertEqualsM("getStartingFuel#1", 17.8, getCrankingFuel3(0, 4 PASS_ENGINE_PARAMETER_SUFFIX));
	assertEqualsM("getStartingFuel#2", 8.4480, getCrankingFuel3(8, 15 PASS_ENGINE_PARAMETER_SUFFIX));
	assertEqualsM("getStartingFuel#3", 6.48, getCrankingFuel3(70, 0 PASS_ENGINE_PARAMETER_SUFFIX));
	assertEqualsM("getStartingFuel#4", 2.6069, getCrankingFuel3(70, 50 PASS_ENGINE_PARAMETER_SUFFIX));
}


static void confgiureFordAspireTriggerShape(TriggerShape * s DECLARE_ENGINE_PARAMETER_SUFFIX) {
	s->initialize(FOUR_STROKE_CAM_SENSOR, true);

	s->addEvent2(53.747, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(121.90, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(232.76, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(300.54, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(360, T_PRIMARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);

	s->addEvent2(409.8412, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(478.6505, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(588.045, T_SECONDARY, TV_RISE PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(657.03, T_SECONDARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);
	s->addEvent2(720, T_PRIMARY, TV_FALL PASS_ENGINE_PARAMETER_SUFFIX);

	assertEquals(53.747 / 720, s->wave.getSwitchTime(0));
	assertEqualsM("@0", 1, s->wave.getChannelState(1, 0));
	assertEqualsM("@0", 1, s->wave.getChannelState(1, 0));

	assertEqualsM("@1", 0, s->wave.getChannelState(0, 1));
	assertEqualsM("@1", 0, s->wave.getChannelState(1, 1));

	assertEqualsM("@2", 0, s->wave.getChannelState(0, 2));
	assertEqualsM("@2", 1, s->wave.getChannelState(1, 2));

	assertEqualsM("@3", 0, s->wave.getChannelState(0, 3));
	assertEqualsM("@3", 0, s->wave.getChannelState(1, 3));

	assertEqualsM("@4", 1, s->wave.getChannelState(0, 4));
	assertEqualsM("@5", 1, s->wave.getChannelState(1, 5));
	assertEqualsM("@8", 0, s->wave.getChannelState(1, 8));
	assertEquals(121.90 / 720, s->wave.getSwitchTime(1));
	assertEquals(657.03 / 720, s->wave.getSwitchTime(8));

	assertEqualsM("expecting 0", 0, s->wave.findAngleMatch(53.747 / 720.0, s->getSize()));
	assertEqualsM("expecting not found", -1, s->wave.findAngleMatch(53 / 720.0, s->getSize()));
	assertEquals(7, s->wave.findAngleMatch(588.045 / 720.0, s->getSize()));

	assertEqualsM("expecting 0", 0, s->wave.findInsertionAngle(23.747 / 720.0, s->getSize()));
	assertEqualsM("expecting 1", 1, s->wave.findInsertionAngle(63.747 / 720.0, s->getSize()));
}

void testAngleResolver(void) {
	printf("*************************************************** testAngleResolver\r\n");

	EngineTestHelper eth(FORD_ASPIRE_1996);
	EXPAND_EngineTestHelper;

	engineConfiguration->globalTriggerAngleOffset = 175;

	TriggerShape * ts = &engine->triggerCentral.triggerShape;
	ts->initializeTriggerShape(NULL PASS_ENGINE_PARAMETER_SUFFIX);

	assertEqualsM("index 2", 52.76, ts->eventAngles[3]); // this angle is relation to synch point
	assertEqualsM("time 2", 0.3233, ts->wave.getSwitchTime(2));
	assertEqualsM("index 5", 412.76, ts->eventAngles[6]);
	assertEqualsM("time 5", 0.5733, ts->wave.getSwitchTime(5));

	assertEquals(4, ts->getTriggerShapeSynchPointIndex());

	assertEqualsM("shape size", 10, ts->getSize());

	event_trigger_position_s injectionStart;

	printf("*************************************************** testAngleResolver 0\r\n");
	TRIGGER_SHAPE(findTriggerPosition(&injectionStart, -122 PASS_ENGINE_PARAMETER_SUFFIX));
	assertEqualsM("eventIndex@0", 2, injectionStart.eventIndex);
	assertEquals(0.24, injectionStart.angleOffset);

	printf("*************************************************** testAngleResolver 0.1\r\n");
	TRIGGER_SHAPE(findTriggerPosition(&injectionStart, -80 PASS_ENGINE_PARAMETER_SUFFIX));
	assertEqualsM("eventIndex@0", 2, injectionStart.eventIndex);
	assertEquals(42.24, injectionStart.angleOffset);

	printf("*************************************************** testAngleResolver 0.2\r\n");
	TRIGGER_SHAPE(findTriggerPosition(&injectionStart, -54 PASS_ENGINE_PARAMETER_SUFFIX));
	assertEqualsM("eventIndex@0", 2, injectionStart.eventIndex);
	assertEquals(68.2400, injectionStart.angleOffset);

	printf("*************************************************** testAngleResolver 0.3\r\n");
	TRIGGER_SHAPE(findTriggerPosition(&injectionStart, -53 PASS_ENGINE_PARAMETER_SUFFIX));
	assertEquals(2, injectionStart.eventIndex);
	assertEquals(69.24, injectionStart.angleOffset);

	printf("*************************************************** testAngleResolver 1\r\n");
	TRIGGER_SHAPE(findTriggerPosition(&injectionStart, 0 PASS_ENGINE_PARAMETER_SUFFIX));
	assertEquals(2, injectionStart.eventIndex);
	assertEquals(122.24, injectionStart.angleOffset);

	printf("*************************************************** testAngleResolver 2\r\n");
	TRIGGER_SHAPE(findTriggerPosition(&injectionStart, 56 PASS_ENGINE_PARAMETER_SUFFIX));
	assertEquals(2, injectionStart.eventIndex);
	assertEquals(178.24, injectionStart.angleOffset);

	TriggerShape t;
	confgiureFordAspireTriggerShape(&t PASS_ENGINE_PARAMETER_SUFFIX);
}

void testPinHelper(void) {
	printf("*************************************************** testPinHelper\r\n");
	assertEquals(0, getElectricalValue(0, OM_DEFAULT));
	assertEquals(1, getElectricalValue(1, OM_DEFAULT));

	assertEquals(0, getElectricalValue(1, OM_INVERTED));
	assertEquals(1, getElectricalValue(0, OM_INVERTED));
}
