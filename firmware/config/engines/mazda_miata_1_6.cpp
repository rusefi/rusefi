/*
 * @file mazda_miata_1_6.cpp
 *
 * @date Apr 16, 2016
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "engine_template.h"
#include "custom_engine.h"

EXTERN_ENGINE;

void setMiataNA_1_6_Configuration(DECLARE_ENGINE_PARAMETER_F) {
	setCustomEngineConfiguration(PASS_ENGINE_PARAMETER_F);


	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_THEN_3_THEN_4_THEN2;


}


