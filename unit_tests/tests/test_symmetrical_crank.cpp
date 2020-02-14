/*
 * @file test_symmetrical_crank.cpp
 *
 * @date Feb 14, 2020
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "engine_test_helper.h"

TEST(engine, testSymmetricalCrank) {

	WITH_ENGINE_TEST_HELPER(MAZDA_MIATA_2003);

	float x = 0.02;

	ASSERT_EQ( 0,  GET_RPM()) << "RPM#0";

	eth.fireFall(x * 384);
	eth.fireRise(x * 16);
	eth.fireFall(x * 304);
	eth.fireRise(x * 16);
	ASSERT_EQ( 0,  GET_RPM()) << "RPM#1";

	eth.fireFall(x * 384);
	eth.fireRise(x * 16);
	eth.fireFall(x * 304);
	eth.fireRise(x * 16);
	ASSERT_EQ( 0,  GET_RPM()) << "RPM#2";

	eth.fireFall(x * 384);
	eth.fireRise(x * 16);
	ASSERT_EQ( 549,  GET_RPM()) << "RPM#2.5";
	eth.fireFall(x * 304);
	eth.fireRise(x * 16);
	ASSERT_EQ( 549,  GET_RPM()) << "RPM#3";

	eth.fireFall(x * 384);
	eth.fireRise(x * 16);
	eth.fireFall(x * 304);
	eth.fireRise(x * 16);
	ASSERT_EQ( 549,  GET_RPM()) << "RPM#4";

	eth.fireFall(x * 384);
	eth.fireRise(x * 16);
	eth.fireFall(x * 304);
	eth.fireRise(x * 16);
	ASSERT_EQ( 549,  GET_RPM()) << "RPM#5";

	eth.fireFall(x * 384);
	eth.fireRise(x * 16);
	eth.fireFall(x * 304);
	eth.fireRise(x * 16);
	ASSERT_EQ( 549,  GET_RPM()) << "RPM#6";

	eth.fireFall(x * 384);
	eth.fireRise(x * 16);
	eth.fireFall(x * 304);
	eth.fireRise(x * 16);
	ASSERT_EQ( 374,  GET_RPM()) << "RPM#7";

	eth.fireFall(x * 384);
	eth.fireRise(x * 16);
	eth.fireFall(x * 304);
	eth.fireRise(x * 16);
	ASSERT_EQ( 374,  GET_RPM()) << "RPM#8";

}
