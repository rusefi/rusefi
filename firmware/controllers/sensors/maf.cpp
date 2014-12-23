#include "main.h"
#include "engine.h"
#include "adc_inputs.h"
#include "maf.h"

EXTERN_ENGINE;

float getMaf(DECLARE_ENGINE_PARAMETER_F) {
	return getMafT(engineConfiguration);
}
