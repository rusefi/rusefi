/*
 * me7pnp.cpp
 *
 *  Created on: 1. mai 2019
 *      Author: Ola
 */

#include "pch.h"

#include "me7pnp.h"
#include "fsio_impl.h"
#include "cj125.h"

/**
 * set engine_type 102
 */
void vag_18_Turbo(DECLARE_CONFIG_PARAMETER_SIGNATURE) {

	//Base Engine Settings

	setOperationMode(engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
	engineConfiguration->trigger.type = TT_60_2_VW;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;
	engineConfiguration->injector.flow = 205;
	engineConfiguration->specs.displacement = 1.839;
	engineConfiguration->globalTriggerAngleOffset = 114;
	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS;
	engineConfiguration->crankingInjectionMode = IM_SEQUENTIAL;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;
	engineConfiguration->ignitionPinMode = OM_INVERTED;
	engineConfiguration->injectionPinMode = OM_OPENDRAIN_INVERTED;
	engineConfiguration->isCylinderCleanupEnabled = true;
	engineConfiguration->rpmHardLimit = 8000;
	engineConfiguration->cranking.baseFuel = 28;


	//Analog Inputs

	engineConfiguration->map.sensor.type = MT_GM_3_BAR;
	setCommonNTCSensor(&engineConfiguration->clt, 2700);
	setCommonNTCSensor(&engineConfiguration->iat, 2700);
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_7;
	//engineConfiguration->tps1_1AdcChannel = PF3;   TODO: ADC channel 3
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_10;
	engineConfiguration->clt.adcChannel = EFI_ADC_6;
	//engineConfiguration->iat.adcChannel = PF4;  TODO: ADC channel 3
	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;
	engineConfiguration->vbattAdcChannel = EFI_ADC_4;
	engineConfiguration->vbattDividerCoeff = ((float) (10.0 + 33)) / 10 * 2;
	engineConfiguration->mafAdcChannel = EFI_ADC_8;

	//CAN Settings

	engineConfiguration->canNbcType = CAN_BUS_NBC_VAG;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canWriteEnabled = true;

	engineConfiguration->canTxPin = GPIOB_6;
	engineConfiguration->canRxPin = GPIOB_12;

// Injectors

	engineConfiguration->injectionPins[0] = GPIOE_4; // #1
	engineConfiguration->injectionPins[1] = GPIOE_2; // #2
	engineConfiguration->injectionPins[2] = GPIOE_3; // #3
	engineConfiguration->injectionPins[3] = GPIOE_1; // #4
	engineConfiguration->injectionPins[4] = GPIOE_6; // #5
	engineConfiguration->injectionPins[5] = GPIOE_5; // #6
	engineConfiguration->injectionPins[6] = GPIOB_9; // #7
	engineConfiguration->injectionPins[7] = GPIOE_0; // #8

	//Ignition Outputs

	engineConfiguration->ignitionPins[0] = GPIOD_1; // #1
	engineConfiguration->ignitionPins[1] = GPIOD_6; // #2
	engineConfiguration->ignitionPins[2] = GPIOD_3; // #3
	engineConfiguration->ignitionPins[3] = GPIOD_4; // #4
	engineConfiguration->ignitionPins[4] = GPIOD_0; // #5
	engineConfiguration->ignitionPins[5] = GPIOD_2; // #6
	engineConfiguration->ignitionPins[6] = GPIOA_15; // #7
	engineConfiguration->ignitionPins[7] = GPIOC_12; // #8

	//SPI Settings
	engineConfiguration->is_enabled_spi_1 = true;
	engineConfiguration->is_enabled_spi_3 = false;

#if EFI_CJ125
	cj125defaultPinout(PASS_CONFIG_PARAMETER_SIGNATURE);
	engineConfiguration->cj125ur = EFI_ADC_11; // PC3
	engineConfiguration->cj125CsPin = GPIOB_11;
#endif

	engineConfiguration->debugMode = DBG_CJ125;

	//Digital Inputs/Outputs
#if (BOARD_TLE8888_COUNT > 0)
	engineConfiguration->tle8888spiDevice = SPI_DEVICE_1;
	engineConfiguration->fuelPumpPin = TLE8888_PIN_22;
	engineConfiguration->tachOutputPin = TLE8888_PIN_16;
	engineConfiguration->alternatorControlPin = TLE8888_PIN_17;
	engineConfiguration->auxPidPins[0] = TLE8888_PIN_6; // VVT solenoid control
#endif /* BOARD_TLE8888_COUNT */

	engineConfiguration->mainRelayPin = GPIO_UNASSIGNED;
	engineConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	engineConfiguration->fanPin = GPIO_UNASSIGNED;
	engineConfiguration->clutchDownPin = GPIOD_11;
	engineConfiguration->brakePedalPin = GPIOE_10;
	engineConfiguration->camInputs[0] = GPIOA_2;
#if defined(STM32_HAS_GPIOG) && STM32_HAS_GPIOG
//	engineConfiguration->triggerInputPins[0] = GPIOG_7;
#endif /* STM32_HAS_GPIOF */
#if defined(STM32_HAS_GPIOF) && STM32_HAS_GPIOF
//	engineConfiguration->vehicleSpeedSensorInputPin = GPIOF_14;
#endif /* STM32_HAS_GPIOF */


//Alternator Settings
	engineConfiguration->alternatorControlPinMode = OM_OPENDRAIN;
	engineConfiguration->targetVBatt = 13.8;
	engineConfiguration->alternatorControl.offset = 40;
	engineConfiguration->alternatorControl.pFactor = 14;
	engineConfiguration->alternatorControl.iFactor = 0.1;
	engineConfiguration->alternatorControl.dFactor = 0;
	engineConfiguration->alternatorControl.periodMs = 10;

	//ETB Settings

#if EFI_FSIO
#if defined(STM32_HAS_GPIOF) && STM32_HAS_GPIOF
	setFsio (14, GPIOF_13, "1" PASS_CONFIG_PARAMETER_SUFFIX);
#endif /* STM32_HAS_GPIOF */
	setFsioExt (3, GPIOE_0, "0.15 90 coolant 120 min max 90 - 30 / 0.8 * +", 25 PASS_CONFIG_PARAMETER_SUFFIX);
#endif
	// is this needed? engineConfiguration->auxPidFrequency[3] = 25;
	CONFIG(etb_use_two_wires) = true;
#if defined(STM32_HAS_GPIOF) && STM32_HAS_GPIOF
	CONFIG(etbIo[0].directionPin1) = GPIOF_15;
	CONFIG(etbIo[0].directionPin2) = GPIOF_14;
	CONFIG(etbIo[0].disablePin) = GPIOF_12;
#endif /* STM32_HAS_GPIOF */
	engineConfiguration->isHip9011Enabled = false;

#if EFI_FSIO
	setFsio (15, GPIOE_6, "1" PASS_CONFIG_PARAMETER_SUFFIX);
#endif
	CONFIG(etbIo[1].directionPin1) = GPIOE_2;
	CONFIG(etbIo[1].directionPin2) = GPIOE_4;
	CONFIG(etbIo[1].disablePin) = GPIOE_5;

	engineConfiguration->etb.pFactor = 1.07;
	engineConfiguration->etb.iFactor = 0.18;
	engineConfiguration->etb.dFactor = 0.24;
	engineConfiguration->etb.offset = 80;
	engineConfiguration->etb_iTermMin = -300;
	engineConfiguration->etb_iTermMax = 300;

	// VVT Settings

	engineConfiguration->auxPidFrequency[0] = 300;
	engineConfiguration->fsio_setting[0] = 0.0;
	engineConfiguration->auxPid[0].pFactor = 2;
	engineConfiguration->auxPid[0].iFactor = 0.005;
	engineConfiguration->auxPid[0].dFactor = 0;
	engineConfiguration->auxPid[0].offset = 33;
	engineConfiguration->auxPid[0].minValue = 24;
	engineConfiguration->auxPid[0].maxValue = 44;
	engineConfiguration->auxPidFrequency[0] = 300;
	engineConfiguration->fsio_setting[0] = 0.0;


	//AC Settings
	//Configuration 1 : bidirectional Wire
	//Configuration 2 : Over CAN with variable Kompressor(CAN: Input=B_sacc,B_skoc Output: B_kov)
	//Configuration 3 : 2 Wires

	engineConfiguration->acRelayPin = GPIO_UNASSIGNED;
	engineConfiguration->acIdleRpmBump = 200;
	// TODO: AC driver request input PE13 and AC compressor input-output PE7

	//Cruise Control
	//TODO: Implement Criuse Control

	//TODO: Thermostat Control TLE8888_PIN_5

	//TODO: N75 Boost Control TLE8888_PIN_21

}

