/**
 * @file	test_sensors.cpp
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "global.h"
#include "thermistors.h"
#include "allsensors.h"
#include "engine_test_helper.h"

static ThermistorConf tc;

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

TEST(sensors, tps) {
	print("************************************************** testTps\r\n");

	WITH_ENGINE_TEST_HELPER(DODGE_RAM);

	engineConfiguration->tpsMax = 193;
	engineConfiguration->tpsMin = 43;

	ASSERT_NEAR(49.3333, getTpsValue(4 * 117 PASS_ENGINE_PARAMETER_SUFFIX), EPS4D);


	engineConfiguration->tpsMax = 43;
	engineConfiguration->tpsMin = 193;
	assertEqualsM("test#2", 50.6667, getTpsValue(4 * 117 PASS_ENGINE_PARAMETER_SUFFIX));
}

TEST(sensors, testTpsRateOfChange) {
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

TEST(sensors, thermistors) {

	ThermistorMath tm;
	{
		setThermistorConfiguration(&tc, 32, 9500, 75, 2100, 120, 1000);
		tm.setConfig(&tc.config);
		float t = tm.getKelvinTemperatureByResistance(2100);
		ASSERT_FLOAT_EQ(75 + KELV, t);

		ASSERT_NEAR(-0.003, tm.s_h_a, EPS4D);
		ASSERT_NEAR(0.001, tm.s_h_b, EPS4D);
		ASSERT_NEAR(0.0, tm.s_h_c, EPS5D);

	}

	{
		// 2003 Neon sensor
		setThermistorConfiguration(&tc, 0, 32500, 30, 7550, 100, 700);
		tm.setConfig(&tc.config);

		float t = tm.getKelvinTemperatureByResistance(38000);
		ASSERT_NEAR(-2.7983, t - KELV, EPS4D);

		assertEqualsM("A", 0.0009, tm.s_h_a);
		assertEqualsM("B", 0.0003, tm.s_h_b);
		ASSERT_NEAR(0.0, tm.s_h_c, EPS4D);
	}
}
