/**
 * @file boards/hellen/hellen121nissan/board_configuration.cpp
 *
 * All fabricated boards use 144 pin MCU
 *
 * @brief Configuration defaults for the hellen121nissan board
 *
 * See https://rusefi.com/s/hellen121nissan
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2020
 */

#include "pch.h"
#include "custom_engine.h"
#include "hellen_meta.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = H144_LS_1;
	engineConfiguration->injectionPins[1] = H144_LS_2;
	engineConfiguration->injectionPins[2] = H144_LS_3;
	engineConfiguration->injectionPins[3] = H144_LS_4;
	engineConfiguration->injectionPins[4] = H144_LS_5;
	engineConfiguration->injectionPins[5] = H144_LS_6;

	// Disable remainder
	for (int i = 6; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->injectionPins[i] = Gpio::Unassigned;
	}

	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = H144_IGN_1;
	engineConfiguration->ignitionPins[1] = H144_IGN_2;
	engineConfiguration->ignitionPins[2] = H144_IGN_3;
	engineConfiguration->ignitionPins[3] = H144_IGN_4;
	engineConfiguration->ignitionPins[4] = H144_IGN_5;
	engineConfiguration->ignitionPins[5] = H144_IGN_6;
	
	// disable remainder
	for (int i = 6; i < MAX_CYLINDER_COUNT; i++) {
		engineConfiguration->ignitionPins[i] = Gpio::Unassigned;
	}

	engineConfiguration->ignitionPinMode = OM_DEFAULT;
}

static void setupVbatt() {
	// 4.7k high side/4.7k low side = 2.0 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	// set vbatt_divider 5.835
	// 33k / 6.8k
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8; // 5.835

	// pin input +12 from Main Relay
	engineConfiguration->vbattAdcChannel = EFI_ADC_5; // 4T

	engineConfiguration->adcVcc = 3.29f;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = H144_IN_CRANK;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = H144_IN_CAM;
	// todo: remove from default since 4 cylinder does not use it
	// todo: this requires unit test change
	engineConfiguration->camInputs[1 * CAMS_PER_BANK] = H144_IN_D_AUX4;

	engineConfiguration->tps1_1AdcChannel = H144_IN_TPS;
	engineConfiguration->tps1_2AdcChannel = H144_IN_AUX1;

	engineConfiguration->throttlePedalPositionAdcChannel = H144_IN_PPS;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_14;
	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engineConfiguration->map.sensor.hwChannel = H144_IN_MAP2;

	engineConfiguration->afr.hwChannel = EFI_ADC_1;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;

	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_NONE;
	engineConfiguration->auxTempSensor2.adcChannel = EFI_ADC_NONE;
}

void setBoardConfigOverrides() {
	setHellen144LedPins();
	setupVbatt();
	setHellenSdCardSpi3();

	engineConfiguration->etbIo[0].directionPin1 = H144_OUT_PWM7;
	engineConfiguration->etbIo[0].directionPin2 = H144_OUT_PWM6;
	engineConfiguration->etbIo[0].controlPin = Gpio::D13; // ETB_EN out_pwm1
	engineConfiguration->etb_use_two_wires = true;

	engineConfiguration->clt.config.bias_resistor = 4700;
	engineConfiguration->iat.config.bias_resistor = 4700;
}

/**
 * @brief   Board-specific configuration defaults.
 *
 * See also setDefaultEngineConfiguration
 *
 * @todo    Add your board-specific code, if any.
 */
void setBoardDefaultConfiguration() {
	setInjectorPins();
	setIgnitionPins();

	engineConfiguration->displayLogicLevelsInEngineSniffer = true;
	engineConfiguration->isSdCardEnabled = true;

	engineConfiguration->enableSoftwareKnock = true;
	engineConfiguration->canNbcType = CAN_BUS_NBC_NONE; // none because handled by Lua!

	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;

	engineConfiguration->fuelPumpPin = Gpio::D12;	// OUT_IO9 // 113 Fuel Pump Relay
	engineConfiguration->idle.solenoidPin = Gpio::Unassigned;
//	engineConfiguration->fanPin = Gpio::D12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = Gpio::G14;	// pin: 111a, OUT_IO3

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();


	// Some sensible defaults for other options
	setCrankOperationMode();

	engineConfiguration->vvtCamSensorUseRise = true;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
//	setAlgorithm(LM_SPEED_DENSITY);
    // at least this starts
	engineConfiguration->fuelAlgorithm = LM_ALPHA_N;

	engineConfiguration->cranking.rpm = 400;
	engineConfiguration->fanOnTemperature = 85;
	engineConfiguration->fanOffTemperature = 81;

	engineConfiguration->useETBforIdleControl = true;
	engineConfiguration->etbIdleThrottleRange = 10;
	engineConfiguration->cutFuelOnHardLimit = false;
	engineConfiguration->idlePidRpmUpperLimit = 300;
	engineConfiguration->mapErrorDetectionTooLow = 10;


	// Bosch VQ40 VR56 VK56 0280158007
	engineConfiguration->injector.flow = 296.2;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_NISSAN);

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	engineConfiguration->luaOutputPins[0] = Gpio::G5; // 104 ETB Relay

	engineConfiguration->throttlePedalUpVoltage = 0.75;
	engineConfiguration->throttlePedalWOTVoltage = 4.45;
	engineConfiguration->throttlePedalSecondaryUpVoltage = 0.43;
	engineConfiguration->throttlePedalSecondaryWOTVoltage = 2.20;

	engineConfiguration->startUpFuelPumpDuration = 4;
	engineConfiguration->postCrankingFactor = 1.05;

	engineConfiguration->etb.pFactor = 6.1350;
	engineConfiguration->etb.iFactor = 87.7182;
	engineConfiguration->etb.dFactor = 0.0702;

	// this calibration reminds me of VAG just flipped?
	engineConfiguration->tpsMin = 100;
	engineConfiguration->tpsMax = 889;

	engineConfiguration->tps1SecondaryMin = 891;
	engineConfiguration->tps1SecondaryMax = 102;
	hellenWbo();
}
