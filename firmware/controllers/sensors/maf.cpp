#include "main.h"
#include "engine.h"
#include "adc_inputs.h"
#include "maf.h"

EXTERN_ENGINE
;

float getMaf(DECLARE_ENGINE_PARAMETER_F) {
	return getMafT(engineConfiguration);
}

void setBosch0280218037(engine_configuration_s *engineConfiguration) {
	int i = 0;
	engineConfiguration->mafDecoding[i] = -34.5;
	engineConfiguration->mafDecodingBins[i++] = 0;

	engineConfiguration->mafDecoding[i] = -6;
	engineConfiguration->mafDecodingBins[i++] = 0.78125;

	engineConfiguration->mafDecoding[i] = 10.5;
	engineConfiguration->mafDecodingBins[i++] = 1.38671875;

	engineConfiguration->mafDecoding[i] = 105.3;
	engineConfiguration->mafDecodingBins[i++] = 2.91015625;

	engineConfiguration->mafDecoding[i] = 387.5;
	engineConfiguration->mafDecodingBins[i++] = 4.2578125;

	engineConfiguration->mafDecoding[i] = 738;
	engineConfiguration->mafDecodingBins[i++] = 4.98046875;

	while (i < MAF_DECODING_COUNT) {
		engineConfiguration->mafDecoding[i] = 738;
		engineConfiguration->mafDecodingBins[i++] = 4.98046875 + i;
	}
}
