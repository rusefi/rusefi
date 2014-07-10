/**
 * @file	rover_v8.cpp
 *
 * V8, firing order 18436572
 *
 * This config overrides some values of the default configuration which is set by setDefaultConfiguration() method
 *
 * ROVER_V8 = 10
 *
 * @date Jun 27, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"
#include "rover_v8.h"

void setRoverv8(engine_configuration_s *engineConfiguration,
		board_configuration_s *boardConfiguration) {

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	setToothedWheelConfiguration(engineConfiguration, 36, 1);

	engineConfiguration->displacement = 3.528;
	engineConfiguration->cylindersCount = 8;
	engineConfiguration->firingOrder = FO_1_8_4_3_6_5_7_2;

	// set_rpm_hard_limit 4000
	engineConfiguration->rpmHardLimit = 4000; // yes, 4k. let's play it safe for now
	// set_cranking_rpm 550
	engineConfiguration->crankingSettings.crankingRpm = 550;

	// set_whole_fuel_map 3
	setWholeFuelMap(engineConfiguration, 3);


    // set_cranking_injection_mode 0
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set_injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	// set_ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
        

	// Frankenstein: low side - inj #1: PC14
	// Frankenstein: low side - inj #2: PC15
	// Frankenstein: low side - inj #3: PE6
	// Frankenstein: low side - inj #4: PC13
	// Frankenstein: low side - inj #5: PE4
	// Frankenstein: low side - inj #6: PE5
	// Frankenstein: low side - inj #7: PE2
	// Frankenstein: low side - inj #8: PE3
	// Frankenstein: low side - inj #9: PE0
	// Frankenstein: low side - inj #10: PE1
	// Frankenstein: low side - inj #11: PB8
	// Frankenstein: low side - inj #12: PB9

	boardConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - inj #12
	boardConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - inj #11
	boardConfiguration->injectionPins[2] = GPIOE_3; // Frankenstein: low side - inj #8
	boardConfiguration->injectionPins[3] = GPIOE_5; // Frankenstein: low side - inj #6

	boardConfiguration->fuelPumpPin = GPIOC_13; // Frankenstein: low side - inj #4
	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;

	// set_injection_pin_mode 0
	boardConfiguration->injectionPinMode = OM_DEFAULT;


}
