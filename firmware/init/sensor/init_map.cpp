#include "map.h"
#include "function_pointer_sensor.h"
#include "engine.h"

EXTERN_ENGINE;

struct GetMapWrapper {
	DECLARE_ENGINE_PTR;

	float getMap() {
		return ::getMap(PASS_ENGINE_PARAMETER_SIGNATURE);
	}
};

static GetMapWrapper mapWrapper;

static FunctionPointerSensor mapSensor(SensorType::Map,
[]() {
	return mapWrapper.getMap();
});

void initMap(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	INJECT_ENGINE_REFERENCE(&mapWrapper);

	mapSensor.Register();
}
