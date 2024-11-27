#include "pch.h"

#include "init.h"
#include "sent.h"
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

static LinearFunc acPressureFunc;
static FunctionalSensor acPressureSensor(SensorType::AcPressure, /* timeout = */ MS2NT(50));

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

#if EFI_SENT_SUPPORT
static void initSentLinearSensor(LinearFunc& func, FunctionalSensor& sensor, int in1, float out1, int in2, float out2, float min, float max)
{
	func.configure(in1, out1, in2, out2, min, max);

	sensor.setFunction(func);

	sensor.Register();
}
#endif

void initFluidPressure() {
	initFluidPressure(oilpSensorFunc, oilpSensor, engineConfiguration->oilPressure, 10);
	initFluidPressure(fuelPressureFuncLow, fuelPressureSensorLow, engineConfiguration->lowPressureFuel, 10);

#if EFI_SENT_SUPPORT
	if ((engineConfiguration->FuelHighPressureSentType != SentFuelHighPressureType::NONE) &&
		(engineConfiguration->FuelHighPressureSentInput != SentInput::NONE)) {
		if (engineConfiguration->FuelHighPressureSentType == SentFuelHighPressureType::GM_TYPE) {
			/* This sensor sends two pressure signals:
			 * Sig0 occupies 3 first nibbles
			 * Sig1 occupies next 3 nibbles
			 * Signals are close, but not identical.
			 * Sig0 shows about 197..198 at 1 Atm (open air) and 282 at 1000 KPa (10 Bar)
			 * Sig1 shows abour 202..203 at 1 Atm (open air) and 283 at 1000 KPa (10 Bar)
			 */
			initSentLinearSensor(fuelPressureFuncHigh, fuelPressureSensorHigh,
				200, BAR2KPA(1),
				283, BAR2KPA(10),
				BAR2KPA(0), BAR2KPA(1000) /* What is limit of this sensor? */);
		}
	} else
#endif
	{
		initFluidPressure(fuelPressureFuncHigh, fuelPressureSensorHigh, engineConfiguration->highPressureFuel, 100);
	}
    initFluidPressure(acPressureFunc, acPressureSensor, engineConfiguration->acPressure, 10);
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

#if EFI_PROD_CODE
#if EFI_SENT_SUPPORT
/* init_ file is not correct place for following code, but pressure sensor is defined here and static */
/* TODO: move? */

void sentPressureDecode(SentInput sentCh) {
	if (engineConfiguration->FuelHighPressureSentInput != sentCh) {
		return;
	}

	if (engineConfiguration->FuelHighPressureSentType == SentFuelHighPressureType::GM_TYPE) {
		uint16_t sig0, sig1;
		int ret = getSentValues(sentCh, &sig0, &sig1);

		if (ret) {
			return;
		}

		/* This sensor sends two pressure signals - average */
		fuelPressureSensorHigh.postRawValue(((float)sig0 + (float)sig1) / 2, getTimeNowNt());
	}
}

#endif /* EFI_SENT_SUPPORT */
#endif /* EFI_PROD_CODE */
