/**
 * @file    accel_enrichment.h
 * @brief   Acceleration enrichment calculator
 *
 * @date Apr 21, 2014
 * @author Dmitry Sidin
 * @author Andrey Belomutskiy (c) 2012-2014
 */

#ifndef ACC_ENRICHMENT_H_
#define ACC_ENRICHMENT_H_

#include "engine_configuration.h"

class AccelEnrichmemnt {
public:
	void updateDiffEnrichment(engine_configuration_s *engineConfiguration, float engineLoad);
	float getDiffEnrichment(void);
private:
	float engineLoadD[5];
	float diffEnrichment;
};

void initDiffEnrichment(void);
float getAccelEnrichment(void);

#endif /* ACC_ENRICHMENT_H_ */
