#include "pch.h"

#include "init.h"
#include "adc_subscription.h"
#include "function_pointer_sensor.h"

struct GetAfrWrapper : public EnginePtr {
	float getLambda() {
		return getAfr(PASS_ENGINE_PARAMETER_SIGNATURE) / 14.7f;
	}
};

static GetAfrWrapper afrWrapper;

static FunctionPointerSensor lambdaSensor(SensorType::Lambda1,
[]() {
	return afrWrapper.getLambda();
});

#if EFI_CAN_SUPPORT
#include "AemXSeriesLambda.h"
static AemXSeriesWideband aem1(0, SensorType::Lambda1);
static AemXSeriesWideband aem2(1, SensorType::Lambda2);
#endif

void initLambda(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	afrWrapper.inject(PASS_ENGINE_PARAMETER_SIGNATURE);

#if EFI_CAN_SUPPORT
	if (CONFIG(enableAemXSeries)) {
		if (!CONFIG(canWriteEnabled) || !CONFIG(canReadEnabled)) {
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
