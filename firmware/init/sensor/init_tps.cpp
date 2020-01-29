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

LinearFunc tpsFunc1p;
//LinearFunc tpsFunc1s;
LinearFunc tpsFunc2p;
//LinearFunc tpsFunc2s;

FunctionalSensor tpsSens1p(SensorType::Tps1Primary, MS2NT(10));
//FunctionalSensor tpsSens1s(SensorType::Tps1Secondary, MS2NT(10));
FunctionalSensor tpsSens2p(SensorType::Tps2Primary, MS2NT(10));
//FunctionalSensor tpsSens2s(SensorType::Tps2Secondary, MS2NT(10));

static void initTpsFunc(LinearFunc& func, FunctionalSensor& sensor, adc_channel_e channel, float closed, float open) {
	// Only register if we have a sensor
	if (channel == EFI_ADC_NONE) {
		return;
	}

	func.configure(
		closed / 200.0f, 0,
		open / 200.0f, 100, 
		CONFIG(tpsErrorDetectionTooLow),
		CONFIG(tpsErrorDetectionTooHigh)
	);

	sensor.setFunction(func);

	AdcSubscription::SubscribeSensor(sensor, channel);

	if (!sensor.Register()) {
		// todo: handle
	}
}

void initTps() {
	initTpsFunc(tpsFunc1p, tpsSens1p, CONFIG(tps1_1AdcChannel), CONFIG(tpsMin), CONFIG(tpsMax));
	initTpsFunc(tpsFunc2p, tpsSens2p, CONFIG(tps2_1AdcChannel), CONFIG(tps2Min), CONFIG(tps2Max));
}
