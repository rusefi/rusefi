/**
 * @file	nissan_primera.c
 *
 * @date Oct 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"

#if EFI_SUPPORT_NISSAN_PRIMERA || defined(__DOXYGEN__)
#include "nissan_primera.h"

void setNissanPrimeraEngineConfiguration(engine_configuration_s *engineConfiguration) {
	engineConfiguration->triggerConfig.totalToothCount = 60;
	engineConfiguration->triggerConfig.skippedToothCount = 2;
}

void setNissanPrimeraengine_configuration2_s(engine_configuration_s *engineConfiguration, engine_configuration2_s *engineConfiguration2) {
}

#endif /* EFI_SUPPORT_NISSAN_PRIMERA */
