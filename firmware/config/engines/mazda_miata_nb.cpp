/**
 * @file	mazda_miata_nb.cpp
 *
 *
 * MAZDA_MIATA_NB1 = 9
 * set engine_type 9
 *
 * http://rusefi.com/forum/viewtopic.php?t=467
 * http://rusefi.com/forum/viewtopic.php?t=987
 *
 * @date Feb 18, 2014
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "mazda_miata_nb.h"
#include "thermistors.h"

EXTERN_ENGINE;

void setMazdaMiataNb1EngineConfiguration(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	// set_rpm_hard_limit 3000
	engineConfiguration->rpmHardLimit = 3000; // yes, 3k. let's play it safe for now

	engineConfiguration->trigger.type = TT_MAZDA_MIATA_NB1;

	// set cranking_injection_mode 0
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	// set ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	// set_firing_order 2
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	setThermistorConfiguration(&engineConfiguration->clt, 0, 32500, 30, 7550, 100, 700);
	engineConfiguration->clt.config.bias_resistor = 2700;

	setThermistorConfiguration(&engineConfiguration->iat, -10, 160310, 60, 7700, 120.00, 1180);
	engineConfiguration->iat.config.bias_resistor = 2700;

	engineConfiguration->tpsAdcChannel = EFI_ADC_3; // 15 is the old value
	engineConfiguration->vbattAdcChannel = EFI_ADC_0; // 1 is the old value
//  engineConfiguration->map.channel = 1;
	engineConfiguration->mafAdcChannel = EFI_ADC_1;
	engineConfiguration->clt.adcChannel = EFI_ADC_11;
	engineConfiguration->iat.adcChannel = EFI_ADC_13;
	engineConfiguration->afr.hwChannel = EFI_ADC_2;

	boardConfiguration->idle.solenoidPin = GPIOE_0;
	boardConfiguration->idle.solenoidPinMode = OM_DEFAULT;

	boardConfiguration->fuelPumpPin = GPIOC_14; // Frankenstein: low side - out #4
	boardConfiguration->fuelPumpPinMode = OM_DEFAULT;

	boardConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
	boardConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
	boardConfiguration->injectionPins[2] = GPIOE_3; // Frankenstein: low side - out #8
	boardConfiguration->injectionPins[3] = GPIOE_5; // Frankenstein: low side - out #6
	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPinMode = OM_DEFAULT;

	boardConfiguration->ignitionPins[0] = GPIOE_10; // Frankenstein: high side #1
	boardConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPins[2] = GPIOC_9; // // Frankenstein: high side #2
	boardConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	boardConfiguration->ignitionPinMode = OM_INVERTED;

	boardConfiguration->malfunctionIndicatorPin = GPIOE_1;
	boardConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	boardConfiguration->fanPin = GPIOE_6;
	boardConfiguration->fanPinMode = OM_DEFAULT;

	boardConfiguration->clutchDownPin = GPIO_UNASSIGNED;

	// set_whole_fuel_map 3
	setWholeFuelMap(3 PASS_ENGINE_PARAMETER_SUFFIX);


// 10 deg before TDC is default timing

	// VICS solenoid
	/**
	 * to test
	 * set_fsio_setting 1 5000
	 */
	boardConfiguration->fsio_setting[0] = 5000;
	// (self and (rpm > 4800)) OR (rpm > 5000)
	// set_rpn_expression 1 "self rpm 4800 > & rpm 5000 > OR"
//	setFsioExt(0, GPIOE_3, "self rpm 4800 > & rpm 5000 > OR", 150 PASS_ENGINE_PARAMETER_SUFFIX);
}
