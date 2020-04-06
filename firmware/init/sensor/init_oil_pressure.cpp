#include "init.h"
#include "adc_subscription.h"
#include "engine.h"
#include "error_handling.h"
#include "global.h"
#include "functional_sensor.h"
#include "linear_func.h"
#if EFI_TUNER_STUDIO
#include "tunerstudio_configuration.h"
#endif

EXTERN_ENGINE;

LinearFunc oilpSensorFunc;
FunctionalSensor oilpSensor(SensorType::OilPressure, /* timeout = */ MS2NT(50));

void configureOilPressure(LinearFunc& func, const oil_pressure_config_s& cfg) {
	float val1 = cfg.value1;
	float val2 = cfg.value2;

	// Limit to max given pressure - val1 or val2 could be larger
	// (sensor may be backwards, high voltage = low pressure)
	float greaterOutput = val1 > val2 ? val1 : val2;

	// Allow slightly negative output (-5kpa) so as to not fail the sensor when engine is off
	func.configure(cfg.v1, val1, cfg.v2, val2, /*minOutput*/ -5, greaterOutput);
}

void initOilPressure(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// Only register if we have a sensor
	auto channel = CONFIG(oilPressure.hwChannel);
	if (channel == EFI_ADC_NONE) {
		return;
	}

	configureOilPressure(oilpSensorFunc, CONFIG(oilPressure));
	oilpSensor.setFunction(oilpSensorFunc);

	// Subscribe the sensor to the ADC
	AdcSubscription::SubscribeSensor(oilpSensor, channel);

	if (!oilpSensor.Register()) {
		warning(OBD_Oil_Pressure_Sensor_Malfunction, "Duplicate oilp sensor registration, ignoring");
	}
}

void reconfigureOilPressure(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	configureOilPressure(oilpSensorFunc, CONFIG(oilPressure));
}
