/**
 * @file boards/hellen/hellen121nissan/board_configuration.cpp
 *
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
	engineConfiguration->ignitionPins[0] = Gpio::C13;
	engineConfiguration->ignitionPins[1] = Gpio::E5;
	engineConfiguration->ignitionPins[2] = Gpio::E4;
	engineConfiguration->ignitionPins[3] = Gpio::E3;
	engineConfiguration->ignitionPins[4] = Gpio::E2;
	engineConfiguration->ignitionPins[5] = Gpio::B8;
	
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
	engineConfiguration->vbattAdcChannel = H144_IN_VBATT; // 4T

	engineConfiguration->adcVcc = 3.29f;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = H144_IN_CRANK;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = H144_IN_CAM;

	engineConfiguration->tps1_1AdcChannel = H144_IN_TPS;
	engineConfiguration->tps1_2AdcChannel = H144_IN_AUX1;

	engineConfiguration->throttlePedalPositionAdcChannel = EFI_ADC_3;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = EFI_ADC_14;
	engineConfiguration->mafAdcChannel = EFI_ADC_10;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_11;

	engineConfiguration->afr.hwChannel = EFI_ADC_1;

	engineConfiguration->clt.adcChannel = H144_IN_CLT;

	engineConfiguration->iat.adcChannel = H144_IN_IAT;

	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_NONE;
	engineConfiguration->auxTempSensor2.adcChannel = EFI_ADC_NONE;
}

void setBoardConfigOverrides() {
	setHellen144LedPins();
	setupVbatt();

    // hellen88bmw uses L6205PD
	engineConfiguration->etbIo[0].directionPin1 = H144_OUT_PWM2; // ETB+
	engineConfiguration->etbIo[0].directionPin2 = H144_OUT_PWM3; // ETB-
	engineConfiguration->etbIo[0].controlPin = H144_OUT_PWM1; // ETB_EN
	engineConfiguration->etb_use_two_wires = true;

	setHellenSdCardSpi3();

	engineConfiguration->clt.config.bias_resistor = 2700;
	engineConfiguration->iat.config.bias_resistor = 2700;
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
	engineConfiguration->canNbcType = CAN_BUS_NISSAN_VQ;

	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;

	engineConfiguration->fuelPumpPin = Gpio::D12;	// OUT_IO9 // 113 Fuel Pump Relay
	engineConfiguration->idle.solenoidPin = H144_OUT_PWM5;
//	engineConfiguration->fanPin = Gpio::D12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = Gpio::G14;	// pin: 111a, OUT_IO3
	engineConfiguration->tachOutputPin = H144_OUT_PWM7;

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();


	// Some sensible defaults for other options
	setCrankOperationMode();

	engineConfiguration->vvtCamSensorUseRise = true;
	engineConfiguration->useOnlyRisingEdgeForTrigger = true;
	setAlgorithm(LM_SPEED_DENSITY);


	// Bosch VQ40 VR56 VK56 0280158007
	engineConfiguration->injector.flow = 296.2;

	strcpy(engineConfiguration->engineMake, ENGINE_MAKE_NISSAN);

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SIMULTANEOUS;//IM_BATCH;// IM_SEQUENTIAL;

	engineConfiguration->luaOutputPins[0] = Gpio::G5; // 104 ETB Relay

	engineConfiguration->throttlePedalUpVoltage = 0.75;
	engineConfiguration->throttlePedalWOTVoltage = 4.45;
	engineConfiguration->throttlePedalSecondaryUpVoltage = 0.43;
	engineConfiguration->throttlePedalSecondaryWOTVoltage = 2.20;

	// this calibration reminds me of VAG just flipped?
	engineConfiguration->tpsMin = 100;
	engineConfiguration->tpsMax = 889;

	engineConfiguration->tps1SecondaryMin = 891;
	engineConfiguration->tps1SecondaryMax = 102;
	hellenWbo();
}
