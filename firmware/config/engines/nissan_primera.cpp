/**
 * @file	nissan_primera.cpp
 *
 * NISSAN_PRIMERA
 * set engine_type 5
 *
 * @date Oct 14, 2013
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "nissan_primera.h"

void setNissanPrimeraEngineConfiguration() {
	setDefaultFrankensoConfiguration();

	engineConfiguration->trigger.type = TT_NISSAN_SR20VE;

	engineConfiguration->auxValves[0] = GPIOE_14;
	engineConfiguration->auxValves[1] = GPIOE_12;
}

void setNissanPrimeraEngineConfiguration_360() {
	engineConfiguration->trigger.type = TT_NISSAN_SR20VE_360;
}
