/*
 * @file ford_festiva.cpp
 *
 * todo: revisit "unit" tests and remove this configuration?
 *
 * http://rusefi.com/wiki/index.php?title=Vehicle:Mazda_Protege_1993
 * http://rusefi.com/forum/viewtopic.php?f=3&t=537
 * http://rusefi.com/forum/viewtopic.php?f=3&t=822
 *
 * @date Jan 3, 2016
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "ford_festiva.h"
#include "custom_engine.h"

/**
 * pin 1I/W9 - extra +5v
 * set engine_type 14
 */
void setFordEscortGt() {
	engineConfiguration->trigger.type = trigger_type_e::TT_MAZDA_DOHC_1_4;

	engineConfiguration->displacement = 1.839;
	setAlgorithm(engine_load_mode_e::LM_SPEED_DENSITY);

	// in case of DOHC distributor we have two signals
	engineConfiguration->triggerInputPins[0] = Gpio::C6;
	engineConfiguration->triggerInputPins[1] = Gpio::A5; // 2E White CKP
	// set global_trigger_offset_angle -37
	engineConfiguration->globalTriggerAngleOffset = -37;

	// todo: change to 15?
	// set cranking_timing_angle 3
	engineConfiguration->crankingTimingAngle = 3;

	setSingleCoilDwell();
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;

	engineConfiguration->injectionPins[0] = Gpio::D3;
	engineConfiguration->injectionPins[1] = Gpio::E2;
}

