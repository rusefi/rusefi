#include "pch.h"

#include "unit_test_framework.h"
#include "init.h"
#include "functional_sensor.h"

static void postToFuncSensor(Sensor* s, float value) {
	static_cast<FunctionalSensor*>(s)->postRawValue(value, getTimeNowNt());
}

#define EXPECT_POINT_VALID(s, raw, expect) \
	{\
		postToFuncSensor(s, raw); \
		auto res = s->get(); \
		EXPECT_TRUE(res.Valid); \
		EXPECT_NEAR(res.Value, expect, EPS2D); \
	}

#define EXPECT_POINT_INVALID(s, raw) \
	{\
		postToFuncSensor(s, raw); \
		auto res = s->get(); \
		EXPECT_FALSE(res.Valid); \
	}

TEST(SensorInit, Tps) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->tpsMin = 200;	// 1 volt
	engineConfiguration->tpsMax = 800;	// 4 volts

	initTps();

	// Ensure the sensors were registered
	auto s = const_cast<Sensor*>(Sensor::getSensorOfType(SensorType::Tps1Primary));
	ASSERT_NE(nullptr, s);

	// Test in range
	EXPECT_POINT_VALID(s, 1.0f, 0.0f);	// closed throttle
	EXPECT_POINT_VALID(s, 2.5f, 50.0f);	// half throttle
	EXPECT_POINT_VALID(s, 4.0f, 100.0f) // full throttle

	// Test out of range
	EXPECT_POINT_INVALID(s, 0.0f);
	EXPECT_POINT_INVALID(s, 5.0f);

	// Test that the passthru (redundant sensor) is working
	EXPECT_POINT_VALID(s, 2.5f, 50.0f);
	EXPECT_NEAR(50.0f, Sensor::get(SensorType::Tps1).value_or(-1), EPS2D);
}

TEST(SensorInit, TpsValuesTooClose) {
	EngineTestHelper eth(TEST_ENGINE);

	// Should fail, 0.49 volts apart
	engineConfiguration->tpsMin = 200;	// 1.00 volt
	engineConfiguration->tpsMax = 298;	// 1.49 volts
	EXPECT_FATAL_ERROR(initTps());
	Sensor::resetRegistry();

	// Should fail, -0.49 volts apart
	engineConfiguration->tpsMin = 298;	// 1.49 volt
	engineConfiguration->tpsMax = 200;	// 1.00 volts
	EXPECT_FATAL_ERROR(initTps());
	Sensor::resetRegistry();

	// Should succeed, 0.51 volts apart
	engineConfiguration->tpsMin = 200;	// 1.00 volt
	engineConfiguration->tpsMax = 302;	// 1.51 volts
	EXPECT_NO_FATAL_ERROR(initTps());
	Sensor::resetRegistry();

	// Should succeed, -0.51 volts apart
	engineConfiguration->tpsMin = 302;	// 1.51 volt
	engineConfiguration->tpsMax = 200;	// 1.00 volts
	EXPECT_NO_FATAL_ERROR(initTps());
	Sensor::resetRegistry();

	// With no pin, it should be ok that they are the same
	// Should succeed, -0.51 volts apart
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_NONE;
	engineConfiguration->tpsMin = 200;	// 1.00 volt
	engineConfiguration->tpsMax = 200;	// 1.00 volts
	EXPECT_NO_FATAL_ERROR(initTps());
	Sensor::resetRegistry();

	// Test a random bogus pin index, shouldn't fail
	engineConfiguration->tps1_1AdcChannel = static_cast<adc_channel_e>(175);
	engineConfiguration->tpsMin = 200;	// 1.00 volt
	engineConfiguration->tpsMax = 200;	// 1.00 volt
	EXPECT_NO_FATAL_ERROR(initTps());
	Sensor::resetRegistry();

	// de-init and re-init should also work without error
	EXPECT_NO_FATAL_ERROR(deinitTps());
	EXPECT_NO_FATAL_ERROR(initTps());
}

TEST(SensorInit, Pedal) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_0;
	engineConfiguration->throttlePedalUpVoltage = 1;
	engineConfiguration->throttlePedalWOTVoltage = 4;

	initTps();

	// Ensure the sensors were registered
	auto s = const_cast<Sensor*>(Sensor::getSensorOfType(SensorType::AcceleratorPedalPrimary));
	ASSERT_NE(nullptr, s);

	// Test in range
	EXPECT_POINT_VALID(s, 1.0f, 0.0f);	// closed throttle
	EXPECT_POINT_VALID(s, 2.5f, 50.0f);	// half throttle
	EXPECT_POINT_VALID(s, 4.0f, 100.0f) // full throttle

	// Test out of range
	EXPECT_POINT_INVALID(s, 0.0f);
	EXPECT_POINT_INVALID(s, 5.0f);

	// Test that the passthru (redundant sensor) is working
	EXPECT_POINT_VALID(s, 2.5f, 50.0f);
	EXPECT_NEAR(50.0f, Sensor::get(SensorType::AcceleratorPedal).value_or(-1), EPS2D);
}

TEST(SensorInit, DriverIntentNoPedal) {
	EngineTestHelper eth(TEST_ENGINE);

	// We have no pedal - so we should get the TPS
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;

	initTps();

	// Ensure a sensor got set
	ASSERT_TRUE(Sensor::hasSensor(SensorType::DriverThrottleIntent));

	// Set values so we can identify which one got proxied
	Sensor::setMockValue(SensorType::Tps1, 25);
	Sensor::setMockValue(SensorType::AcceleratorPedal, 75);

	// Should get the TPS
	EXPECT_EQ(Sensor::get(SensorType::DriverThrottleIntent).Value, 25);
}


TEST(SensorInit, DriverIntentWithPedal) {
	EngineTestHelper eth(TEST_ENGINE);

	// We have a pedal, so we should get it
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_0;

	initTps();

	// Ensure a sensor got set
	ASSERT_TRUE(Sensor::hasSensor(SensorType::DriverThrottleIntent));

	// Set values so we can identify which one got proxied
	Sensor::setMockValue(SensorType::Tps1, 25);
	Sensor::setMockValue(SensorType::AcceleratorPedal, 75);

	// Should get the pedal
	EXPECT_EQ(Sensor::get(SensorType::DriverThrottleIntent).Value, 75);
}

TEST(SensorInit, OilPressure) {
	EngineTestHelper eth(TEST_ENGINE);

	engineConfiguration->oilPressure.hwChannel = EFI_ADC_0;
	engineConfiguration->oilPressure.v1 = 1;
	engineConfiguration->oilPressure.v2 = 4;
	engineConfiguration->oilPressure.value1 = 0;
	engineConfiguration->oilPressure.value2 = 1000;

	initOilPressure();

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
	EngineTestHelper eth(TEST_ENGINE);

	// 2003 neon sensor
	engineConfiguration->clt.config = {0, 30, 100, 32500, 7550, 700, 2700};

	initThermistors();

	// Ensure the sensors were registered
	auto s = const_cast<Sensor*>(Sensor::getSensorOfType(SensorType::Clt));
	ASSERT_NE(nullptr, s);

	// Test in range
	EXPECT_POINT_VALID(s, 4.61648f, 0.0f);	// minimum - 0C
	EXPECT_POINT_VALID(s, 3.6829f, 30.0f);	// mid - 30C
	EXPECT_POINT_VALID(s, 1.0294f, 100.0f)	// maximium - 100C

	// Test out of range
	EXPECT_POINT_INVALID(s, 0.0f);
	EXPECT_POINT_INVALID(s, 5.0f);
}

TEST(SensorInit, Lambda) {
	EngineTestHelper eth(TEST_ENGINE);

	initLambda();

	auto s = Sensor::getSensorOfType(SensorType::Lambda1);
	ASSERT_NE(nullptr, s);
}

TEST(SensorInit, Map) {
	EngineTestHelper eth(TEST_ENGINE);

	initMap();

	auto s = Sensor::getSensorOfType(SensorType::Map);
	ASSERT_NE(nullptr, s);

	Sensor::setMockValue(SensorType::MapFast, 25);
	Sensor::setMockValue(SensorType::MapSlow, 75);

	// Should prefer fast MAP
	EXPECT_FLOAT_EQ(25, Sensor::getOrZero(SensorType::Map));

	// But when that fails, should return slow MAP
	Sensor::resetMockValue(SensorType::MapFast);
	EXPECT_FLOAT_EQ(75, Sensor::getOrZero(SensorType::Map));
}
