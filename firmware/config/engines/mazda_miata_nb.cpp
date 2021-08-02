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
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "mazda_miata_nb.h"
#include "thermistors.h"

void setMazdaMiataNb1EngineConfiguration(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setDefaultFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);

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

	engineConfiguration->clt.config = {0, 30, 100, 32500, 7550, 700, 2700};
	engineConfiguration->iat.config = {-10, 60, 120.00, 160310, 7700, 1180, 2700};

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_3; // 15 is the old value
	engineConfiguration->vbattAdcChannel = EFI_ADC_0; // 1 is the old value
//  engineConfiguration->map.channel = 1;
	engineConfiguration->mafAdcChannel = EFI_ADC_1;
	engineConfiguration->clt.adcChannel = EFI_ADC_11;
	engineConfiguration->iat.adcChannel = EFI_ADC_13;
	engineConfiguration->afr.hwChannel = EFI_ADC_2; // PA2

	engineConfiguration->idle.solenoidPin = GPIOE_0;
	engineConfiguration->idle.solenoidPinMode = OM_DEFAULT;

	engineConfiguration->fuelPumpPin = GPIOC_14; // Frankenstein: low side - out #4
	engineConfiguration->fuelPumpPinMode = OM_DEFAULT;

	engineConfiguration->injectionPins[0] = GPIOB_9; // Frankenstein: low side - out #12
	engineConfiguration->injectionPins[1] = GPIOB_8; // Frankenstein: low side - out #11
	engineConfiguration->injectionPins[2] = GPIOE_3; // Frankenstein: low side - out #8
	engineConfiguration->injectionPins[3] = GPIOE_5; // Frankenstein: low side - out #6
	engineConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPinMode = OM_DEFAULT;

	engineConfiguration->ignitionPins[0] = GPIOE_10; // Frankenstein: high side #1
	engineConfiguration->ignitionPins[1] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPins[2] = GPIOC_9; // // Frankenstein: high side #2
	engineConfiguration->ignitionPins[3] = GPIO_UNASSIGNED;
	engineConfiguration->ignitionPinMode = OM_INVERTED;

	engineConfiguration->malfunctionIndicatorPin = GPIOE_1;
	engineConfiguration->malfunctionIndicatorPinMode = OM_DEFAULT;

	engineConfiguration->fanPin = GPIOE_6;
	engineConfiguration->fanPinMode = OM_DEFAULT;

	engineConfiguration->clutchDownPin = GPIO_UNASSIGNED;

// 10 deg before TDC is default timing

	// VICS solenoid
	/**
	 * to test
	 * set_fsio_setting 1 5000
	 */
	engineConfiguration->fsio_setting[0] = 5000;
	// (self and (rpm > 4800)) OR (rpm > 5000)
	// set_rpn_expression 1 "self rpm 4800 > & rpm 5000 > OR"
//	setFsioExt(0, GPIOE_3, "self rpm 4800 > & rpm 5000 > OR", 150 PASS_CONFIG_PARAMETER_SUFFIX);
}
