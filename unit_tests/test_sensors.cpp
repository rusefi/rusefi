/**
 * @file	test_sensors.c
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2013
 */

#include "main.h"
#include "thermistors.h"
#include "allsensors.h"

static ThermistorConf tc;

static void testMapDecoding(void) {

	air_pressure_sensor_config_s s;
	s.sensorType = MT_DENSO183;

	assertEqualsM("denso 0 volts", -6.64, decodePressure(0, &s));
	assertEquals(31.244, decodePressure(1, &s));

	s.sensorType = MT_MPX4250;
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

void testSensors(void) {
	print("************************************************** testSensors\r\n");
	testMapDecoding();
	testTpsRateOfChange();

	setThermistorConfiguration(&tc, 32, 9500, 75, 2100, 120, 1000);

	prepareThermistorCurve(&tc);

	assertEquals(-0.003, tc.s_h_a);
	assertEquals(0.001, tc.s_h_b);
	assertEquals(0.0, tc.s_h_c);

	float t = convertResistanceToKelvinTemperature(2100, &tc);
	assertEquals(75 + KELV, t);
}
