#include "adc_subscription.h"
#include "engine.h"
#include "error_handling.h"
#include "global.h"
#include "functional_sensor.h"
#include "redundant_sensor.h"
#include "proxy_sensor.h"
#include "linear_func.h"
#include "tps.h"

EXTERN_ENGINE;

LinearFunc tpsFunc1p(TPS_TS_CONVERSION);
LinearFunc tpsFunc1s(TPS_TS_CONVERSION);
LinearFunc tpsFunc2p(TPS_TS_CONVERSION);
LinearFunc tpsFunc2s(TPS_TS_CONVERSION);

FunctionalSensor tpsSens1p(SensorType::Tps1Primary, MS2NT(10));
FunctionalSensor tpsSens1s(SensorType::Tps1Secondary, MS2NT(10));
FunctionalSensor tpsSens2p(SensorType::Tps2Primary, MS2NT(10));
FunctionalSensor tpsSens2s(SensorType::Tps2Secondary, MS2NT(10));

RedundantSensor tps1(SensorType::Tps1, SensorType::Tps1Primary, SensorType::Tps1Secondary);
RedundantSensor tps2(SensorType::Tps2, SensorType::Tps2Primary, SensorType::Tps2Secondary);

LinearFunc pedalFunc;
FunctionalSensor pedalSensor(SensorType::AcceleratorPedal, MS2NT(10));

// This sensor indicates the driver's throttle intent - Pedal if we have one, TPS if not.
ProxySensor driverIntent(SensorType::DriverThrottleIntent);

static void configureTps(LinearFunc& func, float closed, float open) {
	func.configure(
		closed, 0,
		open, 100, 
		CONFIG(tpsErrorDetectionTooLow),
		CONFIG(tpsErrorDetectionTooHigh)
	);
}

static bool initTpsFunc(LinearFunc& func, FunctionalSensor& sensor, adc_channel_e channel, float closed, float open) {
	// Only register if we have a sensor
	if (channel == EFI_ADC_NONE) {
		return false;
	}

	configureTps(func, closed, open);

	sensor.setFunction(func);

	AdcSubscription::SubscribeSensor(sensor, channel);

	if (!sensor.Register()) {
		firmwareError(CUSTOM_INVALID_TPS_SETTING, "Duplicate TPS registration for TPS sensor");
		return false;
	}

	return true;
}

void initTpsFuncAndRedund(RedundantSensor& redund, LinearFunc& func, FunctionalSensor& sensor, adc_channel_e channel, float closed, float open) {
	bool hasSecond = initTpsFunc(func, sensor, channel, closed, open);

	redund.configure(5.0f, !hasSecond);

	if (!redund.Register()) {
		//todo: error
	}
}

void initTps() {
	initTpsFunc(tpsFunc1p, tpsSens1p, CONFIG(tps1_1AdcChannel), CONFIG(tpsMin), CONFIG(tpsMax));
	initTpsFuncAndRedund(tps1, tpsFunc12, tpsSens12, CONFIG(tps1_2AdcChannel), CONFIG(tpsMin), CONFIG(tpsMax));
	initTpsFunc(tpsFunc2p, tpsSens2p, CONFIG(tps2_1AdcChannel), CONFIG(tps2Min), CONFIG(tps2Max));
	initTpsFuncAndRedund(tps2, tpsFunc2s, tpsSens2s, CONFIG(tps2_2AdcChannel), CONFIG(tps2Min), CONFIG(tps2Max));
	initTpsFunc(pedalFunc, pedalSensor, CONFIG(throttlePedalPositionAdcChannel), CONFIG(throttlePedalUpVoltage), CONFIG(throttlePedalWOTVoltage));

	// Route the pedal or TPS to driverIntent as appropriate
	if (CONFIG(throttlePedalPositionAdcChannel) != EFI_ADC_NONE) {
		driverIntent.setProxiedSensor(SensorType::AcceleratorPedal);
	} else {
		driverIntent.setProxiedSensor(SensorType::Tps1);
	}

	if (!driverIntent.Register()) {
		firmwareError(CUSTOM_INVALID_TPS_SETTING, "Duplicate TPS registration for TPS sensor");
	}
}

void reconfigureTps() {
	configureTps(tpsFunc1p, CONFIG(tpsMin), CONFIG(tpsMax));
	configureTps(tpsFunc2p, CONFIG(tps2Min), CONFIG(tps2Max));
	configureTps(pedalFunc, CONFIG(throttlePedalUpVoltage), CONFIG(throttlePedalWOTVoltage));
}
