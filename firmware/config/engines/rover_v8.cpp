/**
 * @file	rover_v8.cpp
 *
 * V8, firing order 18436572
 *
 * This config overrides some values of the default configuration which is set by setDefaultConfiguration() method
 *
 * set_engine_type 10
 *
 * ROVER_V8 = 10
 *
 * @date Jun 27, 2014
 * @author Andrey Belomutskiy, (c) 2012-2016
 */

#include "main.h"
#include "rover_v8.h"

void setFrankenstein_01_LCD(board_configuration_s *boardConfiguration) {
	boardConfiguration->HD44780_rs = GPIOE_9;
	boardConfiguration->HD44780_e = GPIOE_11;
	boardConfiguration->HD44780_db4 = GPIOE_13;
	boardConfiguration->HD44780_db5 = GPIOE_15;
	boardConfiguration->HD44780_db6 = GPIOB_11;
	boardConfiguration->HD44780_db7 = GPIOB_13;
}

EXTERN_ENGINE;

void setRoverv8(DECLARE_ENGINE_PARAMETER_F) {
	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_TOOTHED_WHEEL_36_1;

	engineConfiguration->specs.displacement = 3.528;
	engineConfiguration->specs.cylindersCount = 8;
	engineConfiguration->specs.firingOrder = FO_1_8_4_3_6_5_7_2;

	// set_rpm_hard_limit 4000
	engineConfiguration->rpmHardLimit = 4000; // yes, 4k. let's play it safe for now
	// set_cranking_rpm 550
	engineConfiguration->cranking.rpm = 550;

	// set_whole_fuel_map 3
	setWholeFuelMap(3 PASS_ENGINE_PARAMETER);

	// set_cranking_injection_mode 0
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set_injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	// set_ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;

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

	boardConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
	boardConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
	boardConfiguration->injectionPins[2] = GPIOE_3; // Frankenstein: low side - out #8
	boardConfiguration->injectionPins[3] = GPIOE_5; // Frankenstein: low side - out #6

	boardConfiguration->fuelPumpPin = GPIOC_13; // Frankenstein: low side - out #4
	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;

	// set_injection_pin_mode 0
	boardConfiguration->injectionPinMode = OM_DEFAULT;

	boardConfiguration->canTxPin = GPIOB_6;
	boardConfiguration->canRxPin = GPIOB_12;
	engineConfiguration->canWriteEnabled = true;
	engineConfiguration->canReadEnabled = false;
	engineConfiguration->canNbcType = CAN_BUS_MAZDA_RX8;
}
