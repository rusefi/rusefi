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

	// Limit to max given pressure
	oilpSensor.configure(sensorCfg->v1, sensorCfg->value1, sensorCfg->v2, sensorCfg->value2, 0, sensorCfg->value2);

	// Subscribe the sensor to ADC

	if (!oilpSensor.Register()) {
		warning(OBD_Oil_Pressure_Sensor_Malfunction, "Duplicate oilp sensor registration, ignoring");
	}
}
