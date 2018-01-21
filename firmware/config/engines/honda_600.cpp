/*
 * @file honda_600.cpp
 *
 * set engine_type 43
 *
 * @date Jul 9, 2016
 * @author Andrey Belomutskiy, (c) 2012-2018
 */

#include "engine_template.h"
#include "honda_600.h"
#include "allsensors.h"
#include "engine_math.h"
#include "custom_engine.h"

#if EFI_PROD_CODE
#include "can_hw.h"
#endif
static const fuel_table_t default_custom_fuel_table = {
  {1.8,  1.8,  1.65, 1.65, 1.65, 1.65, 1.5,  1.5,   2.025, 1.5,  1.5,  1.5,  1.5},
  {1.8,  1.8,  1.65, 1.65, 1.65, 1.65, 1.5,  1.5,   2.025, 1.5,  1.5,  1.5,  1.5},
  {1.95, 1.95, 1.95, 1.8,  2.1,  2.25, 2.34, 2.25,	2.7,   2.7,  2.7,  2.7,  2.7},
  {2.55, 2.55, 2.55, 2.55, 2.55, 2.55, 2.55, 2.55,	2.4,   2.55, 3,    3,    2.7},
  {3,    3,    3,    3,    3,    3.3,  3.3,  3.45,	3,     3.3,  3.6,  3.6,  3.3},
  {3,    3,    3,    3,    3,    3,    3.6,	 4.305,	3.6,   3.6,  3.9,  3.9,  3.3},
  {3,    3,    3,    3,    3,    3,    3.6,	 3.75 , 3.9,   3.9,  4.2,  4.2,  3.9},
  {3,    3,    3,    3,    3,    3,    3.6,	 3.9,   4.2,   4.5,  4.5,  4.5,  4.5},
  {3,    3,    3,    3,    3,    3,    3,    3,     3,     3,    3,    3,    3},
  {3,    3,    3,    3,    3,    3,    3,    3,     3,     3,    3,    3,    3},
  {3,    3,    3,    3,    3,    3,    3,    3,     3,     3,    3,    3,    3}
};

static const ignition_table_t default_custom_timing_table = {
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

EXTERN_ENGINE;

static void setDefaultCustomMaps(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

	setFuelLoadBin(0,100 PASS_ENGINE_PARAMETER_SUFFIX);
	setFuelRpmBin(0, 7000 PASS_ENGINE_PARAMETER_SUFFIX);
	setTimingLoadBin(0,100 PASS_ENGINE_PARAMETER_SUFFIX);
	setTimingRpmBin(0,7000 PASS_ENGINE_PARAMETER_SUFFIX);

	copyFuelTable(default_custom_fuel_table, config->fuelTable);
  copyFuelTable(default_custom_fuel_table, config->veTable);
	copyTimingTable(default_custom_timing_table, config->ignitionTable);
}

void setHonda600(DECLARE_ENGINE_PARAMETER_SIGNATURE) {
	engineConfiguration->trigger.type = TT_HONDA_CBR_600_CUSTOM;
	engineConfiguration->fuelAlgorithm = LM_ALPHA_N;

	// upside down wiring
	boardConfiguration->triggerInputPins[0] = GPIOA_5;
	boardConfiguration->triggerInputPins[1] = GPIOC_6;


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

	setFrankenso_01_LCD(boardConfiguration);
	commonFrankensoAnalogInputs(engineConfiguration);
	setFrankenso0_1_joystick(engineConfiguration);
	setMap(config->injectionPhase, 320);
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
	engineConfiguration->tpsAdcChannel = EFI_ADC_2;

	engineConfiguration->map.sensor.hwChannel = EFI_ADC_0;

	engineConfiguration->clt.adcChannel = EFI_ADC_12;
	engineConfiguration->iat.adcChannel = EFI_ADC_11;
	engineConfiguration->afr.hwChannel = EFI_ADC_13;

	setCommonNTCSensor(&engineConfiguration->clt);
	engineConfiguration->clt.config.bias_resistor = 2700;
	setCommonNTCSensor(&engineConfiguration->iat);
	engineConfiguration->iat.config.bias_resistor = 2700;


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

	boardConfiguration->fuelPumpPin = GPIOE_4;
	boardConfiguration->mainRelayPin = GPIOD_7;
	boardConfiguration->idle.solenoidPin = GPIOC_13;

	boardConfiguration->fanPin = GPIOE_5;

	boardConfiguration->injectionPins[0] = GPIOB_9; // #1
	boardConfiguration->injectionPins[1] = GPIOD_5; // #2
	boardConfiguration->injectionPins[2] = GPIOB_7; // #3
	boardConfiguration->injectionPins[3] = GPIOB_8; // #4

  setDefaultCustomMaps(PASS_ENGINE_PARAMETER_SIGNATURE);
	setAlgorithm(LM_ALPHA_N PASS_ENGINE_PARAMETER_SUFFIX);

	boardConfiguration->injectionPins[4] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[5] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[6] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[7] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[8] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[9] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[10] = GPIO_UNASSIGNED;
	boardConfiguration->injectionPins[11] = GPIO_UNASSIGNED;

	boardConfiguration->ignitionPins[0] = GPIOE_14;
	boardConfiguration->ignitionPins[1] = GPIOC_7;
	boardConfiguration->ignitionPins[2] = GPIOE_10;
	boardConfiguration->ignitionPins[3] = GPIOC_9; // #4

	// todo: 8.2 or 10k?
	engineConfiguration->vbattDividerCoeff = ((float) (10 + 33)) / 10 * 2;

#if EFI_PROD_CODE
	enableFrankensoCan();
#endif
}

