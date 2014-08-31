/**
 * @file	ford_escort_gt.cpp
 *
 * FORD_ESCORT_GT = 14
 *
 * MIATA_1990 = 19
 * MIATA_1994 = 20
 * MIATA_1996 = 21
 *
 * @date Apr 11, 2014
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "ford_escort_gt.h"
#include "engine_math.h"

static void setDefaultCrankingFuel(engine_configuration_s *engineConfiguration) {
	// todo: set cranking parameters method based on injectors and displacement?

	// since CLT is not wired up yet let's just use same value for min and max
	// set_cranking_fuel_max 6 40
	engineConfiguration->crankingSettings.coolantTempMaxC = 37.7; // 6ms at 37.7C
	engineConfiguration->crankingSettings.fuelAtMaxTempMs = 6;

	// set_cranking_fuel_min 6 -40
	engineConfiguration->crankingSettings.coolantTempMinC = -40; // 6ms at -40C
	engineConfiguration->crankingSettings.fuelAtMinTempMs = 6;
}

static void common079721_2351(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {

	engineConfiguration->digitalChartSize = 150;

	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_THEN_3_THEN_4_THEN2;

	boardConfiguration->fuelPumpPin = GPIO_NONE; // fuel pump is not controlled by ECU on this engine

	// set_cranking_injection_mode 0
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set_injection_mode 2
	engineConfiguration->injectionMode = IM_BATCH;

	// Frankenstein analog input #1: adc1
	// Frankenstein analog input #2: adc3
	// Frankenstein analog input #3: adc13
	// Frankenstein analog input #4: adc11
	// Frankenstein analog input #5: adc
	// Frankenstein analog input #6: adc
	// Frankenstein analog input #7: adc
	// Frankenstein analog input #8: adc
	// Frankenstein analog input #9: adc
	// Frankenstein analog input #10: adc
	// Frankenstein analog input #11: adc
	// Frankenstein analog input #12: adc
	engineConfiguration->mafAdcChannel = EFI_ADC_1;
	engineConfiguration->tpsAdcChannel = EFI_ADC_3;
	engineConfiguration->cltAdcChannel = EFI_ADC_11;
}

void setMiata1990(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {
	engineConfiguration->triggerConfig.triggerType = TT_MAZDA_MIATA_NA;

	common079721_2351(engineConfiguration, boardConfiguration);


	// Frankenstein: high side #1 is PE8
	// Frankenstein: high side #2 is PE10
	// Frankenstein: high side #3 is PE12
	// Frankenstein: high side #4 is PE14
	// Frankenstein: high side #5 is PC9
	// Frankenstein: high side #6 is PC7

	boardConfiguration->ignitionPins[0] = GPIOE_12; // Frankenstein: high side #3
	boardConfiguration->ignitionPins[1] = GPIOE_14; // Frankenstein: high side #4
	boardConfiguration->ignitionPins[2] = GPIO_NONE;
	boardConfiguration->ignitionPins[3] = GPIO_NONE;
	boardConfiguration->ignitionPinMode = OM_DEFAULT;

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
	boardConfiguration->injectionPins[2] = GPIO_NONE;
	boardConfiguration->injectionPins[3] = GPIO_NONE;
	boardConfiguration->injectionPins[4] = GPIO_NONE;
	boardConfiguration->injectionPins[5] = GPIO_NONE;
	boardConfiguration->injectionPinMode = OM_DEFAULT;


// todo: idleValvePin
}

void setFordEscortGt(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {
	engineConfiguration->triggerConfig.triggerType = TT_FORD_ESCORT_GT;

	common079721_2351(engineConfiguration, boardConfiguration);

	// set_global_trigger_offset_angle 256
	engineConfiguration->globalTriggerAngleOffset = 256;
	// set_ignition_offset 170
	engineConfiguration->ignitionOffset = 170;
	// set_injection_offset 510
	engineConfiguration->injectionOffset = 59;

	setSingleCoilDwell(engineConfiguration);
	engineConfiguration->ignitionMode = IM_ONE_COIL;

	boardConfiguration->triggerSimulatorPinModes[0] = OM_OPENDRAIN;
	boardConfiguration->triggerSimulatorPinModes[1] = OM_OPENDRAIN;

	// Frankenstein: high side #1 is PE8
	// Frankenstein: high side #2 is PE10
	// Frankenstein: high side #3 is PE12
	// Frankenstein: high side #4 is PE14
	// Frankenstein: high side #5 is PC9
	// Frankenstein: high side #6 is PC7

	boardConfiguration->ignitionPins[0] = GPIOE_12; // Frankenstein: high side #3
	boardConfiguration->ignitionPins[1] = GPIO_NONE;
	boardConfiguration->ignitionPins[2] = GPIO_NONE;
	boardConfiguration->ignitionPins[3] = GPIO_NONE;
	boardConfiguration->ignitionPinMode = OM_DEFAULT;

	// set_whole_fuel_map 3
	setWholeFuelMap(engineConfiguration, 3);

	setDefaultCrankingFuel(engineConfiguration);
}

/**
 * set_engine_type 20
 */
void setMiata1994(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {
	engineConfiguration->triggerConfig.triggerType = TT_MAZDA_MIATA_NA;
	engineConfiguration->displacement = 1.839;

	boardConfiguration->triggerSimulatorPins[0] = GPIOD_2; // 2G - YEL/BLU
	boardConfiguration->triggerSimulatorPins[1] = GPIOB_3; // 2E - WHT - four times
	boardConfiguration->triggerSimulatorPinModes[0] = OM_OPENDRAIN;
	boardConfiguration->triggerSimulatorPinModes[1] = OM_OPENDRAIN;

	boardConfiguration->triggerInputPins[0] = GPIO_NONE;
	boardConfiguration->triggerInputPins[1] = GPIO_NONE;

	boardConfiguration->is_enabled_spi_1 = false;
	boardConfiguration->is_enabled_spi_2 = false;
	boardConfiguration->is_enabled_spi_3 = false;

	setDefaultCrankingFuel(engineConfiguration);
}

void setMiata1996(engine_configuration_s *engineConfiguration, board_configuration_s *boardConfiguration) {

	setDefaultCrankingFuel(engineConfiguration);
}

