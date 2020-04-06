#include "unit_test_framework.h"
#include "init.h"
#include "sensor.h"
#include "functional_sensor.h"

#include "engine_test_helper.h"
#include <gtest/gtest.h>

static void postToFuncSensor(Sensor* s, float value) {
	static_cast<FunctionalSensor*>(s)->postRawValue(value, getTimeNowNt());
}

#define EXPECT_POINT_VALID(s, raw, expect) \
	{\
		postToFuncSensor(s, raw); \
		auto res = s->get(); \
		EXPECT_TRUE(res.Valid); \
		EXPECT_NEAR(res.Value, expect, EPS4D); \
	}

#define EXPECT_POINT_INVALID(s, raw) \
	{\
		postToFuncSensor(s, raw); \
		auto res = s->get(); \
		EXPECT_FALSE(res.Valid); \
	}

TEST(SensorInit, Tps) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	CONFIG(tpsMin) = 200;	// 1 volt
	CONFIG(tpsMax) = 800;	// 4 volts

	initTps(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Ensure the sensors were registered
	auto s = const_cast<Sensor*>(Sensor::getSensorOfType(SensorType::Tps1));
	ASSERT_NE(nullptr, s);

	// Test in range
	EXPECT_POINT_VALID(s, 1.0f, 0.0f);	// closed throttle
	EXPECT_POINT_VALID(s, 2.5f, 50.0f);	// half throttle
	EXPECT_POINT_VALID(s, 4.0f, 100.0f) // full throttle

	// Test out of range
	EXPECT_POINT_INVALID(s, 0.0f);
	EXPECT_POINT_INVALID(s, 5.0f);
}

TEST(SensorInit, Pedal) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	CONFIG(throttlePedalPositionAdcChannel) = EFI_ADC_0;
	CONFIG(throttlePedalUpVoltage) = 1;
	CONFIG(throttlePedalWOTVoltage) = 4;

	initTps(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Ensure the sensors were registered
	auto s = const_cast<Sensor*>(Sensor::getSensorOfType(SensorType::AcceleratorPedal));
	ASSERT_NE(nullptr, s);

	// Test in range
	EXPECT_POINT_VALID(s, 1.0f, 0.0f);	// closed throttle
	EXPECT_POINT_VALID(s, 2.5f, 50.0f);	// half throttle
	EXPECT_POINT_VALID(s, 4.0f, 100.0f) // full throttle

	// Test out of range
	EXPECT_POINT_INVALID(s, 0.0f);
	EXPECT_POINT_INVALID(s, 5.0f);
}

TEST(SensorInit, DriverIntentNoPedal) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// We have no pedal - so we should get the TPS
	CONFIG(throttlePedalPositionAdcChannel) = EFI_ADC_NONE;

	initTps(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Ensure a sensor got set
	ASSERT_TRUE(Sensor::hasSensor(SensorType::DriverThrottleIntent));

	// Set values so we can identify which one got proxied
	Sensor::setMockValue(SensorType::Tps1, 25);
	Sensor::setMockValue(SensorType::AcceleratorPedal, 75);

	// Should get the TPS
	EXPECT_EQ(Sensor::get(SensorType::DriverThrottleIntent).Value, 25);
}


TEST(SensorInit, DriverIntentWithPedal) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// We have a pedal, so we should get it
	CONFIG(throttlePedalPositionAdcChannel) = EFI_ADC_0;

	initTps(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Ensure a sensor got set
	ASSERT_TRUE(Sensor::hasSensor(SensorType::DriverThrottleIntent));

	// Set values so we can identify which one got proxied
	Sensor::setMockValue(SensorType::Tps1, 25);
	Sensor::setMockValue(SensorType::AcceleratorPedal, 75);

	// Should get the pedal
	EXPECT_EQ(Sensor::get(SensorType::DriverThrottleIntent).Value, 75);
}

TEST(SensorInit, OilPressure) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	CONFIG(oilPressure.hwChannel) = EFI_ADC_0;
	CONFIG(oilPressure.v1) = 1;
	CONFIG(oilPressure.v2) = 4;
	CONFIG(oilPressure.value1) = 0;
	CONFIG(oilPressure.value2) = 1000;

	initOilPressure(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Ensure the sensors were registered
	auto s = const_cast<Sensor*>(Sensor::getSensorOfType(SensorType::OilPressure));
	ASSERT_NE(nullptr, s);

	// Test in range
	EXPECT_POINT_VALID(s, 1.0f, 0.0f);	// minimum
	EXPECT_POINT_VALID(s, 2.5f, 500.0f);	// mid
	EXPECT_POINT_VALID(s, 4.0f, 1000.0f) // maximium

	// Test out of range
	EXPECT_POINT_INVALID(s, 0.0f);
	EXPECT_POINT_INVALID(s, 5.0f);
}

TEST(SensorInit, Clt) {
	WITH_ENGINE_TEST_HELPER(TEST_ENGINE);

	// 2003 neon sensor
	CONFIG(clt.config) = {0, 30, 100, 32500, 7550, 700, 2700};

	initNewThermistors(PASS_ENGINE_PARAMETER_SIGNATURE);

	// Ensure the sensors were registered
	auto s = const_cast<Sensor*>(Sensor::getSensorOfType(SensorType::Clt));
	ASSERT_NE(nullptr, s);

	// Test in range

	// TODO: fixme
	EXPECT_POINT_VALID(s, 1.0f, 0.0f);	// minimum
	EXPECT_POINT_VALID(s, 2.5f, 500.0f);	// mid
	EXPECT_POINT_VALID(s, 4.0f, 1000.0f) // maximium

	// Test out of range
	EXPECT_POINT_INVALID(s, 0.0f);
	EXPECT_POINT_INVALID(s, 5.0f);
}
