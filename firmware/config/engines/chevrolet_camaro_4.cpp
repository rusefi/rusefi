/*
 * @file chevrolet_camaro_4.cpp
 *
 *
 * set_engine_type 35
 *
 * @date Oct 16, 2015
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "chevrolet_camaro_4.h"

EXTERN_ENGINE
;

void setCamaro4(DECLARE_ENGINE_PARAMETER_F) {

	engineConfiguration->specs.displacement = 5.7;
	engineConfiguration->specs.cylindersCount = 8;

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0; // PA0

	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->hasMafSensor = false;

}


