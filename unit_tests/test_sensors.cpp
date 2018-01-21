/**
 * @file	test_sensors.cpp
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"
#include "thermistors.h"
#include "allsensors.h"
#include "hip9011_lookup.h"
#include "engine_test_helper.h"

static ThermistorConf tc;

static void testMapDecoding(void) {
	EngineTestHelper eth(FORD_INLINE_6_1995);
	EXPAND_EngineTestHelper;

	air_pressure_sensor_config_s s;
	s.type = MT_DENSO183;

	assertEqualsM("denso 0 volts", -6.64, decodePressure(0, &s PASS_ENGINE_PARAMETER_SUFFIX));
	assertEquals(31.244, decodePressure(1, &s PASS_ENGINE_PARAMETER_SUFFIX));

	s.type = MT_MPX4250;
	assertEqualsM("MPX_4250 0 volts", 8, decodePressure(0, &s PASS_ENGINE_PARAMETER_SUFFIX));
	assertEquals(58.4, decodePressure(1, &s PASS_ENGINE_PARAMETER_SUFFIX));
}

void testTps(void) {
	print("************************************************** testTps\r\n");

	EngineTestHelper eth(DODGE_RAM);
	EXPAND_EngineTestHelper;

	engineConfiguration->tpsMax = 193;
	engineConfiguration->tpsMin = 43;

	assertEquals(49.3333, getTpsValue(4 * 117 PASS_ENGINE_PARAMETER_SUFFIX));


	engineConfiguration->tpsMax = 43;
	engineConfiguration->tpsMin = 193;
	assertEqualsM("test#2", 50.6667, getTpsValue(4 * 117 PASS_ENGINE_PARAMETER_SUFFIX));
}

void testTpsRateOfChange(void) {
	print("************************************************** testTpsRateOfChange\r\n");
//	saveTpsState(0, 0);
//	saveTpsState(CH_FREQUENCY, 50);
//	assertEquals(50, getTpsRateOfChange());
//
//	saveTpsState(2 * CH_FREQUENCY, 50);
//	assertEquals(0, getTpsRateOfChange());
//
//	saveTpsState(3 * CH_FREQUENCY, 75);
//	assertEquals(25, getTpsRateOfChange());
}

static void testHip9011lookup(void) {
	print("************************************************** testHip9011lookup\r\n");

	assertEqualsM2("", 3183.1013, getRpmByAngleWindowAndTimeUs(600, 360), 0.1);
	assertEqualsM2("40us", 47746.5195, getRpmByAngleWindowAndTimeUs(40, 360), 0.1);

	assertEqualsM2("600us 50 degree", 442.0974, getRpmByAngleWindowAndTimeUs(600, 50), 0.1);
	assertEqualsM2("240us 50 degree", 1105.2435, getRpmByAngleWindowAndTimeUs(240, 50), 0.1);
	assertEqualsM2("240us 50 degree", 6631.4619, getRpmByAngleWindowAndTimeUs(40, 50), 0.1);

	assertEquals(0, getHip9011GainIndex(3));
	assertEquals(0, getHip9011GainIndex(2));
	assertEquals(47, getHip9011GainIndex(0.234));
	assertEquals(63, getHip9011GainIndex(0.000001));

	prepareHip9011RpmLookup(50);

	assertEquals(31, getIntegrationIndexByRpm(1));
	assertEquals(21, getIntegrationIndexByRpm(1100));
	assertEquals(1, getIntegrationIndexByRpm(6600));
	assertEquals(0, getIntegrationIndexByRpm(16600));
}

void testSensors(void) {
	print("************************************************** testSensors\r\n");
	testMapDecoding();
	testTps();
	testTpsRateOfChange();
	testHip9011lookup();

	ThermistorMath tm;
	{
		setThermistorConfiguration(&tc, 32, 9500, 75, 2100, 120, 1000);
		tm.setConfig(&tc.config);
		float t = tm.getKelvinTemperatureByResistance(2100);
		assertEquals(75 + KELV, t);

		assertEquals(-0.003, tm.s_h_a);
		assertEquals(0.001, tm.s_h_b);
		assertEquals(0.0, tm.s_h_c);

	}

	{
		// 2003 Neon sensor
		setThermistorConfiguration(&tc, 0, 32500, 30, 7550, 100, 700);
		tm.setConfig(&tc.config);

		float t = tm.getKelvinTemperatureByResistance(38000);
		assertEquals(-2.7983, t - KELV);

		assertEqualsM("A", 0.0009, tm.s_h_a);
		assertEqualsM("B", 0.0003, tm.s_h_b);
		assertEquals(0.0, tm.s_h_c);
	}
}

