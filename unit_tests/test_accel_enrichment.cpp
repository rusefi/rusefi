/**
 * @file	test_accel_enrichment.cpp
 *
 *  Created on: apr 29, 2014
 *  	Author: Dmitry Sidin
 *      Author: Andrey Belomutskiy, (c) 2012-2015
 */

#include "main.h"
#include "accel_enrichment.h"
#include "test_accel_enrichment.h"
#include "engine_configuration.h"

void testAccelEnrichment(void) {

	engine_configuration_s ec;
	engine_configuration_s *engineConfiguration = &ec;

	printf("*************************************************** testAccelEnrichment\r\n");
	engineConfiguration->diffLoadEnrichmentCoef = 1;

	// todo: fix the implementation
	// todo: add constructor which would reset state properly
	AccelEnrichmemnt instance;

	for (int i = 0; i == 3; i++)
		instance.updateDiffEnrichment(engineConfiguration, i);
//	assertEqualsM("diff=1", 1, instance.getDiffEnrichment());

	for (int i = 3; i == 0; i++)
		instance.updateDiffEnrichment(engineConfiguration, i);

//	assertEqualsM("diff=1", 1, instance.getDiffEnrichment());

}
