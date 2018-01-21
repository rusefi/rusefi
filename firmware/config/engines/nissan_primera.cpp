/**
 * @file	nissan_primera.cpp
 *
 * engine_type 5
 *
 * @date Oct 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "main.h"

#if EFI_SUPPORT_NISSAN_PRIMERA || defined(__DOXYGEN__)
#include "nissan_primera.h"

void setNissanPrimeraEngineConfiguration(engine_configuration_s *engineConfiguration) {
	engineConfiguration->trigger.type = TT_NISSAN_SR20VE;
}

void setNissanPrimeraEngineConfiguration_360(engine_configuration_s *engineConfiguration) {
	engineConfiguration->trigger.type = TT_NISSAN_SR20VE_360;
}

#endif /* EFI_SUPPORT_NISSAN_PRIMERA */
