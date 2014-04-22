/**
 * @file	mazda_miata_nb.c
 *
 * @date Feb 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "mazda_miata_nb.h"

void setMazdaMiataNbEngineConfiguration(engine_configuration_s *engineConfiguration) {
	engineConfiguration->triggerConfig.triggerType = TT_MAZDA_MIATA_NB;
	engineConfiguration->triggerConfig.isSynchronizationNeeded = TRUE;

	engineConfiguration->triggerConfig.syncRatioFrom = 0.11 * 0.7;
	engineConfiguration->triggerConfig.syncRatioTo = 0.11 * 1.3;
	engineConfiguration->triggerConfig.useRiseEdge = FALSE;
	engineConfiguration->globalTriggerAngleOffset = 320;
}

void setMazdaMiataNb_configuration2_s(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2) {
}

