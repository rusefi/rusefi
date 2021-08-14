/**
 * @file	test_sensors.cpp
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

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
