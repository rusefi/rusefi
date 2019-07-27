/*
 * me7pnp.cpp
 *
 *  Created on: 1. mai 2019
 *      Author: Ola
 */

#include "me7pnp.h"

#include "global.h"
#include "engine.h"
#include "engine_math.h"
#include "allsensors.h"
#include "fsio_impl.h"
#include "engine_configuration.h"
#include "smart_gpio.h"

EXTERN_ENGINE
;

void vag_18_Turbo(DECLARE_ENGINE_PARAMETER_SIGNATURE) {

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
	boardConfiguration->ignitionPinMode = OM_INVERTED;
	boardConfiguration->injectionPinMode = OM_OPENDRAIN_INVERTED;
	engineConfiguration->isCylinderCleanupEnabled = true;
	engineConfiguration->rpmHardLimit = 8000;
	engineConfiguration->cranking.baseFuel = 4;


	//Analog Inputs

	boardConfiguration->isFastAdcEnabled = true;
	engineConfiguration->map.sensor.type = MT_GM_3_BAR;
	setCommonNTCSensor(&engineConfiguration->clt);
	engineConfiguration->clt.config.bias_resistor = 2700;
	setCommonNTCSensor(&engineConfiguration->iat);
	engineConfiguration->iat.config.bias_resistor = 2700;
	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_7;
	//engineConfiguration->tps1_1AdcChannel = PF3;   TODO: ADC channel 3
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_10;
	engineConfiguration->clt.adcChannel = EFI_ADC_6;
	//engineConfiguration->iat.adcChannel = PF4;  TODO: ADC channel 3
	engineConfiguration->afr.hwChannel = EFI_ADC_NONE;
	engineConfiguration->vbattAdcChannel = EFI_ADC_4;
	engineConfiguration->vbattDividerCoeff = ((float) (10.0 + 33)) / 10 * 2;
	engineConfiguration->cj125ur = EFI_ADC_11;
	engineConfiguration->cj125ua = EFI_ADC_12;
	engineConfiguration->mafAdcChannel = EFI_ADC_8;

	//CAN Settings

	engineConfiguration->canNbcType = CAN_BUS_NBC_VAG;
	engineConfiguration->canReadEnabled = true;
	engineConfiguration->canWriteEnabled = true;
	boardConfiguration->canDeviceMode = CD_USE_CAN1;
	boardConfiguration->canTxPin = GPIOB_6;
	boardConfiguration->canRxPin = GPIOB_12;

// Injectors

	boardConfiguration->injectionPins[0] = GPIOE_4; // #1
	boardConfiguration->injectionPins[1] = GPIOE_2; // #2
	boardConfiguration->injectionPins[2] = GPIOE_3; // #3
	boardConfiguration->injectionPins[3] = GPIOE_1; // #4
	boardConfiguration->injectionPins[4] = GPIOE_6; // #5
	boardConfiguration->injectionPins[5] = GPIOE_5; // #6
	boardConfiguration->injectionPins[6] = GPIOB_9; // #7
	boardConfiguration->injectionPins[7] = GPIOE_0; // #8

	//Ignition Outputs

	boardConfiguration->ignitionPins[0] = GPIOD_1; // #1
	boardConfiguration->ignitionPins[1] = GPIOD_6; // #2
	boardConfiguration->ignitionPins[2] = GPIOD_3; // #3
	boardConfiguration->ignitionPins[3] = GPIOD_4; // #4
	boardConfiguration->ignitionPins[4] = GPIOD_0; // #5
	boardConfiguration->ignitionPins[5] = GPIOD_2; // #6
	boardConfiguration->ignitionPins[6] = GPIOA_15; // #7
	boardConfiguration->ignitionPins[7] = GPIOC_12; // #8

	//SPI Settings
	boardConfiguration->is_enabled_spi_1 = true;
	boardConfiguration->is_enabled_spi_2 = true;
	boardConfiguration->is_enabled_spi_3 = false;
	engineConfiguration->cj125SpiDevice = SPI_DEVICE_2;
	boardConfiguration->cj125CsPin = GPIOB_11;

	//Digital Inputs/Outputs
#if (BOARD_TLE8888_COUNT > 0)
	engineConfiguration->tle8888spiDevice = SPI_DEVICE_1;
	boardConfiguration->fuelPumpPin = TLE8888_PIN_22;
	boardConfiguration->tachOutputPin = TLE8888_PIN_16;
	boardConfiguration->alternatorControlPin = TLE8888_PIN_17;
	engineConfiguration->auxPidPins[0] = TLE8888_PIN_6; // VVT solenoid control
#endif /* BOARD_TLE8888_COUNT */

	boardConfiguration->mainRelayPin = GPIO_UNASSIGNED;
	boardConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
	boardConfiguration->fanPin = GPIO_UNASSIGNED;
	boardConfiguration->clutchDownPin = GPIOD_11;
	engineConfiguration->brakePedalPin = GPIOE_10;
	engineConfiguration->camInputs[0] = GPIOA_2;
#if defined(STM32_HAS_GPIOG) && STM32_HAS_GPIOG
	boardConfiguration->triggerInputPins[0] = GPIOG_7;
#endif /* STM32_HAS_GPIOF */
#if defined(STM32_HAS_GPIOF) && STM32_HAS_GPIOF
	boardConfiguration->vehicleSpeedSensorInputPin = GPIOF_14;
#endif /* STM32_HAS_GPIOF */


//Alternator Settings
	boardConfiguration->alternatorControlPinMode = OM_OPENDRAIN;
	engineConfiguration->targetVBatt = 13.8;
	engineConfiguration->alternatorControl.offset = 40;
	engineConfiguration->alternatorControl.pFactor = 14;
	engineConfiguration->alternatorControl.iFactor = 0.1;
	engineConfiguration->alternatorControl.dFactor = 0;
	engineConfiguration->alternatorControl.periodMs = 10;

	//ETB Settings

#if EFI_FSIO
#if defined(STM32_HAS_GPIOF) && STM32_HAS_GPIOF
	setFsio (12, GPIOF_12, "0" PASS_CONFIG_PARAMETER_SUFFIX);
	setFsio (14, GPIOF_13, "1" PASS_CONFIG_PARAMETER_SUFFIX);
#endif /* STM32_HAS_GPIOF */
	setFsioExt (3, GPIOE_0, "0.15 90 coolant 120 min max 90 - 30 / 0.8 * +", 25 PASS_CONFIG_PARAMETER_SUFFIX);
#endif
	engineConfiguration->auxPidFrequency[3] = 25;
	CONFIG(etb1_use_two_wires) = true;
#if defined(STM32_HAS_GPIOF) && STM32_HAS_GPIOF
	CONFIGB(etb1.directionPin1) = GPIOF_15;
	CONFIGB(etb1.directionPin2) = GPIOF_14;
#endif /* STM32_HAS_GPIOF */
	boardConfiguration->isHip9011Enabled = false;

#if EFI_FSIO
	setFsio (13, GPIOE_5, "0" PASS_CONFIG_PARAMETER_SUFFIX);
	setFsio (15, GPIOE_6, "1" PASS_CONFIG_PARAMETER_SUFFIX);
#endif
	CONFIG(etb2_use_two_wires) = true;
	CONFIG(etb2.directionPin1) = GPIOE_2;
	CONFIG(etb2.directionPin2) = GPIOE_4;

	engineConfiguration->etb.pFactor = 1.07;
	engineConfiguration->etb.iFactor = 0.18;
	engineConfiguration->etb.dFactor = 0.24;
	engineConfiguration->etb.offset = 80;
	engineConfiguration->etb_iTermMin = -300;
	engineConfiguration->etb_iTermMax = 300;

	// VVT Settings

	engineConfiguration->activateAuxPid1 = true;
	engineConfiguration->auxPidFrequency[0] = 300;
	boardConfiguration->fsio_setting[0] = 0.0;
	engineConfiguration->auxPid[0].pFactor = 2;
	engineConfiguration->auxPid[0].iFactor = 0.005;
	engineConfiguration->auxPid[0].dFactor = 0;
	engineConfiguration->auxPid[0].offset = 33;
	engineConfiguration->auxPid[0].minValue = 24;
	engineConfiguration->auxPid[0].maxValue = 44;
	engineConfiguration->auxPidFrequency[0] = 300;
	boardConfiguration->fsio_setting[0] = 0.0;


	//AC Settings
	//Configuration 1 : bidirectional Wire
	//Configuration 2 : Over CAN with variable Kompressor(CAN: Input=B_sacc,B_skoc Output: B_kov)
	//Configuration 3 : 2 Wires

	boardConfiguration->acRelayPin = GPIO_UNASSIGNED;
	engineConfiguration->acCutoffLowRpm = 400;
	engineConfiguration->acCutoffHighRpm = 4500;
	engineConfiguration->acIdleRpmBump = 200;
	// TODO: AC driver request input PE13 and AC compressor input-output PE7

	//Cruise Control
	//TODO: Implement Criuse Control

	//TODO: Thermostat Control TLE8888_PIN_5

	//TODO: N75 Boost Control TLE8888_PIN_21

}

