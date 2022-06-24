#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "function_pointer_sensor.h"
#include "live_data.h"

struct GetAfrWrapper {
	float getLambda() {
		return getAfr() / 14.7f;
	}
};

static GetAfrWrapper afrWrapper;

static FunctionPointerSensor lambdaSensor(SensorType::Lambda1,
[]() {
	return afrWrapper.getLambda();
});

#include "AemXSeriesLambda.h"

#if EFI_CAN_SUPPORT
static AemXSeriesWideband aem1(0, SensorType::Lambda1);
static AemXSeriesWideband aem2(1, SensorType::Lambda2);
#endif

template <>
const wideband_state_s* getLiveDataAddr(size_t idx) {
#if EFI_CAN_SUPPORT
	switch (idx) {
		case 0: return &aem1;
		case 1: return &aem2;
	}
#endif

	return nullptr;
}

void initLambda() {

#if EFI_CAN_SUPPORT
	if (engineConfiguration->enableAemXSeries) {
		if (!engineConfiguration->canWriteEnabled || !engineConfiguration->canReadEnabled) {
			firmwareError(OBD_PCM_Processor_Fault, "CAN read and write are required to use CAN wideband.");
			return;
		}

		registerCanSensor(aem1);
		registerCanSensor(aem2);

		return;
	}
#endif

	lambdaSensor.Register();
}
