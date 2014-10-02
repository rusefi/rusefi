/**
 * @file	test_fuel_map.cpp
 *
 *  Created on: Nov 6, 2013
 *      Author: Andrey Belomutskiy, (c) 2012-2013
 */

#include "test_fuel_map.h"
#include "main.h"
#include "engine_configuration.h"
#include "fuel_math.h"
#include "trigger_structure.h"
#include "allsensors.h"
#include "engine_math.h"
#include "OutputSignalList.h"
#include "ec2.h"
#include "trigger_decoder.h"
#include "engine_test_helper.h"
#include "efiGpio.h"

extern float testMafValue;

void testFuelMap(void) {
	printf("*************************************************** testFuelMap\r\n");

	EngineTestHelper eth(FORD_ASPIRE_1996);

	for (int k = 0; k < FUEL_LOAD_COUNT; k++) {
		for (int r = 0; r < FUEL_RPM_COUNT; r++) {
			eth.engine.engineConfiguration->fuelTable[k][r] = k * 200 + r;
		}
	}
	for (int i = 0; i < FUEL_LOAD_COUNT; i++)
		eth.engine.engineConfiguration->fuelLoadBins[i] = i;
	for (int i = 0; i < FUEL_RPM_COUNT; i++)
		eth.engine.engineConfiguration->fuelRpmBins[i] = i;

	assertEqualsM("base fuel table", 1005, getBaseTableFuel(eth.engine.engineConfiguration, 5, 5));

	printf("*************************************************** initThermistors\r\n");

	initThermistors(&eth.engine);

	printf("*** getInjectorLag\r\n");
	assertEquals(0, getInjectorLag(eth.engine.engineConfiguration, 12));

	eth.engine.engineConfiguration->injectorLag = 0.5;

	for (int i = 0; i < VBAT_INJECTOR_CURVE_SIZE; i++) {
		eth.engine.engineConfiguration->battInjectorLagCorrBins[i] = i;
		eth.engine.engineConfiguration->battInjectorLagCorr[i] = 2 * i;
	}


	// because all the correction tables are zero
	printf("*************************************************** getRunningFuel 1\r\n");
	float baseFuel = getBaseTableFuel(eth.engine.engineConfiguration, 5, getEngineLoadT(&eth.engine));
	assertEqualsM("base fuel", 5.5, getRunningFuel(baseFuel, &eth.engine, 5));

	printf("*************************************************** setting IAT table\r\n");
	for (int i = 0; i < IAT_CURVE_SIZE; i++) {
		eth.engine.engineConfiguration->iatFuelCorrBins[i] = i;
		eth.engine.engineConfiguration->iatFuelCorr[i] = 2 * i;
	}
	eth.engine.engineConfiguration->iatFuelCorr[0] = 2;

	printf("*************************************************** setting CLT table\r\n");
	for (int i = 0; i < CLT_CURVE_SIZE; i++) {
		eth.engine.engineConfiguration->cltFuelCorrBins[i] = i;
		eth.engine.engineConfiguration->cltFuelCorr[i] = 1;
	}
	eth.engine.engineConfiguration->injectorLag = 0;

	engine_configuration_s *engineConfiguration = eth.engine.engineConfiguration;

	assertEquals(NAN, getIntakeAirTemperature(eth.engine.engineConfiguration2));
	float iatCorrection = getIatCorrection(engineConfiguration, -KELV);
	assertEqualsM("IAT", 2, iatCorrection);
	float cltCorrection = getCltCorrection(engineConfiguration, getCoolantTemperature(eth.engine.engineConfiguration2));
	assertEqualsM("CLT", 1, cltCorrection);
	float injectorLag = getInjectorLag(engineConfiguration, getVBatt());
	assertEquals(0, injectorLag);

	testMafValue = 5;

	// 1005 * 2 for IAT correction
	printf("*************************************************** getRunningFuel 2\r\n");
	 baseFuel = getBaseTableFuel(eth.engine.engineConfiguration, 5, getEngineLoadT(&eth.engine));
	assertEqualsM("v1", 30150, getRunningFuel(baseFuel, &eth.engine, 5));

	testMafValue = 0;

	engineConfiguration->crankingSettings.coolantTempMaxC = 65; // 8ms at 65C
	engineConfiguration->crankingSettings.fuelAtMaxTempMs = 8;

	engineConfiguration->crankingSettings.coolantTempMinC = 0; // 20ms at 0C
	engineConfiguration->crankingSettings.fuelAtMinTempMs = 20;

	printf("*************************************************** getStartingFuel\r\n");
	// NAN in case we have issues with the CLT sensor
//	assertEquals(16, getStartingFuel(NAN));
	assertEquals(20, getStartingFuel(engineConfiguration, 0));
	assertEquals(18.5231, getStartingFuel(engineConfiguration, 8));
	assertEquals(8, getStartingFuel(engineConfiguration, 70));
}

extern engine_configuration_s *engineConfiguration;
extern engine_configuration2_s *engineConfiguration2;

static void confgiureFordAspireTriggerShape(trigger_shape_s * s) {
	s->reset(FOUR_STROKE_CAM_SENSOR);

	s->addEvent(53.747, T_SECONDARY, TV_HIGH);
	s->addEvent(121.90, T_SECONDARY, TV_LOW);
	s->addEvent(232.76, T_SECONDARY, TV_HIGH);
	s->addEvent(300.54, T_SECONDARY, TV_LOW);
	s->addEvent(360, T_PRIMARY, TV_HIGH);

	s->addEvent(409.8412, T_SECONDARY, TV_HIGH);
	s->addEvent(478.6505, T_SECONDARY, TV_LOW);
	s->addEvent(588.045, T_SECONDARY, TV_HIGH);
	s->addEvent(657.03, T_SECONDARY, TV_LOW);
	s->addEvent(720, T_PRIMARY, TV_LOW);

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

	assertEqualsM("expecting 0", 0, s->wave.waveIndertionAngle(23.747 / 720.0, s->getSize()));
	assertEqualsM("expecting 1", 1, s->wave.waveIndertionAngle(63.747 / 720.0, s->getSize()));
}

static ActuatorEventList ae;

void testAngleResolver(void) {
	printf("*************************************************** testAngleResolver\r\n");

	engineConfiguration->globalTriggerAngleOffset = 175;
	trigger_shape_s * ts = &engineConfiguration2->triggerShape;

	confgiureFordAspireTriggerShape(ts);

	ts->calculateTriggerSynchPoint(&engineConfiguration->triggerConfig);

	assertEqualsM("index 2", 232.76, ts->eventAngles[3]); // this angle is relation to synch point
	assertEqualsM("time 2", 0.3233, ts->wave.getSwitchTime(2));
	assertEqualsM("index 5", 409.8412, ts->eventAngles[6]);
	assertEqualsM("time 5", 0.5692, ts->wave.getSwitchTime(5));

	assertEquals(9, ts->getTriggerShapeSynchPointIndex());

	assertEqualsM("shape size", 10, ts->getSize());

	OutputSignalList list;

	ae.resetEventList();
	printf("*************************************************** testAngleResolver 0\r\n");
	registerActuatorEventExt(engineConfiguration, &engineConfiguration2->triggerShape, ae.getNextActuatorEvent(), list.add(INJECTOR_1_OUTPUT), 53 - 175);
	assertEqualsM("size", 1, ae.size);
	assertEquals(1, list.getSize());
	assertEquals(0, ae.events[0].position.eventIndex);
	assertEquals(53, ae.events[0].position.angleOffset);

	printf("*************************************************** testAngleResolver 2\r\n");
	ae.resetEventList();
	registerActuatorEventExt(engineConfiguration, &engineConfiguration2->triggerShape, ae.getNextActuatorEvent(), list.add(INJECTOR_1_OUTPUT), 51 + 180 - 175);
	assertEquals(2, ae.events[0].position.eventIndex);
	assertEquals(109.1, ae.events[0].position.angleOffset);
}

void testPinHelper(void) {
	printf("*************************************************** testPinHelper\r\n");
	assertEquals(0, getElectricalValue(0, OM_DEFAULT));
	assertEquals(1, getElectricalValue(1, OM_DEFAULT));

	assertEquals(0, getElectricalValue(1, OM_INVERTED));
	assertEquals(1, getElectricalValue(0, OM_INVERTED));
}
