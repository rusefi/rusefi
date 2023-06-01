/**
 * @file	mitsubishi.cpp
 *
 * MITSU_4G93 16
 *
 * set engine_type 16
 *
 * @date Aug 5, 2014
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"

#include "mitsubishi.h"

void setMitsubishiConfiguration() {

	engineConfiguration->trigger.type = trigger_type_e::TT_MITSUBISHI_4G93; // same trigger as 4G63?

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->displacement = 1.800;

	// set ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	engineConfiguration->firingOrder = FO_1_3_4_2;

	// set global_trigger_offset_angle 671
	engineConfiguration->globalTriggerAngleOffset = 671;

	// set cranking_timing_angle 715
	engineConfiguration->crankingTimingAngle = -715;

//	/**
//	 * 29150 Ohm @ 40C
//	 * 10160 Ohm @ 70C
//	 * 1270 Ohm @ 150C
//	 */
//	setThermistorConfiguration(&engineConfiguration->clt, 40, 29150, 70, 10160, 150, 1270);

	setCommonNTCSensor(&engineConfiguration->clt, 2700);

	// Frankenstein: low side - out #1: PC14
	// Frankenstein: low side - out #2: PC15
	// Frankenstein: low side - out #3: PE6
	// Frankenstein: low side - out #4: PC13
	// Frankenstein: low side - out #5: PE4
	// Frankenstein: low side - out #6: PE5
	// Frankenstein: low side - out #7: PE2
	// Frankenstein: low side - out #8: PE3
	// Frankenstein: low side - out #9: PE0
	// Frankenstein: low side - out #10: PE1
	// Frankenstein: low side - out #11: PB8
	// Frankenstein: low side - out #12: PB9

	engineConfiguration->injectionPins[0] = Gpio::B9; // Frankenstein: low side - out #12
	engineConfiguration->injectionPins[1] = Gpio::B8; // Frankenstein: low side - out #11
	engineConfiguration->injectionPins[2] = Gpio::E3; // Frankenstein: low side - out #8
	engineConfiguration->injectionPins[3] = Gpio::E5; // Frankenstein: low side - out #6


	// Frankenstein: high side #1: PE8
	// Frankenstein: high side #2: PE10

	engineConfiguration->ignitionPins[0] = Gpio::E8; // Frankenstein: high side #1
	engineConfiguration->ignitionPins[1] = Gpio::Unassigned;
	engineConfiguration->ignitionPins[2] = Gpio::E10; // // Frankenstein: high side #2
	engineConfiguration->ignitionPins[3] = Gpio::Unassigned;

	setEgoSensor(ES_Innovate_MTX_L);

}


