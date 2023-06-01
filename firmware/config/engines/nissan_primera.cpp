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
	engineConfiguration->trigger.type = trigger_type_e::TT_NISSAN_SR20VE;

	engineConfiguration->auxValves[0] = Gpio::E14;
	engineConfiguration->auxValves[1] = Gpio::E12;
}
