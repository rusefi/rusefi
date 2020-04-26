/**
 * @file	test_sensors.cpp
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "global.h"
#include "thermistors.h"
#include "allsensors.h"
#include "engine_test_helper.h"

TEST(sensors, mapDecoding) {
	WITH_ENGINE_TEST_HELPER(FORD_INLINE_6_1995);

	air_pressure_sensor_config_s s;
	s.type = MT_DENSO183;

	assertEqualsM("denso 0 volts", -6.64, decodePressure(0, &s PASS_ENGINE_PARAMETER_SUFFIX));
	ASSERT_FLOAT_EQ(31.244, decodePressure(1, &s PASS_ENGINE_PARAMETER_SUFFIX));

	s.type = MT_MPX4250;
	ASSERT_EQ( 8,  decodePressure(0, &s PASS_ENGINE_PARAMETER_SUFFIX)) << "MPX_4250 0 volts";
	ASSERT_FLOAT_EQ(58.4, decodePressure(1, &s PASS_ENGINE_PARAMETER_SUFFIX));
}

TEST(sensors, Thermistor1) {

	ThermistorMath tm;
	thermistor_conf_s tc = {32, 75, 120, 9500, 2100, 1000, 0};
	tm.setConfig(&tc);

	float t = tm.getKelvinTemperatureByResistance(2100);
	ASSERT_FLOAT_EQ(75 + KELV, t);

	ASSERT_NEAR(-0.003, tm.s_h_a, EPS4D);
	ASSERT_NEAR(0.001, tm.s_h_b, EPS4D);
	ASSERT_NEAR(0.0, tm.s_h_c, EPS5D);
}

TEST(sensors, ThermistorNeon)
{
	ThermistorMath tm;
	// 2003 Neon sensor
	thermistor_conf_s tc = {0, 30, 100, 32500, 7550, 700, 0};
	tm.setConfig(&tc);

	float t = tm.getKelvinTemperatureByResistance(38000);
	ASSERT_NEAR(-2.7983, t - KELV, EPS4D);

	assertEqualsM("A", 0.0009, tm.s_h_a);
	assertEqualsM("B", 0.0003, tm.s_h_b);
	ASSERT_NEAR(0.0, tm.s_h_c, EPS4D);
}
