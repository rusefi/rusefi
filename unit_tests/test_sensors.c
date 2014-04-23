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
	assertEqualsM("denso 0 volts", -6.64, getMAPValueHonda_Denso183(0));
	assertEquals(31.244, getMAPValueHonda_Denso183(1));

	assertEqualsM("MPX_4250 0 volts", 8, getMAPValueMPX_4250(0));
	assertEquals(58.4, getMAPValueMPX_4250(1));
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
