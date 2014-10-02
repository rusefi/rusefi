/**
 * @file	dodge_neon.cpp
 *
 * DODGE_NEON_1995 = 2
 *
 * DODGE_NEON_2003 = 23
 *
 * This config overrides some values of the default configuration which is set by setDefaultConfiguration() method
 *
 * @date Dec 16, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#include "main.h"

#if EFI_SUPPORT_DODGE_NEON || defined(__DOXYGEN__)

#include "dodge_neon.h"
#include "engine_configuration.h"
#include "thermistors.h"
#include "engine_math.h"

void setDodgeNeon1995EngineConfiguration(engine_configuration_s *engineConfiguration,
		board_configuration_s *boardConfiguration) {

	engineConfiguration->triggerConfig.triggerType = TT_DODGE_NEON_1995;

	engineConfiguration->algorithm = LM_ALPHA_N;

	// set_rpm_hard_limit 4000
	engineConfiguration->rpmHardLimit = 4000; // yes, 4k. let's play it safe for now
	// set_cranking_rpm 550
	engineConfiguration->crankingSettings.crankingRpm = 550;

	// since CLT is not wired up yet let's just use same value for min and max
	// set_cranking_fuel_max 6 40
	engineConfiguration->crankingSettings.coolantTempMaxC = 37.7; // 6ms at 37.7C
	engineConfiguration->crankingSettings.fuelAtMaxTempMs = 6;

	// set_cranking_fuel_min 6 -40
	engineConfiguration->crankingSettings.coolantTempMinC = -40; // 6ms at -40C
	engineConfiguration->crankingSettings.fuelAtMinTempMs = 6;

	// set_whole_fuel_map 3
	setWholeFuelMap(engineConfiguration, 3);

	// set_cranking_injection_mode 0
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	// set_injection_mode 1
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	// set_ignition_mode 2
	engineConfiguration->ignitionMode = IM_WASTED_SPARK;
	// set_firing_order 2
	engineConfiguration->firingOrder = FO_1_THEN_3_THEN_4_THEN2;

	// set_global_trigger_offset_angle 497
	engineConfiguration->globalTriggerAngleOffset = 497;
	// set_ignition_offset 350
	engineConfiguration->ignitionOffset = 350;
	 // set_injection_offset 510
	engineConfiguration->injectionOffset = 510;

	// set_cranking_charge_angle 70
	engineConfiguration->crankingChargeAngle = 7;
	// set_cranking_timing_angle 0
	engineConfiguration->crankingTimingAngle = 0;

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

	// Frankenstein: high side #1: PE8
	// Frankenstein: high side #2: PE10

	boardConfiguration->ignitionPins[0] = GPIOE_8; // Frankenstein: high side #1
	boardConfiguration->ignitionPins[1] = GPIO_NONE;
	boardConfiguration->ignitionPins[2] = GPIOE_10; // // Frankenstein: high side #2

	// set_ignition_pin_mode 0
	boardConfiguration->ignitionPinMode = OM_DEFAULT;

	setThermistorConfiguration(&engineConfiguration->cltThermistorConf, 0, 32500, 30, 7550, 100, 700);
	engineConfiguration->cltThermistorConf.bias_resistor = 2700;

	engineConfiguration->analogChartFrequency = 7;
}

void setDodgeNeonNGCEngineConfiguration(engine_configuration_s *engineConfiguration,
		board_configuration_s *boardConfiguration) {
	engineConfiguration->triggerConfig.triggerType = TT_DODGE_NEON_2003;
}


#endif /* EFI_SUPPORT_DODGE_NEON */

