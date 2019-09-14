
#include "error_handling.h"
#include "function_pointer_sensor.h"
#include "oil_pressure.h"

#include "analog_input.h"
#include "engine.h"
#include "global.h"
#include "interpolation.h"
#include "os_access.h"

EXTERN_ENGINE;

// Wrapper to handle PASS_ENGINE_PARAMETER_SIGNATURE
float getOilPressureLegacyWrapper() {
	return getOilPressureLegacy(PASS_ENGINE_PARAMETER_SIGNATURE);
}

FunctionPointerSensor oilpSensor(SensorType::OilPressure, &getOilPressureLegacyWrapper);

void initOilPressure() {
	// Only register if we have a sensor
	if (engineConfiguration->oilPressure.hwChannel == EFI_ADC_NONE) {
		return;
	}

	if (!oilpSensor.Register()) {
		warning(OBD_Oil_Pressure_Sensor_Malfunction, "Duplicate oilp sensor registration, ignoring");
	}
}
