#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "linear_func.h"
#include "live_data.h"

static LinearFunc func;

static FunctionalSensor lambdaSensor(SensorType::Lambda1, MS2NT(50));
static FunctionalSensor lambdaSensor2(SensorType::Lambda2, MS2NT(50));

#include "AemXSeriesLambda.h"

#if EFI_CAN_SUPPORT
static AemXSeriesWideband aem1(0, SensorType::Lambda1);
static AemXSeriesWideband aem2(1, SensorType::Lambda2);
#endif

template <>
const wideband_state_s* getLiveData(size_t idx) {
#if EFI_CAN_SUPPORT
	switch (idx) {
		case 0: return &aem1;
		case 1: return &aem2;
	}
#endif

	return nullptr;
}

static void initLambdaSensor(FunctionalSensor& sensor, adc_channel_e channel) {
	if (!isAdcChannelValid(channel)) {
		return;
	}

	AdcSubscription::SubscribeSensor(sensor, channel, 10);
	sensor.Register();
}

void initLambda() {

#if EFI_CAN_SUPPORT
	if (engineConfiguration->enableAemXSeries) {
		if (!engineConfiguration->canWriteEnabled || !engineConfiguration->canReadEnabled) {
			firmwareError(ObdCode::OBD_PCM_Processor_Fault, "CAN read and write are required to use CAN wideband.");
			return;
		}

		registerCanSensor(aem1);
		registerCanSensor(aem2);

		return;
	}
#endif

	auto& cfg = engineConfiguration->afr;
	float minLambda = (cfg.value1 + engineConfiguration->egoValueShift) / 14.7f;
	float maxLambda = (cfg.value2 + engineConfiguration->egoValueShift) / 14.7f;

	func.configure(cfg.v1, minLambda, cfg.v2, maxLambda, 0, 5);

	lambdaSensor.setFunction(func);
	lambdaSensor2.setFunction(func);

	initLambdaSensor(lambdaSensor, engineConfiguration->afr.hwChannel);
	initLambdaSensor(lambdaSensor2, engineConfiguration->afr.hwChannel2);
}

void deinitLambda() {
	AdcSubscription::UnsubscribeSensor(lambdaSensor, engineConfiguration->afr.hwChannel);
	AdcSubscription::UnsubscribeSensor(lambdaSensor2, engineConfiguration->afr.hwChannel2);
}
