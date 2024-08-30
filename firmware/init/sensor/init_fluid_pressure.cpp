#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
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

static LinearFunc auxLinear1Func;
static FunctionalSensor auxLinear1Sensor(SensorType::AuxLinear1, /* timeout = */ MS2NT(50));

static LinearFunc auxLinear2Func;
static FunctionalSensor auxLinear2Sensor(SensorType::AuxLinear2, /* timeout = */ MS2NT(50));

static LinearFunc auxLinear3Func;
static FunctionalSensor auxLinear3Sensor(SensorType::AuxLinear3, /* timeout = */ MS2NT(50));

static LinearFunc auxLinear4Func;
static FunctionalSensor auxLinear4Sensor(SensorType::AuxLinear4, /* timeout = */ MS2NT(50));

/**
 * @param bandwidth Hertz, used by low pass filter in to analog subscribers
 */
static void initFluidPressure(LinearFunc& func, FunctionalSensor& sensor, const linear_sensor_s& cfg, float bandwidth) {
	auto channel = cfg.hwChannel;

	// Only register if we have a sensor
	if (!isAdcChannelValid(channel)) {
		return;
	}

	float val1 = cfg.value1;
	float val2 = cfg.value2;

	// Limit to max given pressure - val1 or val2 could be larger
	// (sensor may be backwards, high voltage = low pressure)
	float greaterOutput = val1 > val2 ? val1 : val2;

	// Allow slightly negative output (-5kpa) so as to not fail the sensor when engine is off
	func.configure(cfg.v1, val1, cfg.v2, val2, /*minOutput*/ -5, greaterOutput);

	sensor.setFunction(func);

	AdcSubscription::SubscribeSensor(sensor, channel, bandwidth);

	sensor.Register();
}

void initFluidPressure() {
	initFluidPressure(oilpSensorFunc, oilpSensor, engineConfiguration->oilPressure, 10);
	initFluidPressure(fuelPressureFuncLow, fuelPressureSensorLow, engineConfiguration->lowPressureFuel, 10);
	initFluidPressure(fuelPressureFuncHigh, fuelPressureSensorHigh, engineConfiguration->highPressureFuel, 100);
	initFluidPressure(auxLinear1Func, auxLinear1Sensor, engineConfiguration->auxLinear1, 10);
	initFluidPressure(auxLinear2Func, auxLinear2Sensor, engineConfiguration->auxLinear2, 10);
	initFluidPressure(auxLinear3Func, auxLinear3Sensor, engineConfiguration->auxLinear3, 10);
	initFluidPressure(auxLinear4Func, auxLinear4Sensor, engineConfiguration->auxLinear4, 10);

	injectorPressure.setProxiedSensor(
		engineConfiguration->injectorPressureType == IPT_High
		? SensorType::FuelPressureHigh
		: SensorType::FuelPressureLow
	);

	injectorPressure.Register();
}

void deinitFluidPressure() {
	AdcSubscription::UnsubscribeSensor(oilpSensor, engineConfiguration->oilPressure.hwChannel);
	AdcSubscription::UnsubscribeSensor(fuelPressureSensorLow, engineConfiguration->lowPressureFuel.hwChannel);
	AdcSubscription::UnsubscribeSensor(fuelPressureSensorHigh, engineConfiguration->highPressureFuel.hwChannel);
	AdcSubscription::UnsubscribeSensor(auxLinear1Sensor, engineConfiguration->auxLinear1.hwChannel);
	AdcSubscription::UnsubscribeSensor(auxLinear2Sensor, engineConfiguration->auxLinear2.hwChannel);
	AdcSubscription::UnsubscribeSensor(auxLinear3Sensor, engineConfiguration->auxLinear3.hwChannel);
	AdcSubscription::UnsubscribeSensor(auxLinear4Sensor, engineConfiguration->auxLinear4.hwChannel);
}
