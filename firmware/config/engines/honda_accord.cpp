/**
 * @file	honda_accord.c
 *
 * @date Jan 12, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "engine_configuration.h"
#include "trigger_decoder.h"

void setHondaAccordConfiguration(engine_configuration_s *engineConfiguration) {
	engineConfiguration->triggerConfig.totalToothCount = 24;
	engineConfiguration->triggerConfig.skippedToothCount = 2;
}

