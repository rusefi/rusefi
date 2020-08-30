#include "init.h"
#include "adc_subscription.h"
#include "engine.h"
#include "error_handling.h"
#include "global.h"
#include "function_pointer_sensor.h"
#include "ego.h"

EXTERN_ENGINE;

static FunctionPointerSensor lambdaSensor(SensorType::Lambda,
[]() {
	return getAfr(PASS_ENGINE_PARAMETER_SIGNATURE) / 14.7f;
});

void init_lambda(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	if (!lambdaSensor.Register()) {
		warning(OBD_PCM_Processor_Fault, "Duplicate lambda sensor registration, ignoring");
	}
}
