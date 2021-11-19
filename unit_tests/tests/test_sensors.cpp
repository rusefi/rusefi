/**
 * @file	test_sensors.cpp
 *
 * @date Dec 7, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

TEST(sensors, vrThreshold) {
	EngineTestHelper eth(HELLEN_128_MERCEDES_4_CYL);

	auto& cfg = engineConfiguration->vrThreshold[0];
	ASSERT_FLOAT_EQ(0.8 * PACK_PERCENT_BYTE_MULT, cfg.values[2]);
}

TEST(sensors, mapDecoding) {
	EngineTestHelper eth(FORD_INLINE_6_1995);

	air_pressure_sensor_config_s s;
	s.type = MT_DENSO183;

	assertEqualsM("denso 0 volts", -6.64, decodePressure(0, &s));
	ASSERT_FLOAT_EQ(31.244, decodePressure(1, &s));

	s.type = MT_MPX4250;
	ASSERT_EQ( 8,  decodePressure(0, &s)) << "MPX_4250 0 volts";
	ASSERT_FLOAT_EQ(58.4, decodePressure(1, &s));
}
