#include "engine.h"
#include "error_handling.h"
#include "global.h"
#include "linear_sensor.h"

EXTERN_ENGINE;

LinearSensor oilpSensor(SensorType::OilPressure);

void initOilPressure() {
	// Only register if we have a sensor
	if (engineConfiguration->oilPressure.hwChannel == EFI_ADC_NONE) {
		return;
	}

	oil_pressure_config_s *sensorCfg = &CONFIG(oilPressure);

	float val1 = sensorCfg->value1;
	float val2 = sensorCfg->value2;

	// Limit to max given pressure - val1 or val2 could be larger
	float greaterOutput = val1 > val2 ? val1 : val2;

	oilpSensor.configure(sensorCfg->v1, val1, sensorCfg->v2, val2, 0, greaterOutput);

	// Subscribe the sensor to ADC

	if (!oilpSensor.Register()) {
		warning(OBD_Oil_Pressure_Sensor_Malfunction, "Duplicate oilp sensor registration, ignoring");
	}
}
