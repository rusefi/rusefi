#include "init.h"
#include "adc_subscription.h"
#include "engine.h"
#include "error_handling.h"
#include "global.h"
#include "function_pointer_sensor.h"
#include "ego.h"

EXTERN_ENGINE;

struct GetAfrWrapper {
	DECLARE_ENGINE_PTR;

	float getLambda() {
		return getAfr(PASS_ENGINE_PARAMETER_SIGNATURE) / 14.7f;
	}
};

static GetAfrWrapper afrWrapper;

static FunctionPointerSensor lambdaSensor(SensorType::Lambda,
[]() {
	return afrWrapper.getLambda();
});

void initLambda(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	INJECT_ENGINE_REFERENCE(&afrWrapper);

	if (!lambdaSensor.Register()) {
		warning(OBD_PCM_Processor_Fault, "Duplicate lambda sensor registration, ignoring");
	}
}
