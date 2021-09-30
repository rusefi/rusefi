#include "pch.h"

#include "fallback_sensor.h"
#include "function_pointer_sensor.h"

struct GetMapWrapper {
	DECLARE_ENGINE_PTR;

	float getMap() {
		return ::getMap(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
};

static GetMapWrapper fastMapWrapper;

struct GetMapWrapper {
	DECLARE_ENGINE_PTR;

	float getMap() {
		return ::getRawMap(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
};

static FunctionPointerSensor mapSensor(SensorType::Map,
[]() {
	return fastMapWrapper.getMap();
});

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




static LinearFunc mapConverter;
static FunctionalSensor slowMapSensor(SensorType::MapSlow, MS2NT(50));




void initMap(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	INJECT_ENGINE_REFERENCE(&mapWrapper);
	INJECT_ENGINE_REFERENCE(&baroWrapper);

	

	mapSensor.Register();

	// Only register if configured
	if (isAdcChannelValid(engineConfiguration->baroSensor.hwChannel)) {
		baroSensor.Register();
	}
}
