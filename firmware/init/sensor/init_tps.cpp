#include "adc_subscription.h"
#include "engine.h"
#include "error_handling.h"
#include "global.h"
#include "functional_sensor.h"
#include "linear_func.h"
#include "tps.h"

EXTERN_ENGINE;

LinearFunc tpsFunc1p;
//LinearFunc tpsFunc1s;
LinearFunc tpsFunc2p;
//LinearFunc tpsFunc2s;

FunctionalSensor tpsSens1p(SensorType::Tps1, MS2NT(10));
//FunctionalSensor tpsSens1s(SensorType::Tps1Secondary, MS2NT(10));
FunctionalSensor tpsSens2p(SensorType::Tps2, MS2NT(10));
//FunctionalSensor tpsSens2s(SensorType::Tps2Secondary, MS2NT(10));

static void configureTps(LinearFunc& func, float closed, float open) {
	func.configure(
		closed / TPS_TS_CONVERSION, 0,
		open / TPS_TS_CONVERSION, 100, 
		CONFIG(tpsErrorDetectionTooLow),
		CONFIG(tpsErrorDetectionTooHigh)
	);
}

static void initTpsFunc(LinearFunc& func, FunctionalSensor& sensor, adc_channel_e channel, float closed, float open) {
	// Only register if we have a sensor
	if (channel == EFI_ADC_NONE) {
		return;
	}

	configureTps(func, closed, open);

	sensor.setFunction(func);

	AdcSubscription::SubscribeSensor(sensor, channel);

	if (!sensor.Register()) {
		firmwareError(CUSTOM_INVALID_TPS_SETTING, "Duplicate TPS registration for TPS sensor");
	}
}

void initTps() {
	initTpsFunc(tpsFunc1p, tpsSens1p, CONFIG(tps1_1AdcChannel), CONFIG(tpsMin), CONFIG(tpsMax));
	initTpsFunc(tpsFunc2p, tpsSens2p, CONFIG(tps2_1AdcChannel), CONFIG(tps2Min), CONFIG(tps2Max));
}

void reconfigureTps() {
	configureTps(tpsFunc1p, CONFIG(tpsMin), CONFIG(tpsMax));
	configureTps(tpsFunc2p, CONFIG(tps2Min), CONFIG(tps2Max));
}
