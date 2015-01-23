/**
 * @file	test_sensors.c
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "thermistors.h"
#include "allsensors.h"
#include "hip9011_lookup.h"

static ThermistorConf tc;

static void testMapDecoding(void) {

	air_pressure_sensor_config_s s;
	s.type = MT_DENSO183;

	assertEqualsM("denso 0 volts", -6.64, decodePressure(0, &s));
	assertEquals(31.244, decodePressure(1, &s));

	s.type = MT_MPX4250;
	assertEqualsM("MPX_4250 0 volts", 8, decodePressure(0, &s));
	assertEquals(58.4, decodePressure(1, &s));
}

void testTpsRateOfChange(void) {
	print("************************************************** testTpsRateOfChange\r\n");
	saveTpsState(0, 0);
	saveTpsState(CH_FREQUENCY, 50);
	assertEquals(50, getTpsRateOfChange());

	saveTpsState(2 * CH_FREQUENCY, 50);
	assertEquals(0, getTpsRateOfChange());

	saveTpsState(3 * CH_FREQUENCY, 75);
	assertEquals(25, getTpsRateOfChange());
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
	testTpsRateOfChange();
	testHip9011lookup();

	setThermistorConfiguration(&tc, 32, 9500, 75, 2100, 120, 1000);

	prepareThermistorCurve(&tc);

	assertEquals(-0.003, tc.s_h_a);
	assertEquals(0.001, tc.s_h_b);
	assertEquals(0.0, tc.s_h_c);

	float t = convertResistanceToKelvinTemperature(2100, &tc);
	assertEquals(75 + KELV, t);
}

