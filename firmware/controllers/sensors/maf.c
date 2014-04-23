#include "main.h"
#include "boards.h"
#include "engine_configuration.h"
#include "adc_inputs.h"

extern engine_configuration_s *engineConfiguration;

float getMaf(void) {
	return getVoltageDivided(engineConfiguration->mafAdcChannel);
}
