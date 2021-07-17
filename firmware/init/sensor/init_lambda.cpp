#include "init.h"
#include "adc_subscription.h"
#include "error_handling.h"
#include "function_pointer_sensor.h"
#include "ego.h"

struct GetAfrWrapper {
	DECLARE_ENGINE_PTR;

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
	INJECT_ENGINE_REFERENCE(&afrWrapper);

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
