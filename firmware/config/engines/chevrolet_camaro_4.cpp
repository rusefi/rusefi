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

	engineConfiguration->specs.firingOrder = FO_1_8_7_2_6_5_4_3;
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_GM_LS_24;

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0; // PA0

	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->hasMafSensor = false;

	engineConfiguration->tpsAdcChannel = EFI_ADC_2;

}


