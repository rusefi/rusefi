#include "pch.h"

#include "unit_test_framework.h"
#include "init.h"
#include "functional_sensor.h"
#include "thermistor_func.h"
#include "adc_inputs.h"
#include "board_overrides.h"

namespace {
struct ScopedTemperatureElectricalOverrides {
	std::optional<setup_custom_get_adc_float_type> supply = custom_board_getThermistorSupplyVoltage;
	std::optional<setup_custom_get_adc_float_type> divider = custom_board_getAnalogInputDividerCoefficient;

	ScopedTemperatureElectricalOverrides() {
		custom_board_getThermistorSupplyVoltage = [](adc_channel_e channel) {
			return channel == EFI_ADC_6 ? 3.3f : 5.0f;
		};
		custom_board_getAnalogInputDividerCoefficient = [](adc_channel_e channel) {
			return channel == EFI_ADC_6 ? 1.0f : engineConfiguration->analogInputDividerCoefficient;
		};
	}

	~ScopedTemperatureElectricalOverrides() {
		custom_board_getThermistorSupplyVoltage = supply;
		custom_board_getAnalogInputDividerCoefficient = divider;
	}
};
}

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
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

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
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

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
	engineConfiguration->tps1_1AdcChannel = static_cast<adc_channel_e>(EFI_ADC_ERROR);
	engineConfiguration->tpsMin = 200;	// 1.00 volt
	engineConfiguration->tpsMax = 200;	// 1.00 volt
	EXPECT_NO_FATAL_ERROR(initTps());
	Sensor::resetRegistry();

	// de-init and re-init should also work without error
	EXPECT_NO_FATAL_ERROR(deinitTps());
	EXPECT_NO_FATAL_ERROR(initTps());
}

TEST(SensorInit, Pedal) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

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
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// We have no pedal - so we should get the TPS
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;

	initTps();

	ASSERT_TRUE(Sensor::hasSensor(SensorType::Tps1));
	// Ensure a sensor got set
	ASSERT_TRUE(Sensor::hasSensor(SensorType::DriverThrottleIntent));

	// Set values so we can identify which one got proxied
	Sensor::setMockValue(SensorType::Tps1, 25);
	Sensor::setMockValue(SensorType::AcceleratorPedal, 75);

	// Should get the TPS
	EXPECT_EQ(Sensor::get(SensorType::DriverThrottleIntent).Value, 25);
}


TEST(SensorInit, DriverIntentWithPedal) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

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

TEST(SensorInit, FordTps) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// pedal
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_0;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_1;

	// tps:
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_2;
	engineConfiguration->tps1_2AdcChannel = EFI_ADC_3;

	engineConfiguration->useFordRedundantTps = true;
	engineConfiguration->useFordRedundantPps = true;

	// Should succeed, 0.51 volts apart
	engineConfiguration->tpsMin = 200;	// 1.00 volt
	engineConfiguration->tpsMax = 302;	// 1.51 volts
	EXPECT_NO_FATAL_ERROR(initTps());
	Sensor::resetRegistry();

	// de-init and re-init should also work without error
	EXPECT_NO_FATAL_ERROR(deinitTps());
	EXPECT_NO_FATAL_ERROR(initTps());
}

TEST(SensorInit, OilPressure) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	engineConfiguration->oilPressure.hwChannel = EFI_ADC_0;
	engineConfiguration->oilPressure.v1 = 1;
	engineConfiguration->oilPressure.v2 = 4;
	engineConfiguration->oilPressure.value1 = 0;
	engineConfiguration->oilPressure.value2 = 1000;

	initFluidPressure();

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
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	// 2003 neon sensor
	engineConfiguration->clt.config = {0, 30, 100, 32500, 7550, 700, 2700};
	engineConfiguration->clt.adcChannel = EFI_ADC_6;

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

TEST(SensorInit, AnalogDividerOverrideLifecycle) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	ScopedTemperatureElectricalOverrides overrides;
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	EXPECT_FLOAT_EQ(1.0f, getAnalogInputDividerCoefficient(EFI_ADC_6));
	EXPECT_FLOAT_EQ(2.0f, getAnalogInputDividerCoefficient(EFI_ADC_7));

	// With no board callback, every channel uses the current configuration.
	custom_board_getAnalogInputDividerCoefficient.reset();
	EXPECT_FLOAT_EQ(2.0f, getAnalogInputDividerCoefficient(EFI_ADC_6));
	engineConfiguration->analogInputDividerCoefficient = 3.0f;
	EXPECT_FLOAT_EQ(3.0f, getAnalogInputDividerCoefficient(EFI_ADC_6));
	EXPECT_FLOAT_EQ(3.0f, getAnalogInputDividerCoefficient(EFI_ADC_7));
}

TEST(SensorInit, BufferedThreeVoltThermistor) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	ScopedTemperatureElectricalOverrides overrides;
	engineConfiguration->clt.config = {0, 30, 100, 32500, 7550, 700, 2150};
	engineConfiguration->clt.adcChannel = EFI_ADC_6;
	engineConfiguration->analogInputDividerCoefficient = 2.0f;
	initThermistors();
	auto sensor = static_cast<const FunctionalSensor*>(Sensor::getSensorOfType(SensorType::Clt));
	ASSERT_NE(nullptr, sensor);
	auto chain = static_cast<thermistor_t*>(sensor->getFunction());
	EXPECT_FLOAT_EQ(1.0f, getAnalogInputDividerCoefficient(EFI_ADC_6));
	auto resistance = chain->get<resist>().convert(1.65f);
	ASSERT_TRUE(resistance.Valid);
	EXPECT_NEAR(2150.0f, resistance.Value, 0.01f);
	EXPECT_EQ(UnexpectedCode::High, chain->get<resist>().convert(3.3f).Code);
	EXPECT_EQ(UnexpectedCode::Low, chain->get<resist>().convert(0.0f).Code);
	EXPECT_FLOAT_EQ(0.0f, chain->get<resist>().getLastResistance());
	// Verify the complete initialized voltage -> resistance -> temperature chain.
	for (auto point : {std::pair{32500.0f, 0.0f}, {7550.0f, 30.0f}, {700.0f, 100.0f}}) {
		const float voltage = 3.3f * point.first / (2150.0f + point.first);
		auto result = chain->convert(voltage * getAnalogInputDividerCoefficient(EFI_ADC_6));
		ASSERT_TRUE(result.Valid);
		EXPECT_NEAR(point.second, result.Value, 0.01f);
	}
}

TEST(SensorInit, ThermistorSupplyIsPerChannel) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	ScopedTemperatureElectricalOverrides overrides;
	engineConfiguration->iat.config = {0, 30, 100, 32500, 7550, 700, 2150};
	engineConfiguration->iat.adcChannel = EFI_ADC_7;
	engineConfiguration->auxTempSensor1 = engineConfiguration->iat;
	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_6;
	engineConfiguration->analogInputDividerCoefficient = 2.0f;
	initThermistors();
	EXPECT_FLOAT_EQ(2.0f, getAnalogInputDividerCoefficient(EFI_ADC_7));
	for (auto type : {SensorType::Iat, SensorType::AuxTemp1}) {
		auto sensor = const_cast<Sensor*>(Sensor::getSensorOfType(type));
		ASSERT_NE(nullptr, sensor);
		const float supply = type == SensorType::Iat ? 5.0f : 3.3f;
		EXPECT_POINT_VALID(sensor, supply * 7550.0f / (2150.0f + 7550.0f), 30.0f);
		EXPECT_POINT_INVALID(sensor, supply);
	}
}

TEST(SensorInit, ThreeVoltPulldownThermistor) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	ScopedTemperatureElectricalOverrides overrides;
	engineConfiguration->clt.config = {0, 30, 100, 32500, 7550, 700, 2150};
	engineConfiguration->clt.adcChannel = EFI_ADC_6;
	engineConfiguration->cltSensorPulldown = true;
	initThermistors();
	auto sensor = const_cast<Sensor*>(Sensor::getSensorOfType(SensorType::Clt));
	ASSERT_NE(nullptr, sensor);
	EXPECT_POINT_VALID(sensor, 3.3f * 2150.0f / (2150.0f + 7550.0f), 30.0f);
}

TEST(SensorInit, LinearTemperatureIgnoresThermistorSupply) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	ScopedTemperatureElectricalOverrides overrides;
	engineConfiguration->clt.config = {0, 100, 200, 0.5f, 2.5f, 4.5f, 2150};
	engineConfiguration->clt.adcChannel = EFI_ADC_6;
	engineConfiguration->useLinearCltSensor = true;
	initThermistors();
	auto sensor = const_cast<Sensor*>(Sensor::getSensorOfType(SensorType::Clt));
	ASSERT_NE(nullptr, sensor);
	EXPECT_POINT_VALID(sensor, 1.5f, 50.0f);
}

TEST(SensorInit, Lambda) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);

	initLambda();

	auto s = Sensor::getSensorOfType(SensorType::Lambda1);
	ASSERT_NE(nullptr, s);
}

TEST(SensorInit, Map) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_4;

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

// Reproduction: a custom barometer currently inherits manifold MAP calibration.
TEST(SensorInit, CustomBarometerCalibration) {
	EngineTestHelper eth(engine_type_e::TEST_ENGINE);
	engineConfiguration->mapErrorDetectionTooLow = 0;
	engineConfiguration->mapErrorDetectionTooHigh = 500;
	engineConfiguration->baroSensor.hwChannel = EFI_ADC_5;
	engineConfiguration->baroSensor.type = MT_CUSTOM;
	engineConfiguration->baroSensor.lowValue = 0;
	engineConfiguration->baroSensor.highValue = 1.0f / 0.007895f;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_3;
	engineConfiguration->map.sensor.type = MT_CUSTOM;
	engineConfiguration->map.sensor.lowValue = 10;
	engineConfiguration->map.sensor.highValue = 350;
	engineConfiguration->mapLowValueVoltage = 0;
	engineConfiguration->mapHighValueVoltage = 5;
	initMap();
	auto baro = const_cast<Sensor*>(Sensor::getSensorOfType(SensorType::BarometricPressure));
	auto map = const_cast<Sensor*>(Sensor::getSensorOfType(SensorType::MapSlow));
	ASSERT_NE(nullptr, baro);
	ASSERT_NE(nullptr, map);
	EXPECT_POINT_VALID(baro, 3.9475f, 278.43f);
	EXPECT_POINT_VALID(map, 5, 350);

	engineConfiguration->map.sensor.highValue = 450;
	engineConfiguration->mapLowValueVoltage = 0.5f;
	engineConfiguration->mapHighValueVoltage = 4.5f;
	initMap();
	EXPECT_POINT_VALID(baro, 3.9475f, 389.225f);
	EXPECT_POINT_VALID(map, 4.5f, 450);

	engineConfiguration->baroSensor.type = MT_GM_1_BAR;
	initMap();
	EXPECT_POINT_VALID(baro, 5, 105);
}
