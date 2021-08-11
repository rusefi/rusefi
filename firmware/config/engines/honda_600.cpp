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

#if EFI_PROD_CODE
#include "can_hw.h"
#endif

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
static const uint8_t default_custom_timing_table[16][16] = {
  /* RPM			   0   500   1000   1500   2000   2500   3000   3500   4000    4500   5000    5500   6000   6500   7000	 */
  /* Load  0% */{	10,  10,    10,    12,    12,   12,    12,   15,    15,     15,    15,     26,    28,    30,    32},
  /* Load 10% */{	10,  10,    10,    12,    12,   12,    12,   15,    15,     15,    15,     26,    28,    30,    32},
  /* Load 20% */{	10,  10,    10,    10,    10,   10,    12,   15,    20,     22,    24,     26,    28,    30,    32},
  /* Load 30% */{ 10,  10,    10,    10,    10,   10,    12,   15,    20,     22,    24,     26,    28,    30,    32},
  /* Load 40% */{ 10,  10,    10,    10,    10,   10,    12,   15,    20,     22,    24,     26,    28,    30,    32},
  /* Load 50% */{ 10,  10,    10,    10,    10,   10,    12,   15,    20,     22,    24,     26,    28,    30,    32},
  /* Load 60% */{ 10,  10,    10,    10,    10,   10,    15,   18,    20,     22,    24,     26,    28,    30,    32},
  /* Load 70% */{ 10,  10,    10,    10,    10,   10,    15,   18,    20,     22,    24,     26,    28,    30,    32},
  /* Load 80% */{ 10,  10,    10,    10,    10,   10,    15,   18,    20,     22,    24,     26,    28,    30,    32},
  /* Load 90% */{ 10,  10,    10,    10,    10,   10,    15,   18,    20,     22,    24,     26,    28,    30,    32},
 /* Load 100% */{ 10,  10,    10,    10,    10,   10,    15,   18,    20,     22,    24,     26,    28,    30,    32},
};
#endif

static void setDefaultCustomMaps(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setTimingLoadBin(0,100 PASS_CONFIG_PARAMETER_SUFFIX);
	setTimingRpmBin(0,7000 PASS_CONFIG_PARAMETER_SUFFIX);

#if IGN_LOAD_COUNT == DEFAULT_IGN_LOAD_COUNT
	copyTable(config->ignitionTable, default_custom_timing_table);
#endif
}

void setHonda600(DECLARE_CONFIG_PARAMETER_SIGNATURE) {
	setDefaultFrankensoConfiguration(PASS_CONFIG_PARAMETER_SIGNATURE);
	engineConfiguration->trigger.type = TT_HONDA_CBR_600;
	engineConfiguration->fuelAlgorithm = LM_ALPHA_N;

	// upside down wiring
	engineConfiguration->triggerInputPins[0] = GPIOA_5;
	engineConfiguration->triggerInputPins[1] = GPIOC_6;


	// set global_trigger_offset_angle 180
	// set global_trigger_offset_angle 540
	engineConfiguration->globalTriggerAngleOffset = 540;

    engineConfiguration->specs.cylindersCount = 4;
    engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
  engineConfiguration->specs.firingOrder = FO_1_3_4_2;
  engineConfiguration->extraInjectionOffset = 320;
  engineConfiguration->cranking.rpm = 800;
//	engineConfiguration->ignitionMode = IM_WASTED_SPARK; //IM_INDIVIDUAL_COILS;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;


  //setIndividualCoilsIgnition();

	setFrankenso_01_LCD(engineConfiguration);
	commonFrankensoAnalogInputs(engineConfiguration);
	setFrankenso0_1_joystick(engineConfiguration);
	setTable(config->injectionPhase, 320.0f);

	/**
	 * Frankenso analog #1 PC2 ADC12 CLT
	 * Frankenso analog #2 PC1 ADC11 IAT
	 * Frankenso analog #3 PA0 ADC0 MAP
	 * Frankenso analog #4 PC3 ADC13 WBO / O2
	 * Frankenso analog #5 PA2 ADC2 TPS
	 * Frankenso analog #6 PA1 ADC1
	 * Frankenso analog #7 PA4 ADC4
	 * Frankenso analog #8 PA3 ADC3
	 * Frankenso analog #9 PA7 ADC7
	 * Frankenso analog #10 PA6 ADC6
	 * Frankenso analog #11 PC5 ADC15
	 * Frankenso analog #12 PC4 ADC14 VBatt
	 */
	engineConfiguration->tps1_1AdcChannel = EFI_ADC_2;

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0;

	engineConfiguration->clt.adcChannel = EFI_ADC_12;
	engineConfiguration->iat.adcChannel = EFI_ADC_11;
	engineConfiguration->afr.hwChannel = EFI_ADC_13;

	setCommonNTCSensor(&engineConfiguration->clt, 2700);
	setCommonNTCSensor(&engineConfiguration->iat, 2700);


	/**
	 * http://rusefi.com/wiki/index.php?title=Manual:Hardware_Frankenso_board
	 */
	// Frankenso low out #1: PE6
	// Frankenso low out #2: PE5
	// Frankenso low out #3: PD7 Main Relay
	// Frankenso low out #4: PC13 Idle valve solenoid
	// Frankenso low out #5: PE3
	// Frankenso low out #6: PE4 fuel pump relay
	// Frankenso low out #7: PE1 (do not use with discovery!)
	// Frankenso low out #8: PE2 injector #2
	// Frankenso low out #9: PB9 injector #1
	// Frankenso low out #10: PE0 (do not use with discovery!)
	// Frankenso low out #11: PB8 injector #3
	// Frankenso low out #12: PB7 injector #4

	engineConfiguration->fuelPumpPin = GPIOE_4;
	engineConfiguration->mainRelayPin = GPIOD_7;
	engineConfiguration->idle.solenoidPin = GPIOC_13;

	engineConfiguration->fanPin = GPIOE_5;

	engineConfiguration->injectionPins[0] = GPIOB_9; // #1
	engineConfiguration->injectionPins[1] = GPIOD_5; // #2
	engineConfiguration->injectionPins[2] = GPIOB_7; // #3
	engineConfiguration->injectionPins[3] = GPIOB_8; // #4

	setDefaultCustomMaps(PASS_CONFIG_PARAMETER_SIGNATURE);
	setAlgorithm(LM_ALPHA_N PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[6] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[7] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[8] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[9] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[10] = GPIO_UNASSIGNED;
	engineConfiguration->injectionPins[11] = GPIO_UNASSIGNED;

	engineConfiguration->ignitionPins[0] = GPIOE_14;
	engineConfiguration->ignitionPins[1] = GPIOC_7;
	engineConfiguration->ignitionPins[2] = GPIOE_10;
	engineConfiguration->ignitionPins[3] = GPIOC_9; // #4

	// todo: 8.2 or 10k?
	engineConfiguration->vbattDividerCoeff = ((float) (10 + 33)) / 10 * 2;

#if EFI_CAN_SUPPORT
	enableFrankensoCan();
#endif /* EFI_CAN_SUPPORT */
}

