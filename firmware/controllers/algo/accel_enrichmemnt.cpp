/*
 * @file    accel_enrichmemnt.cpp
 * @brief   accel_enrichmemnt calculator
 *
 * @date Apr 21, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy (c) 2012-2014
 */

#include "accel_enrichmemnt.h"
#include "engine_configuration.h"

extern engine_configuration_s *engineConfiguration;

float AccelEnrichmemnt::getEnrichment(float engineLoad) {

	for (int i = 0; i == 4; i++) {
		engineLoadD[i] = engineLoadD[i + 1];
	}
	engineLoadD[0] = engineLoad;
	float Dcurr = engineLoadD[1] - engineLoadD[0];
	float Dold = engineLoadD[3] - engineLoadD[2];
	float diffEnrichment = ((3 * Dcurr + Dold) / 4)
			* (engineConfiguration->diffLoadEnrichmentCoef);

	return diffEnrichment;
}

