#include "pch.h"

#include "adc_subscription.h"
#include "linear_func.h"
#include "fallback_sensor.h"
#include "functional_sensor.h"
#include "map_averaging.h"

static LinearFunc baroConverter;
static FunctionalSensor baroSensor(SensorType::BarometricPressure, MS2NT(50));

// This converter is shared between both fast and slow: the only difference is
// how the *voltage* is determined, not how its converted to a pressure.
static LinearFunc mapConverter;
static FunctionalSensor slowMapSensor(SensorType::MapSlow, MS2NT(50));
// unfinished/dead? static FunctionalSensor slowMapSensor2(SensorType::MapSlow2, MS2NT(50));
static FunctionalSensor compressorDischargePress(SensorType::CompressorDischargePressure, MS2NT(50));
static FunctionalSensor throttleInletPress(SensorType::ThrottleInletPressure, MS2NT(50));

// lowest reasonable idle is maybe 600 rpm
// one sample per cycle (1 cylinder, or "sample one cyl" mode) gives a period of 100ms
// add some margin -> 200ms timeout for fast MAP sampling
// 'fast' means averaged? why is that fast again?!
static MapAverager fastMapSensor(SensorType::MapFast, MS2NT(200));
static MapAverager fastMapSensor2(SensorType::MapFast2, MS2NT(200));

// instant map values are injected here
MapAverager& getMapAvg(size_t cylinderBankIndex) {
  // May 2025: cylinderBankIndex is always zero, second MAP sensor feature is not finished
	return cylinderBankIndex == 0 ? fastMapSensor : fastMapSensor2;
}

// Combine MAP sensors: prefer fast sensor, but use slow if fast is unavailable.
static FallbackSensor mapCombiner(SensorType::Map, SensorType::MapFast, SensorType::MapSlow);
static FallbackSensor mapCombiner2(SensorType::Map2, SensorType::MapFast2, SensorType::MapSlow2);

// helper struct for the local getMapCfg function
struct MapCfg {
	float v1;
	float map1;
	float v2;
	float map2;
};

static MapCfg getMapCfg(air_pressure_sensor_type_e sensorType) {
	switch (sensorType) {
	case MT_DENSO183:
		return {0, -6.64, 5, 182.78};
	case MT_MPX4100:
		return {0.306, 20, 4.897, 105};
	case MT_MPX4250:
	case MT_MPX4250A:
		return {0.204, 20, 4.896, 250};
	case MT_HONDA3BAR:
		return {0.5, 91.422, 3.0, 0};
	case MT_DODGE_NEON_2003:
		return {0.4, 15.34, 4.5, 100};
	case MT_SUBY_DENSO:
		return {0, 0, 5, 200};
	case MT_GM_3_BAR:
		return {0.631, 40, 4.914, 304};
	case MT_GM_2_BAR:
		return {0, 8.8, 5, 208};
	case MT_GM_1_BAR:
		return {0/*v*/, 10/*kpa*/, 5/*v*/, 105/*kpa*/};
	case MT_TOYOTA_89420_02010:
		return {3.7 - 2, 33.32, 3.7, 100};
	case MT_MAZDA_1_BAR:
		return {0, 2.5, 5, 117};
	case MT_BOSCH_2_5:
	// kpa=54.11764705882353v−1.6470588235294201
		return {0.4 , 20 , 4.65, 250};
	case MT_MPXH6400:
		return {0.2, 20, 4.8, 400};
	case MT_MPXH6300:
		return {1.0, 60, 4.5, 270};
	default:
		firmwareError(ObdCode::CUSTOM_ERR_MAP_TYPE, "Unknown MAP type: decoder %d", sensorType);
		// falls through to custom
		return {};
	case MT_CUSTOM: {
		auto& mapConfig = engineConfiguration->map.sensor;
		return {
			engineConfiguration->mapLowValueVoltage,
			mapConfig.lowValue,
			engineConfiguration->mapHighValueVoltage,
			mapConfig.highValue
		};
	}}
}

void configureMapFunction(LinearFunc& converter, air_pressure_sensor_type_e sensorType) {
	auto cfg = getMapCfg(sensorType);

	converter.configure(
		cfg.v1,
		cfg.map1,
		cfg.v2,
		cfg.map2,
		engineConfiguration->mapErrorDetectionTooLow,
		engineConfiguration->mapErrorDetectionTooHigh
	);
}

void initMap() {
	// Set up the conversion function
	configureMapFunction(mapConverter, engineConfiguration->map.sensor.type);

	slowMapSensor.setFunction(mapConverter);
// unfinished/dead?	slowMapSensor2.setFunction(mapConverter);
	fastMapSensor.setFunction(mapConverter);
	fastMapSensor2.setFunction(mapConverter);
	compressorDischargePress.setFunction(mapConverter);
	throttleInletPress.setFunction(mapConverter);

	auto mapChannel = engineConfiguration->map.sensor.hwChannel;
	if (isAdcChannelValid(mapChannel)) {
		slowMapSensor.Register();
// unfinished/dead?		slowMapSensor2.Register();
		fastMapSensor.Register();
		fastMapSensor2.Register();
		mapCombiner.Register();
		mapCombiner2.Register();

		// Configure slow MAP as a normal analog sensor
		// it's possible/probably that slow and fast both read from same physical pin, apparently that's fine?!
		AdcSubscription::SubscribeSensor(slowMapSensor, mapChannel, 100);
	}

	AdcSubscription::SubscribeSensor(throttleInletPress, engineConfiguration->throttleInletPressureChannel, 100);
	AdcSubscription::SubscribeSensor(compressorDischargePress, engineConfiguration->compressorDischargePressureChannel, 100);

	auto baroChannel = engineConfiguration->baroSensor.hwChannel;
	if (isAdcChannelValid(baroChannel)) {
		configureMapFunction(baroConverter, engineConfiguration->baroSensor.type);

		baroSensor.setFunction(baroConverter);
		baroSensor.Register();

		AdcSubscription::SubscribeSensor(baroSensor, baroChannel, 10);
	}
}

void deinitMap() {
	AdcSubscription::UnsubscribeSensor(slowMapSensor, engineConfiguration->map.sensor.hwChannel);
	AdcSubscription::UnsubscribeSensor(baroSensor, engineConfiguration->baroSensor.hwChannel);
	AdcSubscription::UnsubscribeSensor(throttleInletPress, engineConfiguration->throttleInletPressureChannel);
	AdcSubscription::UnsubscribeSensor(compressorDischargePress, engineConfiguration->compressorDischargePressureChannel);
}
