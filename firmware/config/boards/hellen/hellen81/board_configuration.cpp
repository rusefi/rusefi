/**
 * @file boards/hellen/hellen81/board_configuration.cpp
 *
 *
 * @brief Configuration defaults for the Hellen81 board
 *
 * See https://rusefi.com/s/hellen81
 *
 * @author andreika <prometheus.pcb@gmail.com>
 * @author Andrey Belomutskiy, (c) 2012-2021
 */

#include "pch.h"
#include "hellen_meta.h"

static void setInjectorPins() {
	engineConfiguration->injectionPins[0] = Gpio::G7;
	engineConfiguration->injectionPins[1] = Gpio::G8;
	engineConfiguration->injectionPins[2] = Gpio::D11;
	engineConfiguration->injectionPins[3] = Gpio::D10;

	//engineConfiguration->injectionPins[4] = Gpio::D9;
	//engineConfiguration->injectionPins[5] = Gpio::F12;
	//engineConfiguration->injectionPins[6] = Gpio::F13;
	//engineConfiguration->injectionPins[7] = Gpio::F14;

	// Disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT;i++) {
		engineConfiguration->injectionPins[i] = Gpio::Unassigned;
	}

	engineConfiguration->injectionPinMode = OM_DEFAULT;
}

static void setIgnitionPins() {
	engineConfiguration->ignitionPins[0] = Gpio::I8;
	engineConfiguration->ignitionPins[1] = Gpio::E5;
	engineConfiguration->ignitionPins[2] = Gpio::E4;
	engineConfiguration->ignitionPins[3] = Gpio::E3;

	// disable remainder
	for (int i = 4; i < MAX_CYLINDER_COUNT; i++) {
		engineConfiguration->ignitionPins[i] = Gpio::Unassigned;
	}

	engineConfiguration->ignitionPinMode = OM_DEFAULT;
}

static void setLedPins() {
#ifdef EFI_COMMUNICATION_PIN
	engineConfiguration->communicationLedPin = EFI_COMMUNICATION_PIN;
#else
	engineConfiguration->communicationLedPin = Gpio::H10;
#endif /* EFI_COMMUNICATION_PIN */

	//!!!!!!!!!!!
	engineConfiguration->runningLedPin = Gpio::Unassigned;
	//engineConfiguration->runningLedPin = Gpio::H9;  // green
	
	engineConfiguration->warningLedPin = Gpio::H11; // yellow
}

static void setupVbatt() {
	// 4.7k high side/4.7k low side = 2.0 ratio divider
	engineConfiguration->analogInputDividerCoefficient = 2.0f;

	// set vbatt_divider 5.835
	// 33k / 6.8k
	engineConfiguration->vbattDividerCoeff = (33 + 6.8) / 6.8; // 5.835

	// pin input +12 from Main Relay
	engineConfiguration->vbattAdcChannel = EFI_ADC_5;

	engineConfiguration->adcVcc = 3.29f;
}

static void setupDefaultSensorInputs() {
	// trigger inputs
	engineConfiguration->triggerInputPins[0] = Gpio::B1;
	engineConfiguration->triggerInputPins[1] = Gpio::Unassigned;
	// Direct hall-only cam input
	engineConfiguration->camInputs[0] = Gpio::A6;

	engineConfiguration->tps1_1AdcChannel = EFI_ADC_4;
	engineConfiguration->tps2_1AdcChannel = EFI_ADC_NONE;

	engineConfiguration->mafAdcChannel = EFI_ADC_NONE;
	engineConfiguration->map.sensor.hwChannel = EFI_ADC_10;

	engineConfiguration->afr.hwChannel = EFI_ADC_0;	// ADC1_16

	engineConfiguration->clt.adcChannel = H144_IN_CLT;	// ADC3_0

	engineConfiguration->iat.adcChannel = H144_IN_IAT;	// ADC3_1

	engineConfiguration->auxTempSensor1.adcChannel = EFI_ADC_NONE;
	engineConfiguration->auxTempSensor2.adcChannel = EFI_ADC_NONE;
}

void setBoardConfigOverrides() {
	setLedPins();
	setupVbatt();

// Hellen81a uses SPI2 for SD-card
#if 1
	setHellenSdCardSpi2();
#else
	setHellenSdCardSpi3();
#endif

	engineConfiguration->clt.config.bias_resistor = 4700;
	engineConfiguration->iat.config.bias_resistor = 4700;

	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;
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

	engineConfiguration->isSdCardEnabled = true;

	engineConfiguration->canTxPin = Gpio::D1;
	engineConfiguration->canRxPin = Gpio::D0;

	engineConfiguration->fuelPumpPin = Gpio::G2;	// OUT_IO9
	engineConfiguration->fanPin = Gpio::D12;	// OUT_PWM8
	engineConfiguration->mainRelayPin = Gpio::I2;	// OUT_LOW3
	engineConfiguration->tachOutputPin = Gpio::D14;	// OUT_PWM6

	engineConfiguration->useStepperIdle = true;
	engineConfiguration->useHbridgesToDriveIdleStepper = true;
	engineConfiguration->stepperDcIo[0].directionPin1 = Gpio::C6;	// COIL_A1 = OUT_PWM2
	engineConfiguration->stepperDcIo[0].directionPin2 = Gpio::C7;	// COIL_A2 = OUT_PWM3
	engineConfiguration->stepperDcIo[1].directionPin1 = Gpio::C8;	// COIL_B1 = OUT_PWM4
	engineConfiguration->stepperDcIo[1].directionPin2 = Gpio::C9;	// COIL_B2 = OUT_PWM5

	// "required" hardware is done - set some reasonable defaults
	setupDefaultSensorInputs();

	engineConfiguration->specs.cylindersCount = 4;
	engineConfiguration->specs.firingOrder = FO_1_3_4_2;

	engineConfiguration->ignitionMode = IM_INDIVIDUAL_COILS; // IM_WASTED_SPARK
	engineConfiguration->crankingInjectionMode = IM_SIMULTANEOUS;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;	// IM_SIMULTANEOUS; //IM_BATCH;
}
