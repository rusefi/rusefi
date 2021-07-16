#include "init.h"
#include "adc_inputs.h"
#include "adc_subscription.h"
#include "engine.h"
#include "error_handling.h"
#include "global.h"
#include "functional_sensor.h"
#include "proxy_sensor.h"
#include "linear_func.h"

static LinearFunc oilpSensorFunc;
static FunctionalSensor oilpSensor(SensorType::OilPressure, /* timeout = */ MS2NT(50));

static LinearFunc fuelPressureFuncLow;
static FunctionalSensor fuelPressureSensorLow(SensorType::FuelPressureLow, /* timeout = */ MS2NT(50));

static LinearFunc fuelPressureFuncHigh;
static FunctionalSensor fuelPressureSensorHigh(SensorType::FuelPressureHigh, /* timeout = */ MS2NT(50));

static ProxySensor injectorPressure(SensorType::FuelPressureInjector);

static void configureFluidPressure(LinearFunc& func, const linear_sensor_s& cfg) {
	float val1 = cfg.value1;
	float val2 = cfg.value2;

	// Limit to max given pressure - val1 or val2 could be larger
	// (sensor may be backwards, high voltage = low pressure)
	float greaterOutput = val1 > val2 ? val1 : val2;

	// Allow slightly negative output (-5kpa) so as to not fail the sensor when engine is off
	func.configure(cfg.v1, val1, cfg.v2, val2, /*minOutput*/ -5, greaterOutput);
}

/**
 * @param bandwidth Hertz, used by low pass filter in to analog subscribers
 */
static void initFluidPressure(LinearFunc& func, FunctionalSensor& sensor, const linear_sensor_s& cfg, float bandwidth) {
	auto channel = cfg.hwChannel;

	// Only register if we have a sensor
	if (!isAdcChannelValid(channel)) {
		return;
	}

	configureFluidPressure(func, cfg);
	sensor.setFunction(func);

	AdcSubscription::SubscribeSensor(sensor, channel, bandwidth);

	sensor.Register();
}

void initOilPressure(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	initFluidPressure(oilpSensorFunc, oilpSensor, CONFIG(oilPressure), 10);
	initFluidPressure(fuelPressureFuncLow, fuelPressureSensorLow, CONFIG(lowPressureFuel), 10);
	initFluidPressure(fuelPressureFuncHigh, fuelPressureSensorHigh, CONFIG(highPressureFuel), 100);

	injectorPressure.setProxiedSensor(
		CONFIG(injectorPressureType) == IPT_High
		? SensorType::FuelPressureHigh
		: SensorType::FuelPressureLow
	);

	injectorPressure.Register();
}

void reconfigureOilPressure(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	configureFluidPressure(oilpSensorFunc, CONFIG(oilPressure));
	configureFluidPressure(fuelPressureFuncLow, CONFIG(lowPressureFuel));
	configureFluidPressure(fuelPressureFuncHigh, CONFIG(highPressureFuel));
}
