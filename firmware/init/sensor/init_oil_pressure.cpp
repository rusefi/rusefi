
#include "function_pointer_sensor.h"
#include "oil_pressure.h"
#include "error_handling.h"

#include "global.h"
#include "os_access.h"
#include "interpolation.h"
#include "analog_input.h"
#include "engine.h"

EXTERN_ENGINE;

// Wrapper to handle PASS_ENGINE_PARAMETER_SIGNATURE
float getOilPressureLegacyWrapper() {
	return getOilPressureLegacy(PASS_ENGINE_PARAMETER_SIGNATURE);
}

FunctionPointerSensor oilpSensor(SensorType::OilPressure, &getOilPressureLegacyWrapper);

void init_oil_pressure() {
	if (!oilpSensor.Register()) {
		warning(OBD_Oil_Pressure_Sensor_Malfunction, "Duplicate oilp sensor registration, ignoring");
	}
}
