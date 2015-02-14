#include "main.h"
#include "engine.h"
#include "adc_inputs.h"
#include "maf.h"

EXTERN_ENGINE
;

/**
 * @return MAF sensor voltage
 */
float getMaf(DECLARE_ENGINE_PARAMETER_F) {
	return getMafT(engineConfiguration);
}

/**
 * @return kg/hour value
 */
float getRealMaf(DECLARE_ENGINE_PARAMETER_F) {
	int mafAdc = getAdcValue(engineConfiguration->mafAdcChannel);
	/**
	 * here we drop from 12 bit ADC to 8 bit index
	 */
	return engine->mafDecodingLookup[mafAdc >> 4];
}

static void fillTheRest(engine_configuration_s *e, int i) {
	/**
	 * unrealistic values just to make binary search happy
	 */
	while (i < MAF_DECODING_COUNT) {
		e->mafDecoding[i] = 200;
		e->mafDecodingBins[i] = 10 + i;
		i++;
	}
}

static int addMafPoint(engine_configuration_s *e, int i, float kgHrValue, float voltage) {
	e->mafDecoding[i] = kgHrValue;
	e->mafDecodingBins[i] = voltage;
	return i + 1;
}

void setBosch0280218037(engine_configuration_s *e) {
	int i = 0;
	i = addMafPoint(e, i, -34.5, 0);
	i = addMafPoint(e, i, -6, 0.78125);

	e->mafDecoding[i] = 10.5;
	e->mafDecodingBins[i++] = 1.38671875;

	e->mafDecoding[i] = 105.3;
	e->mafDecodingBins[i++] = 2.91015625;

	e->mafDecoding[i] = 387.5;
	e->mafDecodingBins[i++] = 4.2578125;

	e->mafDecoding[i] = 738;
	e->mafDecodingBins[i++] = 4.98046875;

	fillTheRest(e, i);
}

void setBosch0280218004(engine_configuration_s *e) {
	int i = 0;

	fillTheRest(e, i);
}

void setDensoTODO(engine_configuration_s *e) {
	int i = 0;

	fillTheRest(e, i);
}
