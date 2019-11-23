/**
 * @file	nissan_primera.cpp
 *
 * NISSAN_PRIMERA
 * set engine_type 5
 *
 * @date Oct 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2019
 */

#include "global.h"

#if EFI_SUPPORT_NISSAN_PRIMERA
#include "nissan_primera.h"

EXTERN_CONFIG;

void setNissanPrimeraEngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setDefaultFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

	engineConfiguration->trigger.type = TT_NISSAN_SR20VE;

//	engineConfiguration->auxValves[0]
}

void setNissanPrimeraEngineConfiguration_360(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_NISSAN_SR20VE_360;
}

#endif /* EFI_SUPPORT_NISSAN_PRIMERA */
