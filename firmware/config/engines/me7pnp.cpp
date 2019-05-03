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

void
vag_18_Turbo (DECLARE_ENGINE_PARAMETER_SIGNATURE)
{

  setOperationMode (engineConfiguration, FOUR_STROKE_CRANK_SENSOR);
  engineConfiguration->trigger.type = TT_60_2_VW;
  engineConfiguration->useOnlyRisingEdgeForTrigger = true;
  setAlgorithm (LM_SPEED_DENSITY PASS_CONFIG_PARAMETER_SUFFIX);

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

  setCommonNTCSensor (&engineConfiguration->clt);
  engineConfiguration->clt.config.bias_resistor = 2700;
  setCommonNTCSensor (&engineConfiguration->iat);
  engineConfiguration->iat.config.bias_resistor = 2700;
  engineConfiguration->baroSensor.hwChannel = EFI_ADC_NONE;
  engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_NONE;

  engineConfiguration->tpsAdcChannel = EFI_ADC_4;
  engineConfiguration->map.sensor.hwChannel = EFI_ADC_7;
  engineConfiguration->clt.adcChannel = EFI_ADC_6;
  engineConfiguration->iat.adcChannel = EFI_ADC_2;
  engineConfiguration->afr.hwChannel = EFI_ADC_NONE;
  engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_13;

  engineConfiguration->tle8888spiDevice = SPI_DEVICE_1;

  engineConfiguration->canNbcType = CAN_BUS_NBC_VAG;
  engineConfiguration->canReadEnabled = true;
  engineConfiguration->canWriteEnabled = true;
  boardConfiguration->canDeviceMode = CD_USE_CAN1;
  boardConfiguration->canTxPin = GPIOB_6;
  boardConfiguration->canRxPin = GPIOB_12;

  boardConfiguration->injectionPins[0] = GPIOG_15; // #1
  boardConfiguration->injectionPins[1] = GPIOG_14; // #2
  boardConfiguration->injectionPins[2] = GPIOG_9; // #3
  boardConfiguration->injectionPins[3] = GPIOC_9; // #4
  boardConfiguration->injectionPins[4] = GPIOE_0; // #5
  boardConfiguration->injectionPins[5] = GPIOE_1; // #6
  boardConfiguration->injectionPins[6] = GPIOB_9; // #7
  boardConfiguration->injectionPins[7] = GPIOB_8; // #8

  boardConfiguration->ignitionPins[0] = GPIOG_3;
  boardConfiguration->ignitionPins[1] = GPIOG_4;
  boardConfiguration->ignitionPins[2] = GPIOG_5;
  boardConfiguration->ignitionPins[3] = GPIOG_6;
  boardConfiguration->ignitionPins[4] = GPIOG_8;
  boardConfiguration->ignitionPins[5] = GPIOC_7;
  boardConfiguration->ignitionPins[6] = GPIOC_6;
  boardConfiguration->ignitionPins[7] = GPIOG_7;
  boardConfiguration->ignitionPins[8] = GPIO_UNASSIGNED;
  boardConfiguration->ignitionPins[9] = GPIO_UNASSIGNED;

  engineConfiguration->map.sensor.type = MT_GM_3_BAR;
  boardConfiguration->fuelPumpPin = TLE8888_PIN_20;
  boardConfiguration->mainRelayPin = GPIO_UNASSIGNED;
  boardConfiguration->idle.solenoidPin = GPIO_UNASSIGNED;
  boardConfiguration->fanPin = GPIO_UNASSIGNED;
  boardConfiguration->clutchDownPin = GPIOD_11;
  engineConfiguration->camInput = GPIOA_2;
  boardConfiguration->triggerInputPins[0] = GPIOG_7;

  engineConfiguration->targetVBatt = 13.8;
  engineConfiguration->alternatorControl.offset = 40;
  engineConfiguration->alternatorControl.pFactor = 14;
  engineConfiguration->alternatorControl.iFactor = 0.1;
  engineConfiguration->alternatorControl.dFactor = 0;
  engineConfiguration->alternatorControl.periodMs = 10;

  engineConfiguration->auxPid[0].pFactor = 2;
  engineConfiguration->auxPid[0].iFactor = 0.005;
  engineConfiguration->auxPid[0].dFactor = 0;
  engineConfiguration->auxPid[0].offset = 33;
  engineConfiguration->auxPid[0].minValue = 24;
  engineConfiguration->auxPid[0].maxValue = 44;

#if EFI_FSIO
  setFsio (12, GPIOF_12, "0" PASS_ENGINE_PARAMETER_SUFFIX);
  setFsio (14, GPIOF_13, "1" PASS_ENGINE_PARAMETER_SUFFIX);
  setFsioExt (3, GPIOE_0, "0.15 90 coolant 120 min max 90 - 30 / 0.8 * +", 25 PASS_ENGINE_PARAMETER_SUFFIX);
#endif
  engineConfiguration->auxPidFrequency[3] = 25;
  CONFIG(etb1_use_two_wires) = true;
  CONFIGB(etb1.directionPin1) = GPIOF_15;
  CONFIGB(etb1.directionPin2) = GPIOF_14;
  boardConfiguration->isHip9011Enabled = false;

#if EFI_FSIO
  setFsio (13, GPIOE_5, "0" PASS_ENGINE_PARAMETER_SUFFIX);
  setFsio (15, GPIOE_6, "1" PASS_ENGINE_PARAMETER_SUFFIX);
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

}
