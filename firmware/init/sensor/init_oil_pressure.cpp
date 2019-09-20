#include "engine.h"
#include "error_handling.h"
#include "global.h"
#include "linear_sensor.h"
#include "tunerstudio_configuration.h"
#include "adc_subscription.h"

EXTERN_ENGINE;

extern TunerStudioOutputChannels tsOutputChannels;

LinearSensor oilpSensor(SensorType::OilPressure);

void initOilPressure() {
	// Only register if we have a sensor
	auto channel = engineConfiguration->oilPressure.hwChannel;
	if (channel == EFI_ADC_NONE) {
		return;
	}

	oil_pressure_config_s *sensorCfg = &CONFIG(oilPressure);

	float val1 = sensorCfg->value1;
	float val2 = sensorCfg->value2;

	// Limit to max given pressure - val1 or val2 could be larger
	// (sensor may be backwards, high voltage = low pressure)
	float greaterOutput = val1 > val2 ? val1 : val2;

	// Allow slightly negative output (-5kpa) so as to not fail the sensor when engine is off
	oilpSensor.configure(sensorCfg->v1, val1, sensorCfg->v2, val2, /*minOutput*/ -5, greaterOutput);

	// Tell it to report to its output channel
	oilpSensor.setReportingLocation(&tsOutputChannels.oilPressure);

	// Subscribe the sensor to the ADC
	AdcSubscription::SubscribeSensor(oilpSensor, channel);

	if (!oilpSensor.Register()) {
		warning(OBD_Oil_Pressure_Sensor_Malfunction, "Duplicate oilp sensor registration, ignoring");
	}
}
