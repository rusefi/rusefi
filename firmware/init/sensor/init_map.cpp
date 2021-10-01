#include "pch.h"

#include "adc_subscription.h"
#include "linear_func.h"
#include "fallback_sensor.h"
#include "functional_sensor.h"
#include "function_pointer_sensor.h"

struct GetBaroWrapper {
	DECLARE_ENGINE_PTR;

	float getBaro() {
		return ::getBaroPressure(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
};

static GetBaroWrapper baroWrapper;

static FunctionPointerSensor baroSensor(SensorType::BarometricPressure,
[]() {
	return baroWrapper.getBaro();
});

// This converter is shared between both fast and slow: the only difference is
// how the *voltage* is determined, not how its converted to a pressure.
static LinearFunc mapConverter;
static FunctionalSensor slowMapSensor(SensorType::MapSlow, MS2NT(50));

// lowest reasonable idle is maybe 600 rpm
// one sample per cycle (1 cylinder, or "sample one cyl" mode) gives a period of 100ms
// add some margin -> 200ms timeout for fast MAP sampling
static FunctionalSensor fastMapSensor(SensorType::MapFast, MS2NT(200));

// This is called from the fast ADC completion callback
void onMapAveraged(float volts, efitick_t nowNt) {
	fastMapSensor.postRawValue(volts, nowNt);
}

// Combine MAP sensors: prefer fast sensor, but use slow if fast is unavailable.
static FallbackSensor mapCombiner(SensorType::Map, SensorType::MapFast, SensorType::MapSlow);

// helper struct for the local getMapCfg function
struct MapCfg {
	float v1, v2;
	float map1, map2;
};

static MapCfg getMapCfg(air_pressure_sensor_type_e sensorType DECLARE_CONFIG_PARAMETER_SUFFIX) {
	switch (sensorType) {
	case MT_DENSO183:
		return {0, -6.64, 5, 182.78};
	case MT_MPX4250:
		return {0, 8, 5, 260};
	case MT_MPX4100:
		return {0.3, 20, 4.9, 105};
	case MT_MPX4250A:
		return {0.25, 20, 4.875, 250};
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
		return {0, 10, 5, 105};
	case MT_TOYOTA_89420_02010:
		return {3.7 - 2, 33.32, 3.7, 100};
	case MT_MAZDA_1_BAR:
		return {0, 2.5, 5, 117};
	case MT_BOSCH_2_5:
		return {0.4 , 20 , 4.65, 250};
	case MT_MPXH6400:
		return {1, 90, 3, 250};
	default:
		firmwareError(CUSTOM_ERR_MAP_TYPE, "Unknown MAP type: decoder %d", sensorType);
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

void configureMapFunction(air_pressure_sensor_type_e sensorType DECLARE_CONFIG_PARAMETER_SUFFIX) {
	auto cfg = getMapCfg(sensorType PASS_CONFIG_PARAMETER_SUFFIX);

	mapConverter.configure(
		cfg.v1,
		cfg.map1,
		cfg.v2,
		cfg.map2,
		engineConfiguration->mapErrorDetectionTooLow,
		engineConfiguration->mapErrorDetectionTooHigh
	);
}

void initMap(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	INJECT_ENGINE_REFERENCE(&baroWrapper);

	auto mapChannel = engineConfiguration->map.sensor.hwChannel;

	if (isAdcChannelValid(mapChannel)) {
		// Set up the conversion function
		configureMapFunction(engineConfiguration->map.sensor.type PASS_CONFIG_PARAMETER_SUFFIX);

		// Both sensors use the same converter function
		slowMapSensor.setFunction(mapConverter);
		fastMapSensor.setFunction(mapConverter);

		slowMapSensor.Register();
		fastMapSensor.Register();
		mapCombiner.Register();

		// Configure slow MAP as a normal analog sensor
		AdcSubscription::SubscribeSensor(slowMapSensor, mapChannel, 100);
	}

	// Only register if configured
	if (isAdcChannelValid(engineConfiguration->baroSensor.hwChannel)) {
		baroSensor.Register();
	}
}

void deinitMap() {
	AdcSubscription::UnsubscribeSensor(slowMapSensor);
}
