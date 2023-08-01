/*
 * @file honda_600.cpp
 *
 * set engine_type 43
 *
 * @date Jul 9, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "engine_template.h"
#include "honda_600.h"
#include "custom_engine.h"

void setHonda600() {

	engineConfiguration->trigger.type = trigger_type_e::TT_HONDA_CBR_600;


	// set global_trigger_offset_angle 180
	// set global_trigger_offset_angle 540
	engineConfiguration->globalTriggerAngleOffset = 70; // huh why is this here? do we need better tdcPosition in configureHondaCbr600?

    engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
    engineConfiguration->cylindersCount = 4;
  engineConfiguration->firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

}

